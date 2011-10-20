#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <functional>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_01.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "move_and_forget_circle.h"

#define STEPDIV (1.0/4.0)
#define SIMFACT 1200
#define SEED 0xDEADBEEF
#define EPSILON (1.0/1024.0)

void simulator::create_ring(std::vector< simulator::node >& nodes)
{
  // get size of the ring
  std::size_t n = nodes.size();
  // initialization
  for (std::size_t i = 0; i < n; ++i) {
    nodes[i].id = i;
    nodes[i].left_neighbor = i == 0 ? n-1 : (i-1) % n;
    nodes[i].right_neighbor = (i+1) % n;
    nodes[i].long_range_link = i;
    nodes[i].long_range_link_age = 0;
  }
}

void simulator::create_rng(std::size_t id)
{
    // lock RNG map, then create and seed new Mersenne Twister instance
    #ifdef _OPENMP
       omp_set_lock(&simulator::generator_mutex);
    #endif
    simulator::generators[id] = boost::mt19937();
    simulator::generators[id].seed(simulator::seed+id);
    #ifdef _OPENMP
       omp_unset_lock(&simulator::generator_mutex);
    #endif
}


int main (int argc, const char* argv[])
{
  // check params
  if (argc < 3) {
    std::cout << "Usage: '" << argv[0] << " <n> <file_prefix> [<seed>]'." << std::endl;
    return 1;
  } else {
    simulator::n_nodes = atoi(argv[1]);
    simulator::path = argv[2];
    if (argc > 3) {
      simulator::seed = atoi(argv[3]);
    } else {
      simulator::seed = SEED; 
    }
  }
  
  // create ring
  std::cout << "Creating ring with " << simulator::n_nodes << " nodes." << std::endl;
  std::vector<simulator::node> nodes(simulator::n_nodes);
  simulator::create_ring(nodes);
  // fill probability cache
  std::cout << "Creating probability cache of size " << PROB_CACHE_SIZE << "." << std::endl;
  for (std::size_t i = 0; i < simulator::forgetting_probablities.size(); ++i) {
    simulator::forgetting_probablities[i] = simulator::forgetting_probablity(i);
  }
  // init mutex
  #ifdef _OPENMP
    omp_init_lock(&simulator::generator_mutex);
  #endif
  // start the 'simulation'
  std::cout << "Starting simulation." << std::endl;
  std::size_t round = 0;
  // configure cout
  std::cout.setf(std::ios::fixed, std::ios::floatfield);
  std::cout.precision(3);
  // setup time measurement
  boost::posix_time::ptime start, now;
  start = boost::posix_time::second_clock::local_time();
  while (round <= simulator::n_nodes*SIMFACT) {
    // compute  metrics
    simulator::evaluate_network(nodes, round);
    // measure progress and time
    double progress = (double)round / (simulator::n_nodes*SIMFACT);
    now = boost::posix_time::second_clock::local_time();
    std::string remaining("Unknown");
    if (progress > 0) {
      double secs = (now-start).total_seconds()/progress*(1.0-progress);
      boost::posix_time::time_duration duration(0, 0, secs, 0);
      remaining = boost::posix_time::to_simple_string(duration);
    }
    // simulate next rounds
    std::cout << "Simulating rounds " << round << " to " << (std::size_t)(round+simulator::n_nodes/STEPDIV) << " (progress: " << progress*100 << "%, est. time remaining: " << remaining << "). " << std::endl;
    simulator::random_walk(nodes, (std::size_t)(simulator::n_nodes/STEPDIV));
    round += (std::size_t)(simulator::n_nodes/STEPDIV);
  }
  // clear lock
  #ifdef _OPENMP
    omp_destroy_lock(&simulator::generator_mutex);
  #endif
  // stop
  return 0;
}

void simulator::random_walk(std::vector< simulator::node >& nodes, const std::size_t& steps)
{
  // get size of the ring
  std::size_t n = nodes.size();
  // perform random walks for each node (in parallel)
  std::size_t i, j, id, lrl, age;
  #pragma omp parallel for shared(nodes) private(i,j,id,lrl,age) schedule(dynamic)
  for (i = 0; i < n; ++i) {
    #ifdef _OPENMP
      id = omp_get_thread_num();
    #endif
    for (j = 0; j < steps; ++j) {
      lrl = nodes[i].long_range_link;
      age = nodes[i].long_range_link_age;
      if (simulator::forget_link(age, id)) {
		// reset long range link
		nodes[i].long_range_link = nodes[i].id;
		nodes[i].long_range_link_age = 0;
      } else {
		if (simulator::flip_coin(id)) {
		  // decrease long range link id
		  nodes[i].long_range_link = lrl == 0 ? n-1 : (lrl-1) % n;
		} else {
		  // increase long range link id
		  nodes[i].long_range_link = (lrl+1) % n;
		}
		++nodes[i].long_range_link_age;
	  }
    }
  }
}

inline bool simulator::flip_coin(const std::size_t& id)
{
  // does the RNG exist?
  if (simulator::generators.find(id) == simulator::generators.end()) {
    // if not create and seed new RNG
    simulator::create_rng(id);
  }
  // uniformly select an integer between 0 and 1
  boost::uniform_int<> distribution(0, 1);
  return distribution(simulator::generators[id]);
}

inline bool simulator::forget_link(const std::size_t& age, const std::size_t& id)
{
  // does the RNG exist?
  if (simulator::generators.find(id) == simulator::generators.end()) {
    // if not create and seed new RNG
    simulator::create_rng(id);
  }
  // uniformly select value from interval [0, 1)
  boost::uniform_01<boost::mt19937> distribution(simulator::generators[id]);
  double random_value = distribution();
  if (age  < simulator::forgetting_probablities.size()) {
    return random_value < forgetting_probablities[age];
  } else {
    return random_value < simulator::forgetting_probablity(age);
  }
}

inline double simulator::forgetting_probablity(const double& age)
{
  // implementation of the forgetting dunction as defined by Chaintreau, Fraignaud and Lebhard
  if (age < 3) {
    return 0.0;
  } else {
    return 1.0 - ((age-1)/age) * std::pow((std::log(age-1)/std::log(age)), 1+EPSILON);
  }
}

inline std::size_t simulator::ring_distance(const std::size_t n, const std::size_t a, const std::size_t b)
{
  std::size_t diff =  a < b ? b-a : a-b;
  return std::min(diff, n-diff);
}

std::size_t simulator::hop_distance(const std::vector<simulator::node>& nodes, std::size_t a, const std::size_t& b)
{
  // implements a local greedy algorithm using the 'ring_distance' function as metric
  std::size_t hops = 0;
  std::size_t ld, rd, lrd;
  std::size_t n = nodes.size();
  while (a != b) {
    ld = ring_distance(n, nodes[a].left_neighbor, b);
    rd = ring_distance(n, nodes[a].right_neighbor, b);
    lrd = ring_distance(n, nodes[a].long_range_link, b);
    if (ld <= rd && ld <= lrd) {
      a = nodes[a].left_neighbor;
    } else if (rd <= lrd) {
      a = nodes[a].right_neighbor;
    } else {
      a = nodes[a].long_range_link;
    }
    ++hops;
  }
  return hops;
}

std::vector< std::pair< double, double > > simulator::calculate_avg_hop_improvement(const std::vector< simulator::node >& nodes)
{
  std::size_t n = nodes.size();
  std::vector< std::pair<double, double> > result(n/2+1);
  std::vector< std::vector<std::size_t> > hops(n/2+1);
  // calculate hops distances
  for (std::size_t a = 0; a < n; ++a) {
    for (std::size_t b = 0; b < n; ++b) {
      if (a == b) continue; // skip self loops
      std::size_t dist = ring_distance(n, a, b);
      // get improvement assuming a local greedy algorithm
      std::size_t ld, rd, lrd, improvement;
      ld = ring_distance(n, nodes[a].left_neighbor, b);
      rd = ring_distance(n, nodes[a].right_neighbor, b);
      lrd = ring_distance(n, nodes[a].long_range_link, b);
      if ((ld <= rd && ld <= lrd) || (rd <= ld && rd <= lrd)) {
	improvement = 1;
      } else {
	improvement = dist - lrd;
      }
      hops[dist].push_back(improvement);
    }
  }
  // calculate mean
  for (std::size_t i = 1; i <= n/2; ++i) {
    // start with 1, since we do not consider self loops
    std::size_t count = 0;
    result[i].first = 0.0; // use first value for mean
    for (std::size_t j = 0; j < hops[i].size(); ++j) {
      result[i].first += hops[i][j];
      ++count;
    }
    result[i].first /= count;
  }
  // calculate sd
  for (std::size_t i = 1; i <= n/2; ++i) {
    // start with 1, since we do not consider self loops
    std::size_t count = 0;
    result[i].second = 0.0;
    for (std::size_t j = 0; j < hops[i].size(); ++j) {
      result[i].second += (result[i].first - hops[i][j])*(result[i].first - hops[i][j]);
      ++count;
    }
    result[i].second /= (count-1);
    result[i].second = std::sqrt(result[i].second);
  }
  // return result vector
  return result;
}


std::vector< size_t > simulator::calculate_length_distribution(const std::vector< simulator::node >& nodes)
{
  std::size_t n = nodes.size();
  std::vector< std::size_t > result(n/2, 0);
  for (std::size_t i = 0; i < n; ++i) {
    std::size_t distance = ring_distance(n, nodes[i].id, nodes[i].long_range_link);
    ++result[distance];
  }
  return result;
}


std::vector< std::size_t > simulator::calculate_full_hop_distance_distribution(const std::vector< simulator::node >& nodes)
{
  // calculate distribution of hop distances (for all pairs)
  std::size_t n = nodes.size();
  std::vector<std::size_t> result(n/2, 0);
  std::size_t i, j, hops;
  #pragma omp parallel for shared(nodes,result,n) private(i,j,hops) schedule(dynamic) 
  for (i = 0; i < n; ++i) {
    for (j = 0; j < n; ++j) {
      if (i == j) continue; // skip self loops
      hops = hop_distance(nodes, i, j);
      #pragma omp critical
      {
		++result[0]; // use slot 0 to save overall number of samples
		++result[hops];
      }
    }
  }
  return result;
}

std::vector< std::size_t > simulator::calculate_random_hop_distance_distribution(const std::vector< simulator::node >& nodes, const std::size_t& sample_size)
{
  // calculate distribution of hop distances (for all pairs)
  std::size_t n = nodes.size();
  std::vector<std::size_t> result(n/2, 0);
  std::vector<std::size_t>* random_ids;
  std::size_t i, j, k, t, hops, id;
  shuffle_generator* generator;
  #pragma omp parallel for shared(nodes,result,n) private(i,j,k,t,id,hops,random_ids,generator) schedule(dynamic) 
  for (i = 0; i < n; ++i) {
    #ifdef _OPENMP
      id = omp_get_thread_num();
    #endif
    // randomize vector
    // does the RNG exist?
    if (simulator::generators.find(id) == simulator::generators.end()) {
      // if not create and seed new RNG
      simulator::create_rng(id);
    }
    generator = new shuffle_generator(generators[id]);
    random_ids = new std::vector<std::size_t>(n);
    for (k = 0; k < n; ++k) {
      (*random_ids)[k] = k;
    }
    std::random_shuffle(random_ids->begin(), random_ids->end(), *generator);
    // calculate hop distances for sample of fixed size
    for (j = 0; j < sample_size; ++j) {
      // get uniformly selected target
      t = (*random_ids)[j];
      if (t == i) continue;
      hops = hop_distance(nodes, i, t);
      #pragma omp critical
      {
		++result[0]; // use slot 0 to save overall number of samples
		++result[hops];
      }
    }
    // free dyn. allocated structures
    delete generator;
    delete random_ids;
  }
  return result;
}

std::pair< double, double > simulator::calculate_mean_sd(const std::vector< std::size_t >& distribution)
{
  std::size_t n = distribution.size();
  double samples = distribution[0]; // slot 0 should store the overall sample size
  // calculate mean
  double mean = 0.0;
  for (std::size_t i = 1; i < n; ++i) {
    mean += i*distribution[i];
  }
  mean /= samples;
  // calculate sd
  double sd = 0.0;
  for (std::size_t i = 1; i < n; ++i) {
    sd += distribution[i]*((mean-i)*(mean-i));
  }
  sd /= (samples-1);
  sd = std::sqrt(sd);
  // return mean and sd
  return std::pair<double, double>(mean, sd);
}

void simulator::evaluate_network(const std::vector< simulator::node >& nodes, const std::size_t& round)
{
  std::ofstream out;
  // save snapshot
  out.open((simulator::path + "_snapshot.dat").c_str(), std::ios::out | std::ios::app);
  std::cout << "Saving simulation snapshot." << std::endl;
  out << round << "\t";
  for (std::size_t i = 0; i < nodes.size(); ++i) {
    out << nodes[i].long_range_link << "\t" << nodes[i].long_range_link_age << "\t";
  }
  out << "\n";
  out.close();
  // save avg. hop distances (meand and sd)  
  out.open((simulator::path + "_hop_distances.dat").c_str(), std::ios::out | std::ios::app);
  std::cout << "Calculating random (100 samples per node) hop distances." << std::endl;
  std::pair<double, double> hop_result = simulator::calculate_mean_sd(simulator::calculate_random_hop_distance_distribution(nodes, 100));
  out << round << "\t" << hop_result.first << "\t" << hop_result.second << "\n";
  out.close();
  // hop distance improvements
  //   out.open((simulator::path + "_single_hop_improvements.dat").c_str(), std::ios::out | std::ios::app);
  //   std::cout << "Calculating single hop improvements." << std::endl;
  //   std::vector< std::pair<double, double> > improv_result = simulator::calculate_avg_hop_improvement(nodes);
  //   out << round << "\n";
  //   for (std::size_t i = 1; i < improv_result.size(); ++i) {
  //     // start with 1, since we do not consider self loops
  //     out << i << "\t" << improv_result[i].first << "\t" << improv_result[i].second << "\n";
  //   }
  //   out << "\n";
  //   out.close();
  // length distribution
  out.open((simulator::path + "_lengths_distribution.dat").c_str(), std::ios::out | std::ios::app);
  std::cout << "Calculating long range link length distribution." << std::endl;
  std::vector< std::size_t > length_result = simulator::calculate_length_distribution(nodes);
  out << round << "\n";
  for (std::size_t i = 0; i < length_result.size(); ++i) {
    out << i << "\t" << length_result[i] << "\n";
  }
  out << "\n";
  out.close();
}


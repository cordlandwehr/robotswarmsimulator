#include <vector>
#include <map>
#include <string>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>

#ifdef _OPENMP
  // the complier support OpenMP, include header for OpenMP utility functions
  #include <omp.h>
#endif

#define PROB_CACHE_SIZE 16384

namespace simulator {
// global parameters
std::size_t n_nodes;
std::string path;
std::size_t seed;
  
// random generator
std::map<std::size_t, boost::mt19937> generators; // Mersenne Twister
#ifdef _OPENMP
  omp_lock_t generator_mutex;
#endif

// random shuffler for std::shuffle
struct shuffle_generator : std::unary_function<unsigned, unsigned> {
    boost::mt19937 &_rng;
    unsigned operator()(unsigned i) {
        boost::uniform_int<> distribution(0, i - 1);
        return distribution(_rng);
    }
    shuffle_generator(boost::mt19937 &rng) : _rng(rng) {}
};

// cache for small rpobabilities
std::vector<double> forgetting_probablities(PROB_CACHE_SIZE);
  
// small struct to represent nodes
struct node {
  std::size_t id;
  std::size_t left_neighbor;
  std::size_t right_neighbor;
  std::size_t long_range_link;
  std::size_t long_range_link_age;
};

// function to create new ring
void create_rng(std::size_t id);
void create_ring(std::vector<node>& nodes);
void random_walk(std::vector<node>& nodes, const std::size_t& steps = 1);
inline bool flip_coin(const std::size_t& id = 0);
inline bool forget_link(const std::size_t& age, const std::size_t& id = 0);
inline double forgetting_probablity(const double& age);
inline std::size_t ring_distance(const std::size_t n, const std::size_t a, const std::size_t b);
std::size_t hop_distance(const std::vector<node>& nodes, std::size_t a, const std::size_t& b);
std::vector<std::size_t> calculate_full_hop_distance_distribution(const std::vector<node>& nodes);
std::vector<std::size_t> calculate_random_hop_distance_distribution(const std::vector<node>& nodes, const std::size_t& sample_size);
std::vector<std::size_t> calculate_length_distribution(const std::vector<node>& nodes);
std::vector< std::pair<double, double> > calculate_avg_hop_improvement(const std::vector<node>& nodes);
std::pair<double, double> calculate_mean_sd(const std::vector<std::size_t>& distribution);
void evaluate_network(const std::vector<node>& nodes, const std::size_t& round);

}


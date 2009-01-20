#include <cmath>
#include <limits>
#include <sstream>
#include <algorithm>


#include "numset_stats.h"

NumSetStats::NumSetStats() {
	cfg_ = DEFAULT;
}

NumSetStats::NumSetStats(int cfg) {
	cfg_ = cfg;
}

NumSetStats::~NumSetStats() {
}

const int NumSetStats::cfg() const {
	return cfg_;
}

void NumSetStats::set_cfg(int cfg) {
	cfg_ = cfg;
}

void NumSetStats::init() {
	min_ = std::numeric_limits<double>::max();
	max_ = std::numeric_limits<double>::min();
	avg_ = 0.0;
	sum_ = 0.0;
	abssum_ = 0.0;
	stddeviation_ = 0.0;
}

void NumSetStats::handle(const std::vector<double> & data) {
	// reset any previous values
	init();

	// parse the array and update the values
	int n = data.size();

	for(int i=0; i<n; i++) {
		double val = data[i];
		// update min_
		if (val < min_)
			min_ = val;
		// update max_
		if (val > max_)
			max_ = val;
		// update sum_
		sum_ += val;
		// update abssum_
		abssum_ += std::abs(val);
		// update stddeviation helper-variable
		stddeviation_ += (val*val);
	}

	// calculate diam
	diam_ = max_ - min_;

	// caluclate avg
	avg_ = sum_ / n;

	// calculate standard-deviation
	stddeviation_ = sqrt(stddeviation_/n - (sum_*sum_)/(n*n));

	// if median should be calculated, then sort vector.
	if (cfg_ & MEDIAN) {
		std::vector<double> dataCpy = std::vector<double>(data);
		std::sort(dataCpy.begin(), dataCpy.end());
		median_ = dataCpy[(n-1)/2]; // 1,2=>0 | 3,4=>1 | ...
	}
}

const double NumSetStats::min() const {
	return min_;
}

const double NumSetStats::max() const {
	return max_;
}

const double NumSetStats::diam() const {
	return diam_;
}

const double NumSetStats::avg() const {
	return avg_;
}

const double NumSetStats::sum() const {
	return sum_;
}

const double NumSetStats::abssum() const {
	return abssum_;
}

const double NumSetStats::median() const {
	return median_;
}

const double NumSetStats::stddeviation() const {
	return stddeviation_;
}

const std::string NumSetStats::to_string() const {
	if (cfg_==0)
		return "";

	std::ostringstream o;
	if (cfg_ & MIN)
		o << "min:" << min_ << ", ";
	if (cfg_ & MAX)
		o << "max:" << max_ << ", ";
	if (cfg_ & DIAM)
		o << "diam:" << diam_ << ", ";
	if (cfg_ & AVG)
		o << "avg:" << avg_ << ", ";
	if (cfg_ & MEDIAN)
		o << "median:" << median_ << ", ";
	if (cfg_ & SUM)
		o << "sum:" << sum_ << ", ";
	if (cfg_ & ABSSUM)
		o << "abssum:" << abssum_ << ", ";
	if (cfg_ & STDDEVIATION)
		o << "stddeviation:" << stddeviation_ << ", ";

	return o.str().substr(0, o.str().size()-2);
}

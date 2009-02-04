#include "vecset_stats.h"

VecSetStats::VecSetStats() {
	cfg_ = DEFAULT;
}

VecSetStats::VecSetStats(int cfg) {
	cfg_ = cfg;
}

VecSetStats::~VecSetStats() {
}

const int VecSetStats::cfg() const {
	return cfg_;
}

void VecSetStats::set_cfg(int cfg) {
	cfg_ = cfg;
}

void VecSetStats::init() {
	cumullen_ = 0;
	sum_.clear();
	avg_.clear();
	shortestlen_ = std::numeric_limits<double>::max();
	longestlen_ = -1;
}

void VecSetStats::handle(const std::vector< boost::shared_ptr<Vector3d> >& data) {
	// reset any previous values
	init();

	// initialize temporary vars
	double veclen;

	// parse the array and update the values
	int n = data.size();

	for(int i=0; i<n; i++) {
		boost::shared_ptr<Vector3d> vec = data[i];
		veclen = vec_len(*vec);

		if (cfg_ & (SUM||SUMNORM||SUMLEN))
			// update sum_
			sum_ += *vec;

		// update shortest_
		if ((cfg_ & (SHORTEST||SHORTEST_LEN)) && (veclen < shortestlen_)) {
			shortestlen_ = veclen;
			shortest_ = *vec;
		}

		// update longest_
		if ((cfg_ & (LONGEST||LONGEST_LEN)) && (veclen > longestlen_)) {
			longestlen_ = veclen;
			longest_ = *vec;
		}

		// update cumullen_
		cumullen_ += veclen;
	}

	if (cfg_ & (SUMNORM||SUMLEN)) {
		// update sumlen_
		sumlen_ = vec_len(sum_);

		// update sumnorm_
		if (cfg_ & SUMNORM) {
			sumnorm_ = sum_;
			sumnorm_[0] /= sumlen_;
			sumnorm_[1] /= sumlen_;
			sumnorm_[2] /= sumlen_;
		}
	}

	if (cfg_ & (AVG||AVGNORM||AVGLEN)) {
		// update avg_
		avg_ = sum_;
		avg_[0] /= n;
		avg_[1] /= n;
		avg_[2] /= n;

		// update avglen_
		avglen_ = vec_len(avg_);

		// update avgnorm_
		if (cfg_ & AVGNORM) {
			avgnorm_ = avg_;
			avgnorm_[0] /= avglen_;
			avgnorm_[1] /= avglen_;
			avgnorm_[2] /= avglen_;
		}
	}
}

void VecSetStats::handle(const std::vector< boost::shared_ptr<Vector3d> >& data, int cfg) {
	set_cfg(cfg);
	handle(data);
}

const Vector3d & VecSetStats::sum() const {
	return sum_;
}

const Vector3d & VecSetStats::sumnorm() const {
	return sumnorm_;
}

const double VecSetStats::sumlen() const {
	return sumlen_;
}

const Vector3d & VecSetStats::avg() const {
	return avg_;
}

const Vector3d & VecSetStats::avgnorm() const {
	return avgnorm_;
}

const double VecSetStats::avglen() const {
	return avglen_;
}

const Vector3d & VecSetStats::shortest() const {
	return shortest_;
}

const double VecSetStats::shortestlen() const {
	return shortestlen_;
}

const Vector3d & VecSetStats::longest() const {
	return longest_;
}

const double VecSetStats::longestlen() const {
	return longestlen_;
}

const double VecSetStats::cumullen() const {
	return cumullen_;
}

const double VecSetStats::vec_len(const Vector3d & vec) const {
	return sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]);
}

const std::string VecSetStats::to_string(const Vector3d & vec) const {
	std::ostringstream o;
	o << "(" << vec[0] << "," << vec[1] << "," << vec[2] << ")";
	return o.str();
}

const std::string VecSetStats::to_string() const {
	if (cfg_==0)
		return "";

	std::ostringstream o;
	if (cfg_ & SUM)
		o << "sum:" << to_string(sum_) << ", ";
	if (cfg_ & SUMLEN)
			o << "sumlen:" << sumlen_ << ", ";
	if (cfg_ & SUMNORM)
		o << "sumnorm:" << to_string(sumnorm_) << ", ";
	if (cfg_ & AVG)
		o << "avg:" << to_string(avg_) << ", ";
	if (cfg_ & AVGLEN)
		o << "avglen:" << avglen_ << ", ";
	if (cfg_ & AVGNORM)
		o << "avgnorm:" << to_string(avgnorm_) << ", ";
	if (cfg_ & SHORTEST)
		o << "shortest:" << to_string(shortest_) << ", ";
	if (cfg_ & SHORTEST_LEN)
		o << "shortestlen:" << shortestlen_ << ", ";
	if (cfg_ & LONGEST)
		o << "longest:" << to_string(longest_) << ", ";
	if (cfg_ & LONGEST_LEN)
		o << "longestlen:" << longestlen_ << ", ";
	if (cfg_ & CUMUL_LEN)
			o << "cumullen:" << cumullen_ << ", ";

	return o.str().substr(0, o.str().size()-2);
}

void VecSetStats::push_values(std::vector<double> & data) const {
	if (cfg_ & SUM) {
		data.push_back(sum_[kXCoord]);
		data.push_back(sum_[kYCoord]);
		data.push_back(sum_[kZCoord]);
	}

	if (cfg_ & SUMLEN)
		data.push_back(sumlen_);

	if (cfg_ & SUMNORM) {
		data.push_back(sumnorm_[kXCoord]);
		data.push_back(sumnorm_[kYCoord]);
		data.push_back(sumnorm_[kZCoord]);
	}

	if (cfg_ & AVG) {
		data.push_back(avg_[kXCoord]);
		data.push_back(avg_[kYCoord]);
		data.push_back(avg_[kZCoord]);
	}

	if (cfg_ & AVGLEN)
		data.push_back(avglen_);

	if (cfg_ & AVGNORM) {
		data.push_back(avgnorm_[kXCoord]);
		data.push_back(avgnorm_[kYCoord]);
		data.push_back(avgnorm_[kZCoord]);
	}

	if (cfg_ & SHORTEST) {
		data.push_back(shortest_[kXCoord]);
		data.push_back(shortest_[kYCoord]);
		data.push_back(shortest_[kZCoord]);
	}

	if (cfg_ & SHORTEST_LEN)
		data.push_back(shortestlen_);

	if (cfg_ & LONGEST) {
		data.push_back(longest_[kXCoord]);
		data.push_back(longest_[kYCoord]);
		data.push_back(longest_[kZCoord]);
	}

	if (cfg_ & LONGEST_LEN)
		data.push_back(longestlen_);

	if (cfg_ & CUMUL_LEN)
		data.push_back(cumullen_);
}


void VecSetStats::push_names(std::vector<std::string> & designation, std::string praefix) const {
	if (cfg_ & SUM) {
		designation.push_back(praefix+"_sum_x");
		designation.push_back(praefix+"_sum_y");
		designation.push_back(praefix+"_sum_z");
	}

	if (cfg_ & SUMLEN)
		designation.push_back(praefix+"_sum_len");

	if (cfg_ & SUMNORM) {
		designation.push_back(praefix+"_sumnorm_x");
		designation.push_back(praefix+"_sumnorm_y");
		designation.push_back(praefix+"_sumnorm_z");
	}

	if (cfg_ & AVG) {
		designation.push_back(praefix+"_avg_x");
		designation.push_back(praefix+"_avg_y");
		designation.push_back(praefix+"_avg_z");
	}

	if (cfg_ & AVGLEN)
		designation.push_back(praefix+"_avg_len");

	if (cfg_ & AVGNORM) {
		designation.push_back(praefix+"_avgnorm_x");
		designation.push_back(praefix+"_avgnorm_y");
		designation.push_back(praefix+"_avgnorm_z");
	}

	if (cfg_ & SHORTEST) {
		designation.push_back(praefix+"_shortest_x");
		designation.push_back(praefix+"_shortest_y");
		designation.push_back(praefix+"_shortest_z");
	}

	if (cfg_ & SHORTEST_LEN)
		designation.push_back(praefix+"_shortest_len");

	if (cfg_ & LONGEST) {
		designation.push_back(praefix+"_longest_x");
		designation.push_back(praefix+"_longest_y");
		designation.push_back(praefix+"_longest_z");
	}

	if (cfg_ & LONGEST_LEN)
		designation.push_back(praefix+"_longest_len");

	if (cfg_ & CUMUL_LEN)
		designation.push_back(praefix+"_cumul_len");
}

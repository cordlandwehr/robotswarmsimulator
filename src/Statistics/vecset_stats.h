/**
 * \class	VecSetStats
 * \author	Sven Kurras
 * \brief	creates information on a set of Vector3d
 *
 * 1. create an instance of this class
 * 2. configure it by a ||-combination of the various bitflags that are
 *    given as static ints in the class. Pass the configuration to the
 *    constructor or to set_cfg(...)-function.
 * 3. call handle(...) on a vector of Vector3d to analyse. The currently
 *    configured information will now become (re)calculated.
 * 4. read out the information by the respective getter-function or get
 *    an overview of all configured information by the to_string()-function.
 * 5. continue with any of the points above.
 */

#ifndef VECSET_STATS_H_
#define VECSET_STATS_H_

#include <boost/smart_ptr.hpp>
#include "../Utilities/vector3d.h"

class VecSetStats {

public:

	/**
	 * flag for sum-vector
	 */
	static const int SUM          = 0x01;

	/**
	 * flag for normalized sum-vector
	 */
	static const int SUMNORM      = 0x02;

	/**
	 * flag for length of sum-vector
	 */
	static const int SUMLEN       = 0x04;

	/**
	 * flag for average-vector
	 */
	static const int AVG          = 0x08;

	/**
	 * flag for normalized average-vector
	 */
	static const int AVGNORM      = 0x10;

	/**
	 * flag for length of average-vector
	 */
	static const int AVGLEN       = 0x20;

	/**
	 * flag for shortest vector
	 */
	static const int SHORTEST     = 0x40;

	/**
	 * flag for shortest length
	 */
	static const int SHORTEST_LEN = 0x80;

	/**
     * flag for longest vector
	 */
	static const int LONGEST      = 0x100;

	/**
	 * flag for longest length
	 */
	static const int LONGEST_LEN  = 0x200;

	/**
	 * flag for cumulated length of all vectors
	 */
	static const int CUMUL_LEN    = 0x400;

	/**
	 * combined flag of default parameters
	 */
	static const int DEFAULT      = SUM|AVG|SHORTEST|LONGEST;

	/**
	 * combined flag of all parameters
	 */
	static const int ALL          = 0xFFFFFFFF;

	/**
	 * default constructor
	 */
	explicit VecSetStats();
	virtual ~VecSetStats();

	/**
	 * constructor with initial configuration
	 * \param the cfg-flag-combination to use
	 */
	explicit VecSetStats(int cfg);

	/**
	 * Calculates all information on all the Vector3D
	 * in this std::vector that is requested by the currently
	 * set flags. Access the individual information
	 * via the respective getter-function, or all combined as
	 * a string via the to_string()-function.
	 * \param data the vectorial data to analyse
	 */
	void handle(const std::vector< boost::shared_ptr<Vector3d> >& data);

	/**
	 * \return current configuration-flags
	 */
	const int cfg() const;

	/**
	 * \param cfg new configuration-flags to use
	 */
	void set_cfg(int cfg);

	/**
	 * \return constant reference to latest calculated sum-vector,
	 * unspecified if not calculated
	 */
	const Vector3d & sum() const;

	/**
	 * \return constant reference to latest calculated normalized sum-vector,
	 * unspecified if not calculated
	 */
	const Vector3d & sumnorm() const;

	/**
	 * \return latest calculated sum-vector's length,
	 * unspecified if not calculated
	 */
	const double sumlen() const;

	/**
	 * \return constant reference to latest calculated average-vector,
	 * unspecified if not calculated
	 */
	const Vector3d & avg() const;

	/**
	 * \return constant reference to latest calculated normalized average-vector,
	 * unspecified if not calculated
	 */
	const Vector3d & avgnorm() const;

	/**
	 * \return latest calculated average-vector's length,
	 * unspecified if not calculated
	 */
	const double avglen() const;

	/**
	 * \return constant reference to latest calculated shortest vector,
	 * unspecified if not calculated
	 */
	const Vector3d & shortest() const;

	/**
	 * \return latest calculated shortest vector's length,
	 * unspecified if not calculated
	 */
	const double shortestlen() const;

	/**
	 * \return constant reference to latest calculated longest vector,
	 * unspecified if not calculated
	 */
	const Vector3d & longest() const;

	/**
	 * \return latest calculated longest vector's length,
	 * unspecified if not calculated
	 */
	const double longestlen() const;

	/**
	 * \return latest calculated cumulated length over all vectors,
	 * unspecified if not calculated
	 */
	const double cumullen() const;

	/**
	 * \return string-representation of all
	 * latest calculated values with flags set in cfg,
	 * unspecified if nothing calculated
	 */
	const std::string to_string() const;

private:
	/**
	 * the current configuration of the calculation
	 */
	int cfg_;

	/**
     * the values to calculate
	 */
	Vector3d sum_, sumnorm_, avg_, avgnorm_, shortest_, longest_;
	double sumlen_, avglen_, shortestlen_, longestlen_, cumullen_;

	/**
	 * performs required initializations before calculation
	 */
	void init();

	/**
	 * helper-function that calculates the euclidean length
	 * of the given vector
	 * \param vec the vector for which to get the length
	 * \return euclidean length of vec
	 */
	const double vec_len(const Vector3d & vec) const;

	/**
	 * helper-function that constructs a string-representation of a Vector3d
	 * \param vec the vector for which to get the coordinates as string
	 * \return string-representation of vec
	 */
	const std::string to_string(const Vector3d & vec) const;
};

#endif /* VECSET_STATS_H_ */

/**
 * \class	NumSetStats
 * \author	Sven Kurras
 * \brief	creates information on a numerical set
 *
 * 1. create an instance of this class
 * 2. configure it by a ||-combination of the various bitflags that are
 *    given as static ints in the class. Pass the configuration to the
 *    constructor or to set_cfg(...)-function.
 * 3. call handle(...) on a vector of the numbers to analyse. The currently
 *    configured information will now become (re)calculated.
 * 4. read out the information by the respective getter-function or get
 *    an overview of all configured information by the to_string()-function.
 * 5. continue with any of the points above.
 */

#ifndef NUMSET_STATS_H_
#define NUMSET_STATS_H_

#include <string>
#include <vector>

class NumSetStats {

public:

	/**
	 * flag for minimum
	 */
	static const int MIN	      = 0x01;

	/**
	 * flag for maximum
	 */
	static const int MAX          = 0x02;

	/**
	 * flag for maximum
	 */
	static const int DIAM         = 0x04;

	/**
	 * flag for average
	 */
	static const int AVG          = 0x08;

	/**
	 * flag for calculating median
	 */
	static const int MEDIAN       = 0x10;

	/**
	 * flag for calculating the sum
	 */
	static const int SUM          = 0x20;

	/**
	 * flag for calculating the sum of absolute values
	 */
	static const int ABSSUM       = 0x40;

	/**
	 * flag for calculating the standard deviation
	 */
	static const int STDDEVIATION = 0x80;

	/**
	 * combined flag of default parameters
	 */
	static const int DEFAULT      = MIN|MAX|DIAM|AVG|STDDEVIATION;

	/**
	 * combined flag of all parameters
	 */
	static const int ALL          = 0xFFFFFFFF;

	/**
	 * default constructor
	 */
	explicit NumSetStats();
	virtual ~NumSetStats();

	/**
	 * constructor with initial configuration
	 * \param the cfg-flag-combination to use
	 */
	explicit NumSetStats(int cfg);

	/**
	 * \return current configuration-flags
	 */
	const int cfg() const;

	/**
	 * \param cfg new configuration-flags to use
	 */
	void set_cfg(int cfg);

	/**
	 * Calculates all information on the numerical data
	 * in this vector that is requested by the currently
	 * set flags. Access the individual information
	 * via the respective getter-function, or all combined as
	 * a string via the to_string()-function.
	 * \param data the numerical data to analyse
	 */
	void handle(const std::vector<double> & data);

	/**
	 * sets cfg as configuration and calls handle(data)
	 */
	void handle(const std::vector<double> & data, int cfg);

	/**
	 * \return latest calculated minimum-value,
	 * unspecified if not calculated
	 */
	const double min() const;

	/**
	 * \return latest calculated maximum-value,
	 * unspecified if not calculated
	 */
	const double max() const;

	/**
	 * \return latest calculated diameter-value (max-min)
	 * unspecified if not calculated
	 */
	const double diam() const;

	/**
	 * \return latest calculated average-value,
	 * unspecified if not calculated
	 */
	const double avg() const;

	/**
	 * \return latest calculated overall-sum,
	 * unspecified if not calculated
	 */
	const double sum() const;

	/**
	 * \return latest calculated overall-sum of absolute values,
	 * unspecified if not calculated
	 */
	const double abssum() const;

	/**
	 * \return latest calculated median value,
	 * unspecified if not calculated
	 */
	const double median() const;

	/**
	 * \return latest calculated standard-deviation,
	 * unspecified if not calculated
	 */
	const double stddeviation() const;

	/**
	 * \return string-representation of all
	 * latest calculated values with flags set in cfg,
	 * unspecified if nothing calculated
	 */
	const std::string to_string() const;

	/**
	 * \return appends the requested values
	 */
	void push_values(std::vector<double> & data) const;

	/**
	 * \return appends the requested value-names
	 */
	void push_names(std::vector<std::string> & designation, std::string praefix) const;

private:
	/**
	 * the current configuration of the calculation
	 */
	int cfg_;

	/**
     * the values to calculate
	 */
	double min_, max_, diam_, avg_, median_, sum_, abssum_, stddeviation_;

	/**
	 * performs required initializations before calculation
	 */
	void init();
};

#endif /* NUMSET_STATS_H_ */

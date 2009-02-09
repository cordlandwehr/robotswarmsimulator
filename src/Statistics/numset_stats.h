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
	 * flag for calculating the minimum value
	 */
	static const int MIN	      = 0x01;

	/**
	 * flag for calculating the maximum value
	 */
	static const int MAX          = 0x02;

	/**
	 * flag for calculating the diameter (== max-min)
	 */
	static const int DIAM         = 0x04;

	/**
	 * flag for calculating the average of all values
	 */
	static const int AVG          = 0x08;

	/**
	 * flag for calculating the (lower) median of all values
	 */
	static const int MEDIAN       = 0x10;

	/**
	 * flag for calculating the (oriented) sum of all values
	 */
	static const int SUM          = 0x20;

	/**
	 * flag for calculating the absolute sum of all values
	 */
	static const int ABSSUM       = 0x40;

	/**
	 * flag for calculating the standard deviation
	 */
	static const int STDDEVIATION = 0x80;

	/**
	 * combined flags of default parameters
	 */
	static const int DEFAULT      = MIN|MAX|DIAM|AVG|STDDEVIATION;

	/**
	 * combined flags of all parameters
	 */
	static const int ALL          = 0xFFFFFFFF;

	/**
	 * default constructor that uses DEFAULT as
	 * initial configuration.
	 */
	explicit NumSetStats();

	/**
	 * constructor with given initial configuration
	 * \param the flags-combination to use for configuration
	 */
	explicit NumSetStats(int cfg);

	virtual ~NumSetStats();

	/**
	 * \return the current set configuration-flags
	 */
	const int cfg() const;

	/**
	 * \param cfg the new configuration-flags to use
	 */
	void set_cfg(int cfg);

	/**
	 * Calculates for the given vector of numbers
	 * all the information that is requested by the currently
	 * set flags.
	 * \param data the numerical data to analyze
	 */
	void handle(const std::vector<double> & data);

	/**
	 * sets cfg as configuration using set_cfg(cfg)
	 * and then calls handle(data)
	 * \param data the numerical data to analyze
	 * \param cfg the new configuration-flags to use
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
	 * \return latest calculated diameter-value (== max-min)
	 * unspecified if not calculated
	 */
	const double diam() const;

	/**
	 * \return latest calculated average-value,
	 * unspecified if not calculated
	 */
	const double avg() const;

	/**
	 * \return latest calculated (oriented) sum of all values,
	 * unspecified if not calculated
	 */
	const double sum() const;

	/**
	 * \return latest calculated absolute sum of all absolute values,
	 * unspecified if not calculated
	 */
	const double abssum() const;

	/**
	 * \return latest calculated (lower) median value,
	 * unspecified if not calculated
	 */
	const double median() const;

	/**
	 * \return latest calculated standard-deviation,
	 * unspecified if not calculated
	 */
	const double stddeviation() const;

	/**
	 * \return string-representation of all the
	 * latest calculated values whose flags are set in current cfg,
	 * unspecified if nothing was calculated or the current cfg-flags
	 * are not fitting the configuration-flags during calculation.
	 */
	const std::string to_string() const;

	/**
	 * Appends all the configured and already calculated values
	 * to the given vector.
	 *
	 */
	void push_values(std::vector<double> & data) const;

	/**
	 * Appends names for all the configured values
	 * to the given vector using the given praefix.
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

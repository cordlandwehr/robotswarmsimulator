/**
 * \class	StatsConfig
 * \author	Sven Kurras
 * \brief	holds the configuration for all statistics
 *
 * This class initializes itself from a given Parser-instance
 * and sets all configuration-parameters that are latter accessed
 * from anywhere in the statistics-module to calculate and log
 * only the information requested by the user.
 */

#ifndef STATS_CONFIG_H_
#define STATS_CONFIG_H_

#include <boost/smart_ptr.hpp>

#include "../Utilities/parser.h"

class StatsConfig {
public:

	explicit StatsConfig();
	virtual ~StatsConfig();

	/**
	 * initializes from the given parser (defines the subsets for
	 * which to perform the calculations and all the configuration
	 * for it). So any latter invocations to any other functions
	 * of StatsConfig *after* the initialization will return
	 * whether or not the respective information should
	 * be calculated or how it should be configured.
	 * \param parser the parser from which to initialize
	 */
	void init(boost::shared_ptr<Parser> parser);

	/*
	 *
	 *
	 * Subset-configuration following...
	 *
	 *
	 */

	/**
	 * \return whether or not for any subset of the robots
	 * the latter defined information should be calculated.
	 * If false, the whole statistics-calculation is deactivated.
	 */
	const bool is_any_subset() const;

	/**
	 * \return whether or not to use all robots as a subswarm
	 * for which to create the latter defined information.
	 */
	const bool is_subset_all() const;

	/**
	 * \return whether or not to use all active robots as a subswarm
	 * for which to create the latter defined information.
	 */
	const bool is_subset_actall() const;

	/**
	 * \return whether or not to use all inactive robots as a subswarm
	 * for which to create the latter defined information.
	 */
	const bool is_subset_inactall() const;

	/**
	 * \return whether or not to use masters seperately as a subswarm
	 * for which to create the latter defined information.
	 */
	const bool is_subset_masters() const;

	/**
	 * \return whether or not to use masters seperately as a subswarm
	 * for which to create the latter defined information.
	 */
	const bool is_subset_actmasters() const;

	/**
	 * \return whether or not to use masters seperately as a subswarm
	 * for which to create the latter defined information.
	 */
	const bool is_subset_inactmasters() const;

	/**
	 * \return whether or not to use slaves seperately as a subswarm
	 * for which to create the latter defined information.
	 */
	const bool is_subset_slaves() const;

	/**
	 * \return whether or not to use active slaves seperately as a subswarm
	 * for which to create the latter defined information.
	 */
	const bool is_subset_actslaves() const;

	/**
	 * \return whether or not to use inactive slaves seperately as a subswarm
	 * for which to create the latter defined information.
	 */
	const bool is_subset_inactslaves() const;

	/*
	 *
	 *
	 * Calculation-parameter following...
	 *
	 *
	 */

	/**
	 * \return whether or not to log information on
	 * the total number of robots in the subswarm.
	 */
	const bool is_num_robots() const;

	/**
	 * \return whether or not to log information on
	 * the total number of masters in the subswarm.
	 */
	const bool is_num_masters() const;

	/**
	 * \return whether or not to log information on
	 * the total number of slaves in the subswarm.
	 */
	const bool is_num_slaves() const;

	/**
	 * \return whether or not to log information on
	 * the unweighted average-position of all robots
	 * in the subswarm.
	 */
	const bool is_swarm_avg_pos() const;

	/**
	 * \return configuration-flags for velocities' NumSetStats
	 */
	const int vel_cfg() const;

private:
	bool any_subset_;
	bool subset_all_, subset_actall_, subset_inactall_;
	bool subset_masters_, subset_actmasters_, subset_inactmasters_;
	bool subset_slaves_, subset_actslaves_, subset_inactslaves_;

	bool num_robots_, num_masters_, num_slaves_;
	bool swarm_avg_pos_;
	int vel_cfg_;

	/**
	 * Activates all information for calculation and logging,
	 * but does not affect the subswarm-settings.
	 */
	const void init_activate_all();

	/**
	 * Activates basic information for calculation and logging,
	 * but does not affect the subswarm-settings.
	 */
	const void init_activate_basic();

	/**
	 * Activates no information for calculation and logging,
	 * but does not affect the subswarm-settings.
	 */
	const void init_activate_none();
};

#endif /* STATS_CONFIG_H_ */

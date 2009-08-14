/**
 * \class	StatsConfig
 * \author	Sven Kurras
 * \brief	holds the configuration for all statistics
 *
 * This class initializes itself from a given parser's parametermap
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

	static const int DATADUMP_NONE = 0;
	static const int DATADUMP_FULL = 1;

	explicit StatsConfig();
	virtual ~StatsConfig();

	/**
	 * Initializes from the given parameter-map (defines the subsets for
	 * which to perform the calculations and all the configuration
	 * for it). So any latter invocations to any other functions
	 * of StatsConfig *after* the initialization will return
	 * whether or not the respective information should
	 * be calculated or how it should be configured.
	 */
	void init(std::map<std::string, std::string> &params);

	/**
	 * Returns an integer identifying the datadump-level set in projectfile
	 * by field STATS_DATADUMP.
	 * \return the respective StatsConfig.DATADUMP_*
	 */
	const int datadump_level() const;

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
	 * Calculation-parameters following...
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
	 * \return whether or not to log the center's xyz-coordinates
	 * of the miniball around all robots in the current subswarm.
	 */
	const bool is_miniball_center() const;

	/**
	 * \return whether or not to log the radius
	 * of the miniball around all robots in the current subswarm.
	 */
	const bool is_miniball_radius() const;

	/**
	 * \return whether or not to log the distance between the centerpoints
	 * of the miniballs around all robots in the current subswarm (normalized to 1 timeunit).
	 */
	const bool is_miniball_movedist() const;

	/**
	 * \return whether or not to log the quotient of robots' total volume
	 * (naively computed number*robotvolume) and the miniball volume.
	 */
	const bool is_volume_quot() const;

	/**
	 * \return configuration-flags for velocities' NumSetStats
	 */
	const int vel_cfg() const;

	/**
	 * \return whether or not to log iff the visibility graph is connected
	 */
	const bool is_visgraph_connected() const;

	/**
	 * \return whether or not to log the maximum of all robots' minimum
	 * distance to another in local view.
	 */
	const bool is_max_mindist() const;

	/**
	 * \return whether or not to log the maximum of all robots' distance
	 * to the origin.
	 */
	const bool is_max_origindist() const;


private:
	/**
	 * the datadumplevel used (one of DATADUMP_*)
	 */
	int datadump_level_;

	bool any_subset_;
	bool subset_all_, subset_actall_, subset_inactall_;
	bool subset_masters_, subset_actmasters_, subset_inactmasters_;
	bool subset_slaves_, subset_actslaves_, subset_inactslaves_;

	/*
	 * BEGIN Configuration-Block
	 *
	 * (all values in this block should be initialized
	 *  by the init_activate_* - functions)
	 */
	bool num_robots_, num_masters_, num_slaves_;
	bool swarm_avg_pos_;
	bool miniball_center_, miniball_radius_, miniball_movedist_, volume_quot_;
	int  vel_cfg_;
	bool visgraph_connected_;
	bool max_mindist_, max_origindist_;

	/*
	 * END Configuration-Block
	 */

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

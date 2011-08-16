/**
 * \class	stats_calc
 * \author	Tobias Isenberg
 * \brief	performs all the statistical calculations
 *
 * The stats_calc-class implements the pure calculation for a given worldinformation.
 * Therefore it receives from StatsControl an updated
 * struct StatsCalcInData with all information needed.
 */

// BEGIN STRUCT-DEFINTION

/**
 * StatsCalcInData
 * \author	Tobias Isenberg
 * \brief	holds information on current and last worldinformation
 *
 * This struct is filled by the StatsControl with the latest worldinformation
 * for a given point in time. Then it is sent to stats_calc as input for performing
 * all calculations. Some of them might need to access the previous worldinformation,
 * so the previous one is saved here, too. Additionally the stats_calc might perform
 * some time-consuming operations whose result shouldn't be recalculated when
 * accessing the previous worldinformation, so these results are also saved here.
 */

#ifndef STATS_CALC_H_
#define STATS_CALC_H_

#include <boost/smart_ptr.hpp>

#include "../Model/world_information.h"
#include "../SimulationKernel/simulation_listener.h"

#include "stats_config.h"
#include "stats_out.h"
//#include "statistics_data_object.h"

struct StatsCalcInData {

	/**
	 * Holds a copy of the latest WorldInformation for the simulation-time world_info_.time().
	 * It is guranteed that *all* events for exactly that time have already been processed.
	 */
	boost::shared_ptr<WorldInformation> world_info_;

	/**
	 * After performing the calculations on world_info_ it is moved to prev_world_info_ for
	 * latter additional calculations on the differences to future world_info_.
	 * At simulation's begin it will hold that "prev_world_info_.get() == NULL"
	 */
	boost::shared_ptr<WorldInformation> prev_world_info_;


	// SAVED PRECALCULATED VALUES
	// for all subsets in one vector.
	/*none so far*/
};

// END STRUCT-DEFINTION

class StatsCalc {
public:

	static const bool DEBUG = false;

	StatsCalc();
	virtual ~StatsCalc();

	/**
	 * \brief initializes from the given configuration
	 */
	void init(StatsConfig* stats_cfg);

	/**
	 * \brief performs all calculations for the given data and subset
	 */
	void calculate(const StatsCalcInData &data,
			const boost::shared_ptr<StatsOut> & stats_out);

	int calculateDegree(const std::vector<boost::shared_ptr<RobotData> >& nodes);

	int calculateMaximalDefect(const boost::shared_ptr<WorldInformation> graph);

	int calculateTotalDefects(const boost::shared_ptr<WorldInformation> graph);
private:
	/**
	 * pointer to the stats_cfg stored in StatsControl
	 */
	StatsConfig* stats_cfg_;
};

#endif /* STATS_CALC_H_ */

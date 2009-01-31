#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>

#include "../../SimulationKernel/simulation_kernel.h"
#include "../../SimulationControl/history.h"
#include "../../Model/sphere.h"
#include "../../Model/sphere_identifier.h"
#include "../../Model/box.h"
#include "../../Model/box_identifier.h"
#include "../../Model/marker_information.h"
#include "../../Model/marker_identifier.h"
#include "../../Utilities/vector3d.h"

#include "../Fixtures/simple_world_fixture.h"

BOOST_AUTO_TEST_CASE(save_main_project_file_1)
{
/*	SimulationKernel* simkernel = new SimulationKernel();

	//dummy values
	simkernel->asg_ = 0;
	simkernel->compass_model_ = "NO_COMPASS";
	simkernel->event_handler_ = 42;
	simkernel->obstacle_filename_ = "src/Tests/TestData/obsti.obstacle";
	simkernel->robot_filename_ = "src/Tests/TestData/i.robot";
	simkernel->statistics_module_ = 23;

	simkernel->save_main_project_file("src/Tests/TestData/garbled_projectfile_c.swarm");
*/
}

BOOST_AUTO_TEST_CASE(save_robot_file_1)
{
/*	SimulationKernel* simkernel = new SimulationKernel();

	//dummy values
	simkernel->asg_ = 0;
	simkernel->compass_model_ = "NO_COMPASS";
	simkernel->event_handler_ = 42;
	simkernel->obstacle_filename_ = "src/Tests/TestData/obsti.obstacle";
	simkernel->robot_filename_ = "src/Tests/TestData/i.robot";
	simkernel->statistics_module_ = 23;


//	simkernel->save_robot_file();
*/
}

BOOST_AUTO_TEST_CASE(write_obstacle_1)
{
	SimulationKernel* simkernel = new SimulationKernel();

	struct SimpleWorldFixture myworld = SimpleWorldFixture();

//  TODO(mmarcus) why does this not work???
	boost::shared_ptr<SphereIdentifier> id_sphere;
	id_sphere.reset(new SphereIdentifier(0));

	// create obstacle-ids
//	id_box.reset(new BoxIdentifier(1));
//	id_marker.reset(new MarkerIdentifier(2));

	// create position for the sphere: (1.0, 1.0, 1.0)
	boost::shared_ptr<Vector3d> pos_sphere;
	pos_sphere.reset(new Vector3d());
	pos_sphere->insert_element(kXCoord,1.0);
	pos_sphere->insert_element(kYCoord,1.0);
	pos_sphere->insert_element(kZCoord,1.0);

	boost::shared_ptr<Sphere> sphere;
//	sphere.reset(new Sphere(id_sphere, pos_sphere, 2.0));

//	cout << simkernel->write_obstacle(sphere);

}
BOOST_FIXTURE_TEST_CASE(write_robot_1, SimpleWorldFixture)
{
/*	SimulationKernel* simkernel = new SimulationKernel();

	cout << "robotwriting-test-case-" << endl;
	cout << simkernel->write_robot(robot_data_a);
	cout << simkernel->write_robot(robot_data_b);
	cout << simkernel->write_robot(robot_data_c);
*/
}

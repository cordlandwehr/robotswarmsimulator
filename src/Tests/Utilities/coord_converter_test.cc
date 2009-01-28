#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

#include "../../Utilities/coord_converter.h"


/*
 *  Test: Pass a vector (1,-1,-1) for a coordinate system which only has three axes:
 *  (1.0,5.0,0.3) , (-3.0,1.0,0), (2.0,2.0,2.0)
 *  Expected Results:
 *    vector (2.0, 2.0, -1.7)
 */
BOOST_AUTO_TEST_CASE( coord_converter_smoke_test )
{
	boost::shared_ptr<Vector3d> x_axis(new Vector3d());
	x_axis->insert_element(kXCoord, 1.0);
	x_axis->insert_element(kYCoord, 5.0);
	x_axis->insert_element(kZCoord, 0.3);

	boost::shared_ptr<Vector3d> y_axis(new Vector3d());
	y_axis->insert_element(kXCoord, -3.0);
	y_axis->insert_element(kYCoord, 1.0);
	y_axis->insert_element(kZCoord, 0.0);

	boost::shared_ptr<Vector3d> z_axis(new Vector3d());
	z_axis->insert_element(kXCoord, 2.0);
	z_axis->insert_element(kYCoord, 2.0);
	z_axis->insert_element(kZCoord, 2.0);

	boost::tuple <boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,
	              boost::shared_ptr<Vector3d> > axes(x_axis, y_axis, z_axis);

	// build the origin of the coordinate system of the local robot
	boost::shared_ptr<Vector3d> origin(new Vector3d());
	origin->insert_element(kXCoord, 1.0);
	origin->insert_element(kYCoord, 1.0);
	origin->insert_element(kZCoord, 1.0);

	// build the vector to be passed
	boost::shared_ptr<Vector3d> position(new Vector3d());
	position->insert_element(kXCoord, 1.0);
	position->insert_element(kYCoord, -1.0);
	position->insert_element(kZCoord, -1.0);



	boost::shared_ptr<Vector3d> result = CoordConverter::local_to_global(*position, *origin, axes);

	BOOST_CHECK_CLOSE((*result)(kXCoord), 3.0, 0.1);
	BOOST_CHECK_CLOSE((*result)(kYCoord), 3.0, 0.1);
	BOOST_CHECK_CLOSE((*result)(kZCoord), -0.7, 0.1);

}



/*
 *  Test: Pass a vector for a coordinate system which only has two axes:
 *  Two standard ones (1,0,0) , (0,1,0) and third one set to (0,0,0)
 *  Expected Results:
 *    Projection of the vector to two dimensions
 */
BOOST_AUTO_TEST_CASE( coord_converter_projection_test )
{
	// build a coordinate axes for a robot with unit distance 2
	boost::shared_ptr<Vector3d> x_axis(new Vector3d());
	x_axis->insert_element(kXCoord, 1.0);
	x_axis->insert_element(kYCoord, 0.0);
	x_axis->insert_element(kZCoord, 0.0);

	boost::shared_ptr<Vector3d> y_axis(new Vector3d());
	y_axis->insert_element(kXCoord, 0.0);
	y_axis->insert_element(kYCoord, 1.0);
	y_axis->insert_element(kZCoord, 0.0);

	boost::shared_ptr<Vector3d> z_axis(new Vector3d());
	z_axis->insert_element(kXCoord, 0.0);
	z_axis->insert_element(kYCoord, 0.0);
	z_axis->insert_element(kZCoord, 0.0);

	boost::tuple <boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,
	              boost::shared_ptr<Vector3d> > axes(x_axis, y_axis, z_axis);

	// build the origin of the coordinate system of the local robot
	boost::shared_ptr<Vector3d> origin(new Vector3d());
	origin->insert_element(kXCoord, 1.0);
	origin->insert_element(kYCoord, 1.0);
	origin->insert_element(kZCoord, 0.0);

	// build the vector to be passed
	boost::shared_ptr<Vector3d> position(new Vector3d());
	position->insert_element(kXCoord, 3.0);
	position->insert_element(kYCoord, 5.3);
	position->insert_element(kZCoord, -3.0);

	boost::shared_ptr<Vector3d> result = CoordConverter::local_to_global(*position, *origin, axes);

	BOOST_CHECK_CLOSE((*result)(kXCoord), 4.0, 0.1);
	BOOST_CHECK_CLOSE((*result)(kYCoord), 6.3, 0.1);
	BOOST_CHECK_CLOSE((*result)(kZCoord), 0.0, 0.1);

}

/*
 *  Test: Pass a global vector to global_to_local and the result of that to local_to_global (and vice versa)
 *
 *  Expected Results:
 *    result vector identical to input vector
 */
BOOST_AUTO_TEST_CASE( coord_converter_inverse_test )
{
	// build the coordinate system
	boost::shared_ptr<Vector3d> x_axis(new Vector3d());
	x_axis->insert_element(kXCoord, 2.0);
	x_axis->insert_element(kYCoord, 0.0);
	x_axis->insert_element(kZCoord, 0.0);

	boost::shared_ptr<Vector3d> y_axis(new Vector3d());
	y_axis->insert_element(kXCoord, 0.0);
	y_axis->insert_element(kYCoord, 2.0);
	y_axis->insert_element(kZCoord, 0.0);

	boost::shared_ptr<Vector3d> z_axis(new Vector3d());
	z_axis->insert_element(kXCoord, 0.0);
	z_axis->insert_element(kYCoord, 0.0);
	z_axis->insert_element(kZCoord, 2.0);

	boost::tuple <boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,
	              boost::shared_ptr<Vector3d> > axes(x_axis, y_axis, z_axis);

	// build the origin of the coordinate system of the local robot
	boost::shared_ptr<Vector3d> origin(new Vector3d());
	origin->insert_element(kXCoord, 1.0);
	origin->insert_element(kYCoord, 1.0);
	origin->insert_element(kZCoord, 1.0);

	// build the vector to be passed
	boost::shared_ptr<Vector3d> position(new Vector3d());
	position->insert_element(kXCoord, 3.0);
	position->insert_element(kYCoord, 3.0);
	position->insert_element(kZCoord, 3.0);

	// pass to global_to_local then local_to_global
	boost::shared_ptr<Vector3d> tmp = CoordConverter::global_to_local(*position, *origin, axes);
	boost::shared_ptr<Vector3d> result = CoordConverter::local_to_global(*tmp, *origin, axes);

	BOOST_CHECK_CLOSE((*result)(kXCoord), (*position)(kXCoord), 0.1);
	BOOST_CHECK_CLOSE((*result)(kYCoord), (*position)(kYCoord), 0.1);
	BOOST_CHECK_CLOSE((*result)(kZCoord), (*position)(kZCoord), 0.1);

	// pass to local_to_global then global_to_local
	boost::shared_ptr<Vector3d> tmp_b = CoordConverter::local_to_global(*position, *origin, axes);
	boost::shared_ptr<Vector3d> result_b = CoordConverter::global_to_local(*tmp, *origin, axes);

	BOOST_CHECK_CLOSE((*result_b)(kXCoord), (*position)(kXCoord), 0.1);
	BOOST_CHECK_CLOSE((*result_b)(kYCoord), (*position)(kYCoord), 0.1);
	BOOST_CHECK_CLOSE((*result_b)(kZCoord), (*position)(kZCoord), 0.1);
}

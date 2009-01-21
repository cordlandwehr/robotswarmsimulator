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
	// build a coordinate axes for a robot with unit distance 2
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

	// build the vector to be passed
	boost::shared_ptr<Vector3d> position(new Vector3d());
	position->insert_element(kXCoord, 1.0);
	position->insert_element(kYCoord, -1.0);
	position->insert_element(kZCoord, -1.0);

	boost::shared_ptr<Vector3d> result = CoordConverter::local_to_global(*position, axes);

	BOOST_CHECK_CLOSE((*result)(kXCoord), 2.0, 0.1);
	BOOST_CHECK_CLOSE((*result)(kYCoord), 2.0, 0.1);
	BOOST_CHECK_CLOSE((*result)(kZCoord), -1.7, 0.1);

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

	// build the vector to be passed
	boost::shared_ptr<Vector3d> position(new Vector3d());
	position->insert_element(kXCoord, 3.0);
	position->insert_element(kYCoord, 5.3);
	position->insert_element(kZCoord, -3.0);

	boost::shared_ptr<Vector3d> result = CoordConverter::local_to_global(*position, axes);

	BOOST_CHECK_CLOSE((*result)(kXCoord), 3.0, 0.1);
	BOOST_CHECK_CLOSE((*result)(kYCoord), 5.3, 0.1);
	BOOST_CHECK_CLOSE((*result)(kZCoord), 0.0, 0.1);

}

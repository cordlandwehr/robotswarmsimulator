/*
 *  mubalabieeyes.h
 *  Robot Swarm Simulator
 *
 *  Created by Peter Kling on 06.04.09.
 */

//#ifndef USE_CGAL // if CGAL not available, don't include the full easter egg
//void mubalabieeyes() {
//	std::cout << "You found 'MUBALABIEEYES', but you seem not to have a working CGAL environment :-/" << std::endl;
//	std::cout << "If you want to know what 'MUBALABIEEYES' is, install/setup CGAL and come back!" << std::endl;
//}
//#else // CGAL available --> go on


#include <CGAL/Cartesian.h>
#include <CGAL/point_generators_2.h>


// The kernel we want to use. This is used as a template parameter to most types (e.g. point or line types) to define
// what kind of mathematical objects to operate on. CGAL supports numerous kernels, but we will use simple cartesian
// coordinates represented by double values (and therefor not exact; CGAL supports also exact arithmetic).
typedef CGAL::Cartesian<double>                 Cartesian_k;

// We define some shortcuts to the mathematical object types we will be using (note that they are parameterized by the
// kernel).
typedef CGAL::Point_2<Cartesian_k> Point_2;
typedef CGAL::Line_2<Cartesian_k>  Line_2;


void mubalabieeyes() {
	// greetings to you
	std::cout << "MUBALABIEEYES - Welcome to the 'Most Unfunny But At Least A Bit Informative Easter Egg You Ever Saw'" << std::endl;
	std::cout << "====================================================================================================" << std::endl << std::endl;
	
	// let us generate some random points
	std::cout << "Generating some random points in the unit square..." << std::endl;
	CGAL::Random_points_in_square_2<Point_2> generator(1.0);
	Point_2 p[6];
	for (int i=0; i<6; i++) {
		p[i] = *generator++;
		std::cout << "\tPoint " << i+1 << ": " << p[i] << std::endl;
	}
	std::cout << std::endl;
	
	// now we define three lines (the X- and Y-axes as well as the axis defined by the first two points generated above)
	std::cout << "Generating some lines... (the tripple 'a b c' defines a line by equation 'ax+by+c=0'" << std::endl;
	Line_2 x_axis = Line_2(Point_2(0., 0.), Point_2(1., 0.));
	Line_2 y_axis = Line_2(Point_2(0., 0.), Point_2(0., 1.));
	Line_2 line = Line_2(p[0], p[1]);
	std::cout << "\tX-axes: " << x_axis << std::endl;
	std::cout << "\tY-axes: " << y_axis << std::endl;
	std::cout << "\tline from first two generated points: " << line << std::endl;
	std::cout << std::endl;
	
	// let us do some intersection
	std::cout << "Some intersections... (x-axis/y-axis, x-axis/x-axis, x-axis/'line from first two generated points', y-axis/'line from...')" << std::endl;
	std::vector<CGAL::Object> intersection_results;
	intersection_results.push_back(CGAL::intersection(x_axis, y_axis));
	intersection_results.push_back(CGAL::intersection(x_axis, x_axis));
	intersection_results.push_back(CGAL::intersection(x_axis, line));
	intersection_results.push_back(CGAL::intersection(y_axis, line));
	
	// intersections may by of different types (points, lines, ...), so we have to do corresponding casts
	for (unsigned int i=0; i<intersection_results.size(); i++) {
		try {
			Point_2 point = CGAL::object_cast<Point_2>(intersection_results[i]);
			std::cout << "\tIntersection " << i+1 << " is a point: " << point << std::endl;
			continue;
		} catch (CGAL::Bad_object_cast& e) { /* no Point_2 */ }
		
		try {
			Line_2 line = CGAL::object_cast<Line_2>(intersection_results[i]);
			std::cout << "\tIntersection " << i+1 << " is a line: " << line << std::endl;
			continue;
		} catch (CGAL::Bad_object_cast& e) { /* no Line_2 */ }
		
		std::cout << "\tIntersection " << i+1 << " is of unknokwn type :-/" << std::endl;
	}
	std::cout << std::endl;
	
	// projecting points
	std::cout << "Let us project some points onto lines..." << std::endl;
	std::cout << "\tPoints above onto X-Axis:" << std::endl;
	for (unsigned int i=0; i<6; i++)
		std::cout << "\t\tPoint " << i+1 << ": " << x_axis.projection(p[i]) << std::endl;
	std::cout << "\tPoints above onto Y-Axis:" << std::endl;
	for (unsigned int i=0; i<6; i++)
		std::cout << "\t\tPoint " << i+1 << ": " << y_axis.projection(p[i]) << std::endl;
	std::cout << "\tPoints above onto line from first two generated points:" << std::endl;
	for (unsigned int i=0; i<6; i++)
		std::cout << "\t\tPoint " << i+1 << ": " << line.projection(p[i]) << std::endl;
	std::cout << std::endl;
	
	// we can also get the perpendicular from a point onto a line
	std::cout << "Perpendicular line from third point onto line from first two generated points:" << std::endl;
	std::cout << "\t" << line.perpendicular(p[2]) << std::endl;
	
	// list some more features of CGAL
	std::cout << "CGAL provides many more features that may be interesting to you:" << std::endl;
	std::cout << "\t-circles, spheres, planes, lines, .... and intersections of these" << std::endl;
	std::cout << "\t-computation and algorithms for the convex hull" << std::endl;
	std::cout << "\t-graph algorithms" << std::endl;
	std::cout << "\t..." << std::endl << std::endl;
	
	std::cout << "See http://www.cgal.org/Manual/3.4/doc_html/cgal_manual/contents.html#part_II for a more"
	<< " comprehensive list of topics" << std::endl << std::endl;
}

//#endif /* USE_CGAL */

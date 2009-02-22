/*
 * renderer_camera_test.cc
 *
 *  Created on: 21.01.2009
 *      Author: kamil
 */

#include <ctime>
#include <cstdlib>
#include <cstdio>

#include <vector>
#include <set>

#if !defined(__linux__) && !defined(__APPLE__)
#include <windows.h>
#endif
#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <GLUT/glut.h>
#else
    #include <GL/gl.h>
	#include <GL/glut.h>
#endif

#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>

#include "../../Model/world_information.h"
#include "../../Model/world_object.h"
#include "../../Model/robot_data.h"
#include "../../Model/robot.h"
#include "../../Model/obstacle.h"
#include "../../Model/box.h"
#include "../../Model/sphere.h"
#include "../../Model/robot_identifier.h"

#include "../../Utilities/vector_arithmetics.h"

#include "../../Views/octree.h"
#include "../../Visualisation/camera.h"
#include "../../Visualisation/moveable_camera.h"
#include "../../Visualisation/follow_swarm_camera.h"
#include "../../Visualisation/simulation_renderer.h"

// TODO(peter) Commented out because of duplicated _main symbol (this file's main method and the test main method)
//             We should move such 'manual' tests to a dedicated subdirectory of Tests and exclude it in the eclipse
//             project settings. For such manual tests we could either provide a makefile or add more eclipse
//             configurations.
/*
boost::shared_ptr<WorldInformation> world_info;
SimulationRenderer * sim_render;

class Request;

class MyRobot : public Robot{

public:
	MyRobot(boost::shared_ptr<RobotIdentifier> id): Robot(id) {}
	std::set<boost::shared_ptr<Request> > compute(){

		std::set<boost::shared_ptr<Request> > ret;
		return ret;
	}


};


void draw(){

	float start = std::clock();

	 sim_render->draw(1.0, world_info);

	 float end = std::clock();

	 float ticks = (end - start) / CLOCKS_PER_SEC;
}

void resize(int x, int y){

	sim_render->resize(x,y);

}

void key_proc(unsigned char key, int x, int y){

	if(key == 'e'){
		std::exit(0);
	}

	sim_render->keyboard_func(key,x,y);

}

void key_spec_proc(int key, int x, int y){
	sim_render->keyboard_func(key,x,y);

}

void mouse_proc( int x, int y){
	sim_render->mouse_func(1,1,x,y);
}


//BOOST_AUTO_TEST_CASE(octree_test)
int main(int argc, char ** argv)
{
	std::vector<boost::shared_ptr<RobotData> > robots;
	std::vector<boost::shared_ptr<WorldObject> > markers;
	std::vector<boost::shared_ptr<Obstacle> > obstacles;

	float radius = 2.0;

	// set the starting seed, so we get always the same numbers in rand
	std::srand(47);

	boost::shared_ptr<RobotData> newRobot((RobotData*)0);
	Robot * robot;
	boost::shared_ptr<RobotIdentifier> ident;
	boost::shared_ptr<Vector3d> robot_pos;
	for(int i = 0; i < 1000; i++){


		robot_pos.reset( new Vector3d());
		boost::shared_ptr<RobotIdentifier> id(new RobotIdentifier(i));
		robot = new MyRobot(id);


		boost::shared_ptr<Vector3d> robot_pos = boost::shared_ptr<Vector3d>(new Vector3d());

		robot_pos->insert_element(kXCoord,std::rand()*1000.0/RAND_MAX);
		robot_pos->insert_element(kYCoord,std::rand()*1000.0/RAND_MAX);
		robot_pos->insert_element(kZCoord,std::rand()*1000.0/RAND_MAX);


		boost::shared_ptr<RobotData> newRobot = boost::shared_ptr<RobotData>(new RobotData(id, robot_pos, *robot));

		world_info->add_robot_data(newRobot);
		robots.push_back(newRobot);


	}






	glutInit(&argc, argv);

		// This is where we say that we want a double buffer
		glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);

		glutInitWindowPosition(100,100);
		glutInitWindowSize(320,320);

		glutCreateWindow("Glut Test");
		boost::shared_ptr<Camera> camera = boost::shared_ptr<Camera>(new MoveableCamera());
		camera->set_screen_height(320);
		camera->set_screen_width(320);
		sim_render = new SimulationRenderer(camera);

		sim_render->init(320,320);

		glutDisplayFunc( draw );
		glutIdleFunc( draw );
		glutReshapeFunc( resize );


		glutKeyboardFunc( key_proc );
		glutSpecialFunc( key_spec_proc );

		glutMotionFunc( mouse_proc );

		glutMainLoop();
}
*/

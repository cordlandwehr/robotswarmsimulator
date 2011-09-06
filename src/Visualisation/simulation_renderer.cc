/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Marcus Märtens <mmarcus@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Alexander Klaas <aklaas@uni-paderborn.de>
	Copyright (C) 2009-2010  Martina Hüllmann <martinah@uni-paderborn.de>
	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>

	RobotSwarmSimulator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RobotSwarmSimulator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RobotSwarmSimulator.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * simulation_renderer.cc
 *
 *  Created on: 18.01.2009
 *      Author: kamil
 */

#include <ctime>
#include <cstdio>
#include <algorithm>


#define GL_GLEXT_PROTOTYPES 1

#include <boost/pointer_cast.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/graph_utility.hpp>


#include "../OpenGL/gl_headers.h"
#include "../OpenGL/glu_headers.h"
#include "../OpenGL/pg_glut.h"

#include "../Model/world_information.h"
#include "../Model/world_object.h"
#include "../Model/obstacle.h"
#include "../Model/box.h"
#include "../Model/sphere.h"
#include "../Model/robot_data.h"
#include "../Model/robot_identifier.h"

#include "../SimulationControl/time_point.h"

#include "camera.h"
#include "robot_renderer.h"
#include "simulation_renderer.h"
#include "cog_camera.h"



namespace {
// some simple constants

const float kObstacleColor[] = {1.0f,0.0f,0.0f,1.0f};
const float kRobotColor[] = {0.0f,1.0f,0.0f,1.0f};
const float kTextColor[] = {1.0f,1.0f,1.0f,1.0f};
const float kMarkerColor[] = {1.0f,1.0f,0.0f,1.0f};
const float kCogColor[] = {1.0f,1.0f,0.0f,1.0f};
const float kVelocityColor[] = {1.0f,0.0f,0.0f,1.0f};
const float kAccelerationColor[] = {0.0f,1.0f,0.0f,1.0f};

const float kCoordXColor[] = {1.0f,0.0f,0.0f,1.0f};
const float kCoordYColor[] = {0.0f,1.0f,0.0f,1.0f};
const float kCoordZColor[] = {0.0f,0.0f,1.0f,1.0f};
const float kCoordLineWidth = 1.0;

const int kSphereSlices = 30;
const int kSphereStacks = 30;

const int kArrowSlices = 4;
const double kArrowBase = 1.0;

const double kMinScale = 1.0;
const double kMaxScale = 100.0;
const double kFactorScale = 0.05;

const int kDefHeight = 500;
const int kDefWidth = 500;
const int kTextSpacing=15;

const float kMarkerPointSize = 2.0;

const float kRobotRadius = 0.13;

const float kMessageHeight = 0.02;
const float kMessagewidth = 0.1;
const float kMessageDepth = 0.1;

const std::string kSkyBoxTexName[] = {"resources/Textures/skybox/mountain/","resources/Textures/skybox/mars/", "resources/Textures/skybox/island/", "resources/Textures/skybox/space/","resources/Textures/skybox/work/"};
}

void SimulationRenderer::set_active_cam(unsigned int i){
	active_camera_index_ = i % cameras_.size();
}

void SimulationRenderer::set_cog_cam_pos(Vector3d & pos){
	boost::shared_ptr<CogCamera> cog_cam = boost::dynamic_pointer_cast<CogCamera>(cameras_[2]);
	cog_cam->set_init_pos(pos);
}

void SimulationRenderer::set_free_cam_para(Vector3d & pos, Vector3d & at){
	Vector3d up;
	up(0) = 0.0;
	up(1) = 1.0f;
	up(2) = 0.0f;

	cameras_[1]->position_camera(pos,at,up);
}

SimulationRenderer::SimulationRenderer()
: projection_type_(PROJ_PERSP), render_cog_(false), render_coord_system_(false),  render_local_coord_system_(false),
  render_acceleration_(false), render_velocity_(false), render_sky_box_(true) {

	robot_renderer_ = boost::shared_ptr<RobotRenderer>( new RobotRenderer(this) );


	cameras_[0] = boost::shared_ptr<Camera>( new MoveableCamera() );
	cameras_[1] = boost::shared_ptr<Camera>( new FollowSwarmCamera() );
	cameras_[2] = boost::shared_ptr<Camera>( new CogCamera() );

	active_camera_index_=0;


	text_color_[0] = kTextColor[0];
	text_color_[1] = kTextColor[1];
	text_color_[2] = kTextColor[2];
	text_color_[3] = kTextColor[3];

	obstacle_color_[0] = kObstacleColor[0];
	obstacle_color_[1] = kObstacleColor[1];
	obstacle_color_[2] = kObstacleColor[2];
	obstacle_color_[3] = kObstacleColor[3];

	robot_color_[0] = kRobotColor[0];
	robot_color_[1] = kRobotColor[1];
	robot_color_[2] = kRobotColor[2];
	robot_color_[3] = kRobotColor[3];

	robot_renderer_->set_default_color(kRobotColor[0],kRobotColor[1],kRobotColor[2],kRobotColor[3] );

	marker_color_[0] = kMarkerColor[0];
	marker_color_[1] = kMarkerColor[1];
	marker_color_[2] = kMarkerColor[2];
	marker_color_[3] = kMarkerColor[3];

#if !defined(__linux__) && !defined(__APPLE__)
	p_glWindowPos2i = ( void (APIENTRY * )(int,int) )wglGetProcAddress("glWindowPos2i");
#endif

}

void SimulationRenderer::init(){
	init(kDefWidth, kDefHeight );
}

void SimulationRenderer::init(int x, int y){

	glClearColor(0.0f,0.0f,0.0f,0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Antialiasing
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST );
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST );
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST );


	glPointSize(kMarkerPointSize);

	// Add Lighting
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float light_pos [] = {1.0f,0.0f,0.5f,0.0f};
	float light_properties [] = {0.15f, 0.15f, 0.15f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f};

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_properties);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &light_properties[4]);
	glLightfv(GL_LIGHT0, GL_SPECULAR, &light_properties[8]);

	glEnable(GL_LIGHT1);
	float light_pos2 [] = {.0f,0.25f,0.25f,0.0f};
	float light_properties2 [] = {0.1f, 0.1f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f};

	glLightfv(GL_LIGHT1, GL_POSITION, light_pos2);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_properties2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, &light_properties2[4]);
	glLightfv(GL_LIGHT1, GL_SPECULAR, &light_properties2[8]);

	// Set up for text
	glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
	glPixelStorei(GL_UNPACK_LSB_FIRST, GL_FALSE);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//Set up skybox and Robot renderer
	for(int i = 0; i < 5;i++){
		sky_box_[i].reset( new SkyBox() );
		sky_box_[i]->init( kSkyBoxTexName[i]);
	}
	actuall_skybox_ = 0;
	robot_renderer_->init();
	resize(x,y);
}

void SimulationRenderer::resize(int width, int height){

	if(height < 1)
		height = 1;

	screen_width_ = width;
	screen_height_ = height;

	use_mouse_ = true;


	// Set the viewport to be the entire window
	glViewport(0, 0, width, height);

	setup_projection();

	glLoadIdentity();

	for(unsigned int cam_index = 0; cam_index < cameras_.size(); cam_index++){
		cameras_[cam_index]->set_screen_height(height);
		cameras_[cam_index]->set_screen_width(width);
	}

}

void SimulationRenderer::setup_projection(){

	// Reset the coordinate system before modifying
	float ratio = 1.0* screen_width_ / screen_height_;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	// Set the correct perspective.
	switch ( projection_type_ ){
		case PROJ_PERSP:
				gluPerspective(90,ratio,0.1,1000);
			break;
		case PROJ_ORTHO:
				glOrtho( 0,1,0,1,0.1,1000);


			break;
	}


	glMatrixMode(GL_MODELVIEW);
}

void SimulationRenderer::draw(double extrapolate, const boost::shared_ptr<TimePoint> & time_point){
	boost::shared_ptr<WorldInformation> world_info = time_point->world_information_ptr();
	this->extrapolate_ = extrapolate;
	world_info_=world_info;

	double max_dist = 1.0;
	BOOST_FOREACH(boost::shared_ptr<RobotData> it_robot_data, world_info->robot_data()){
		double dist = boost::numeric::ublas::norm_2( it_robot_data->position() - cameras_[active_camera_index_]->position());

		if(max_dist < dist)
			max_dist = dist;
	}

	// We draw the time in the upper left corner
	char buf[100];

	std::sprintf(buf,"Time: %f   Camera: %s\n",extrapolate + world_info->time(), cameras_[active_camera_index_]->get_name().c_str()  );
	std::string time(buf);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();



	cameras_[active_camera_index_]->update(world_info->markers(), world_info->obstacles(), world_info->robot_data(),extrapolate );
	cameras_[active_camera_index_]->look_rot();

	if(render_sky_box_)
		sky_box_[actuall_skybox_]->draw();

	cameras_[active_camera_index_]->look_translate();


	// Print time
	draw_text2d(0,0,time);

	if( render_coord_system_ ){
		draw_coord_system();
	}




	// draw all obstacles
	std::vector<boost::shared_ptr<Obstacle> >::const_iterator it_obstacle;
	for(it_obstacle = world_info->obstacles().begin(); it_obstacle != world_info->obstacles().end() ;++it_obstacle){

		draw_obstacle(*it_obstacle);
	}

	// draw all markers
	std::vector<boost::shared_ptr<WorldObject> >::const_iterator it_marker;
	for(it_marker = world_info->markers().begin(); it_marker != world_info->markers().end(); ++it_marker){
		draw_marker(*it_marker);
	}

	if(render_cog_){

		draw_cog();
	}


	double robot_size = max_dist * kFactorScale < kMinScale ? 1.0 : std::min(max_dist * kFactorScale, kMaxScale);
	robot_renderer_->set_robot_size(robot_size);
	double robot_radius = robot_size * kRobotRadius;

	// draw all robots
	std::vector<boost::shared_ptr<RobotData> >::const_iterator it_robot;
	for(it_robot = world_info->robot_data().begin(); it_robot != world_info->robot_data().end(); ++it_robot){
		robot_renderer_->draw_robot( *it_robot );

		// draw messages
		glColor3f(0.5,0.5,0.5);
		Vector3d pos = (*it_robot)->position();
		pos[kYCoord] += robot_radius;
		for(int i=0; i<(*it_robot)->get_number_of_messages(); ++i) {
			pos[kYCoord] += robot_size * kMessageHeight*2.5;
			draw_box(pos, robot_size * kMessagewidth, robot_size * kMessageHeight, robot_size * kMessageDepth);
		}

		// draw edges
		std::vector<boost::shared_ptr<EdgeIdentifier> >::const_iterator it_edge;
		for(it_edge = (*it_robot)->get_edges().begin(); it_edge != (*it_robot)->get_edges().end(); ++it_edge) {
			boost::shared_ptr<Edge> edge = world_info->get_according_edge(*it_edge);
			boost::shared_ptr<Vector3d> pos1 = world_info->get_according_robot_data(edge->getRobot1()).extrapolated_position();
			boost::shared_ptr<Vector3d> pos2 = world_info->get_according_robot_data(edge->getRobot2()).extrapolated_position();

			float d = robot_radius/vector3d_distance(*pos1, *pos2);
			if(dynamic_cast<UndirectedEdge*>(edge.get()) != NULL) {
				// For undirected edges only draw a line.
				draw_line(vector3d_interpolate(*pos1, *pos2, d), vector3d_interpolate(*pos1, *pos2, 1.0-d), (*it_robot)->color());
			} else {
				// otherwise draw an arrow.
				draw_arrow(vector3d_interpolate(*pos1, *pos2, d), vector3d_interpolate(*pos1, *pos2, 1.0-d), (*it_robot)->color());
			}
		}

	}

	glFlush();

}

void SimulationRenderer::mouse_func(int button, int state, int x, int y){
	if(use_mouse_){
				cameras_[active_camera_index_]->set_button_press_mouse(x,y);
			}

}

void SimulationRenderer::mouse_motion_func( int x, int y){
	if(use_mouse_){
			cameras_[active_camera_index_]->set_view_by_mouse(x,y);
		}

}

int SimulationRenderer::font_bitmap_string(const std::string & str) {
	std::size_t	len= str.length();

	for(std::size_t i=0;i<len;i++) {
		//glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);
		PgGLUT::glutBitmapCharacter(str[i]);
	}

	return 1;
}

void SimulationRenderer::draw_line(Vector3d pos1, Vector3d pos2, int colorcode){
	glBegin(GL_LINES);
		glColor3fv(&kRobotIdColor[colorcode % kRobotIdColorNum ][0]);
		glVertex3f(pos1(0),pos1(1), pos1(2) );
		glVertex3f(pos2(0), pos2(1), pos2(2) );
	glEnd();

}

void SimulationRenderer::draw_arrow(Vector3d pos1, Vector3d pos2, int colorcode){

	Vector3d pos3 = vector3d_interpolate(pos1, pos2, 0.9);

	draw_line(pos1, pos2, colorcode);

	double x = pos3(0);
	double y = pos3(1);
	double z = pos3(2);

	double vx = pos2(0)-x;
	double vy = pos2(1)-y;
	double vz = pos2(2)-z;

	//handle the degenerate case of z1 == z2 with an approximation
	if(vz == 0)
		vz = .0001;

	double v = std::sqrt( vx*vx + vy*vy + vz*vz );
	double ax = 57.2957795*std::acos( vz/v );
	if ( vz < 0.0 )
		ax = -ax;
	double rx = -vy*vz;
	double ry = vx*vz;

	glPushMatrix();


	glTranslated( x, y, z );
	glRotated(ax, rx, ry, 0.0);

	//draw a cone
	PgGLUT::glutWireCone(kArrowBase, v, kArrowSlices, 1);

	glPopMatrix();

}



void SimulationRenderer::draw_text2d(int x, int y,  const std::string & str ) {

    GLfloat     curcolor[4], position[4];
    GLboolean   texturing;


	// save OpenGL state
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glGetFloatv(GL_CURRENT_COLOR, curcolor);
    glGetBooleanv(GL_TEXTURE_2D, &texturing);
    glGetFloatv(GL_CURRENT_RASTER_POSITION, position);

    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);

    // set up the state we need

	glDisable(GL_TEXTURE_2D);
	//glColor3f(1.0f,1.0f,1.0f);


	glColor3fv(text_color_);

#if !defined(__linux__) && !defined(__APPLE__)
	void (APIENTRY *p_glWindowPos2i)(int , int) = ( void (APIENTRY * )(int,int) )wglGetProcAddress("glWindowPos2i");
	(*p_glWindowPos2i)(x,y);
#else
    glWindowPos2i(x , y );
#endif


	font_bitmap_string(str);


    // restore OpenGL state


    glPopAttrib();
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColor4fv(curcolor);
    if(texturing == GL_TRUE) {
        glEnable(GL_TEXTURE_2D);

    }

#if !defined(__linux__) && !defined(__APPLE__)
    (*p_glWindowPos2i)(position[0],position[1]);
#else
    glWindowPos2i(position[0] ,position[1]);
#endif
}


void SimulationRenderer::draw_text3d(const Vector3d & vector, const std::string &str ){

	GLdouble proj_x = 0;
	GLdouble proj_y = 0;
	GLdouble  win_z;
	GLint * vp = NULL;
	GLdouble * pm= NULL;
	GLdouble * mm= NULL;


	glGetIntegerv(GL_VIEWPORT,vp);

	glGetDoublev(GL_MODELVIEW_MATRIX,mm);

	glGetDoublev(GL_PROJECTION_MATRIX,pm);



	if (gluProject (vector(0), vector(1), vector(2), mm, pm, vp, &proj_x, &proj_y, &win_z) == GL_FALSE)
		return;


	draw_text2d(std::floor(proj_x), std::floor(proj_y),str);

}

void SimulationRenderer::draw_obstacle(const boost::shared_ptr<Obstacle> & obstacle){

	Box * box;
	Sphere * sphere;

	glColor3f(obstacle_color_[0], obstacle_color_[1], obstacle_color_[2] );

	if( (box = dynamic_cast<Box*>(obstacle.get() ) ) ){
			draw_box(box);
	} else if ( ( sphere = dynamic_cast<Sphere*>(obstacle.get() ) ) ){
			draw_sphere(sphere);
	} else {
		//nothing to do right now..
	}


}
void SimulationRenderer::draw_box(const Box*  box){
	draw_box(box->position(), box->width(), box->height(), box->depth());
}
void SimulationRenderer::draw_box(const Vector3d& pos, float width, float height, float depth) {

	float pos_x = pos(0);
	float pos_y = pos(1);
	float pos_z = pos(2);
	float half_width = width/2;
	float half_height = height/2;
	float half_depth = depth/2;

	glBegin(GL_QUADS);	// Start Drawing The Cube

		     //TOP
			glVertex3f(pos_x+ half_width,pos_y+ half_height,pos_z-half_depth);
			glVertex3f(pos_x-half_width, pos_y+half_height,pos_z-half_depth);
			glVertex3f(pos_x-half_width,pos_y+ half_height,pos_z+ half_depth);
			glVertex3f(pos_x +half_width,pos_y+ half_height,pos_z+ half_depth);

			// Bottom
			glVertex3f(pos_x+ half_width,pos_y-half_height, pos_z+half_depth);
			glVertex3f(pos_x-half_width,pos_y-half_height,pos_z+ half_depth);
			glVertex3f(pos_x-half_width,pos_y-half_height,pos_z-half_depth);
			glVertex3f(pos_x+ half_width,pos_y-half_height,pos_z-half_depth);

			//Front
			glVertex3f(pos_x+ half_width,pos_y+ half_height,pos_z+ half_depth);
			glVertex3f(pos_x-half_width, pos_y+half_height,pos_z+ half_depth);
			glVertex3f(pos_x-half_width,pos_y-half_height,pos_z+ half_depth);
			glVertex3f(pos_x+ half_width,pos_y-half_height,pos_z+ half_depth);

			//Back
			glVertex3f(pos_x+ half_width,pos_y-half_height,pos_z-half_depth);
			glVertex3f(pos_x-half_width,pos_y-half_height,pos_z-half_depth);
			glVertex3f(pos_x-half_width, pos_y+half_height,pos_z-half_depth);
			glVertex3f(pos_x +half_width,pos_y+ half_height,pos_z-half_depth);

			//Left

			glVertex3f(pos_x-half_width, pos_y+half_height, pos_z+ half_depth);
			glVertex3f(pos_x-half_width, pos_y+half_height,pos_z- half_depth);
			glVertex3f(pos_x-half_width,pos_y-half_height,pos_z- half_depth);
			glVertex3f(pos_x-half_width,pos_y-half_height,pos_z+ half_depth);

			//Right
			glVertex3f(pos_x+half_width,pos_y+ half_height, pos_z- half_depth);
			glVertex3f(pos_x+ half_width,pos_y+ half_height,pos_z+ half_depth);
			glVertex3f(pos_x+ half_width,pos_y-half_height, pos_z+ half_depth);
			glVertex3f(pos_x+ half_width,pos_y-half_height,pos_z- half_depth);
		glEnd();


}



void SimulationRenderer::draw_sphere(const Sphere*  sphere){

	Vector3d pos = sphere->position();
	double radius = sphere->radius();

	glPushMatrix();
		glTranslatef(pos(0),pos(1),pos(2));

		PgGLUT::glutSolidSphere(radius, kSphereSlices, kSphereStacks);

	glPopMatrix();
}

inline void SimulationRenderer::draw_robot(const boost::shared_ptr<RobotData> & robot){


}

void SimulationRenderer::draw_marker(const boost::shared_ptr<WorldObject> & marker){



		glColor3f(marker_color_[0], marker_color_[1], marker_color_[2]);

		glBegin(GL_POINTS);
			glVertex3f(marker->position()(0),
					   marker->position()(1),
					   marker->position()(2) );
		glEnd();


}

void SimulationRenderer::draw_cog(){

	cog_.insert_element(kXCoord, 0);
	cog_.insert_element(kYCoord, 0);
	cog_.insert_element(kZCoord, 0);

	int num = 0;

	std::vector<boost::shared_ptr<RobotData> >::const_iterator it_robot;
	for(it_robot = world_info_->robot_data().begin(); it_robot != world_info_->robot_data().end(); ++it_robot){
		cog_ = cog_ + (*it_robot)->position();
		num++;
	}

	if(num > 0){
		cog_ = cog_ / num;
	}

	glBegin(GL_POINT);
		glColor3fv(kCogColor);
		glVertex3f( cog_(0), cog_(1), cog_(2) );
	glEnd();

}


void SimulationRenderer::draw_coord_system(){
const float length=1000;
const int step=2;
	glLineWidth(kCoordLineWidth);

	glBegin(GL_LINES);
		glColor3fv(kCoordXColor);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(length,.0f,.0f);
		for(int i=0; i < length; i+=step ){
			glVertex3f(i,-0.5,0);
			glVertex3f(i,0.5,0);
		}

		glColor3fv(kCoordYColor);
		glVertex3f(0.0f,.0f,.0f);
		glVertex3f(.0f,length,.0f);
		for(int i=0; i < length ; i+= step){
			glVertex3f(-0.5,i,0);
			glVertex3f(0.5,i,0);
		}

		glColor3fv(kCoordZColor);
		glVertex3f(.0f,.0f, .0f);
		glVertex3f(0.0f,.0f,length);
		for(int i=0; i < length; i+= step ){
			glVertex3f(0.0,-0.5f,i);
			glVertex3f(0.0,0.5,i);
		}

	glEnd();

	glLineWidth(kCoordLineWidth);

}

void SimulationRenderer::set_text_color(float r, float g ,float b, float alpha){

	text_color_[0] = r;
	text_color_[1] = g;
	text_color_[2] = b;
	text_color_[3] = alpha;

}

void SimulationRenderer::set_obstacle_color(float r, float g ,float b, float alpha){
	obstacle_color_[0] = r;
	obstacle_color_[1] = g;
	obstacle_color_[2] = b;
	obstacle_color_[3] = alpha;
}

void SimulationRenderer::set_robot_color(float r, float g ,float b, float alpha){

	robot_renderer_->set_default_color(r,g,b,alpha);

	robot_color_[0] = r;
	robot_color_[1] = g;
	robot_color_[2] = b;
	robot_color_[3] = alpha;

}

void SimulationRenderer::set_marker_color(float r, float g ,float b, float alpha){

	marker_color_[0] = r;
	marker_color_[1] = g;
	marker_color_[2] = b;
	marker_color_[3] = alpha;

}


boost::shared_ptr<Identifier> SimulationRenderer::pick_object(int x, int y) const {

	boost::shared_ptr<Identifier> id;

	GLint viewport[4];
	GLuint select_buffer[512];

	glSelectBuffer(512,select_buffer);
	glRenderMode(GL_SELECT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glGetIntegerv(GL_VIEWPORT,viewport);
	gluPickMatrix(x,viewport[3]-y, 5,5,viewport);
	gluPerspective(90, (GLfloat)viewport[2]/(GLfloat)viewport[3], 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glInitNames();
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cameras_[active_camera_index_]->look_rot();
	cameras_[active_camera_index_]->look_translate();

	// draw robot spheres
	std::vector<boost::shared_ptr<RobotData> >::const_iterator it_robot;
	for(it_robot = world_info_->robot_data().begin(); it_robot != world_info_->robot_data().end(); ++it_robot){
		glPushName((*it_robot)->id()->id());
		robot_renderer_->draw_robot( *it_robot );
		glPopName();
	}

	int hits;

	// restoring the original projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glFlush();

	// returning to normal rendering mode
	hits = glRenderMode(GL_RENDER);

	// if there are hits process them
	if (hits != 0) {
		GLuint names, *ptr, min_z, *ptr_names, number_of_names;

		ptr = (GLuint *) select_buffer;
		min_z = 0xffffffff;
		for (uint32_t i = 0; i < hits; ++i) {
			names = *ptr;
			ptr++;
			if (*ptr < min_z) {
				number_of_names = names;
				min_z = *ptr;
				ptr_names = ptr+2;
			}

			ptr += names+2;
		}

		if(number_of_names>0) {
			ptr = ptr_names;
			id.reset(new RobotIdentifier(*ptr));
		}
	}

	return id;
}


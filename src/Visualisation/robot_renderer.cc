/*
 * robot_renderer.cc
 *
 *  Created on: 08.02.2009
 *      Author: kamil
 */

#include <cmath>
#include <cstdio>

#include <boost/tuple/tuple.hpp>

#include "../OpenGL/gl_headers.h"
#include "../OpenGL/glu_headers.h"
#include "../OpenGL/glut_headers.h"

#include "../Model/robot_data.h"

#include "camera.h"
#include "simulation_renderer.h"
#include "robot_renderer.h"
#include "robot_color.h"

const float kRobotRadius = 0.15;
const int kRobotSlices = 30;
const int kRobotStacks = 30;

const float kRobotColor[] = {0.0f,1.0f,0.0f,1.0f};
const float kVelColor[] = {1.0f,0.5f,0.0f,1.0f};
const float kAccelColor[] = {0.0f,0.5f,1.0f,1.0f};

const float kCogColor[] = {1.0f,1.0f,0.0f,1.0f};

const float kCoordXColor[] = {1.0f,0.0f,0.0f,1.0f};
const float kCoordYColor[] = {0.0f,1.0f,0.0f,1.0f};
const float kCoordZColor[] = {0.0f,0.0f,1.0f,1.0f};

const float kScaleVecs = 4.0;
const float kScaleCoordVecsRev = 2.0;

RobotRenderer::RobotRenderer(SimulationRenderer * renderer ){


	renderer_ = renderer;
	use_model_ = false;
	for(int i=0; i < 4; i++ ){
		default_color_[i] = kRobotColor[i];
	}



}

RobotRenderer::~RobotRenderer(){
	//Nothing to do atm
}

void RobotRenderer::init(){
	compiled_list_ = glGenLists(1);

	glNewList( compiled_list_, GL_COMPILE );
		glutSolidSphere(kRobotRadius, kRobotSlices, kRobotStacks);

	glEndList();

}

void RobotRenderer::draw_robot(const boost::shared_ptr<RobotData> & robot, double extrapolate ) const{

	boost::shared_ptr<Vector3d> rob_pos = robot->extrapolated_position(extrapolate);



	if( renderer_->render_cog() ){

		glBegin(GL_LINES);
			glColor3fv(kCogColor);
			glVertex3f(renderer_->cog()(0), renderer_->cog()(1), renderer_->cog()(2) );
			glVertex3f((*rob_pos)(0), (*rob_pos)(1), (*rob_pos) (2) );
		glEnd();
	}


	glPushMatrix();
		glTranslatef((*rob_pos)(0), (*rob_pos)(1), (*rob_pos)(2) );

	glBegin(GL_LINES);
	if( renderer_->render_velocity() ){

		Vector3d vel = robot->velocity();
		glColor3fv(kVelColor);
		glVertex3f(0 ,0, 0 );
		glVertex3f( kScaleVecs * vel(0),
				    kScaleVecs *  vel(1),
					kScaleVecs *  vel(2) );

	}

	if( renderer_->render_acceleration() ){

		Vector3d acc = robot->velocity();
		glColor3fv(kAccelColor);
		glVertex3f(0 , 0, 0 );
		glVertex3f( kScaleVecs * acc(0),
					kScaleVecs * acc(1),
					kScaleVecs * acc(2) );
	}

	if( renderer_->render_local_coord_system() ){

		boost::tuple<boost::shared_ptr<Vector3d>,
					 boost::shared_ptr<Vector3d>,
					 boost::shared_ptr<Vector3d> > local_coord = robot->coordinate_system_axis();

		boost::shared_ptr<Vector3d> x_coord = local_coord.get<0>();
		boost::shared_ptr<Vector3d> y_coord = local_coord.get<1>();
		boost::shared_ptr<Vector3d> z_coord = local_coord.get<2>();

		float length_x_coord = Magnitude( *x_coord ) * kScaleCoordVecsRev;
		float length_y_coord = Magnitude( *y_coord ) * kScaleCoordVecsRev;
		float length_z_coord = Magnitude( *z_coord ) * kScaleCoordVecsRev;

		glColor3fv(kCoordXColor);
		glVertex3f(0, 0, 0);
		glVertex3f( (*x_coord)(0) / length_x_coord,
					(*x_coord)(1) / length_x_coord,
					(*x_coord)(2) / length_x_coord );

		glColor3fv(kCoordYColor);
		glVertex3f(0, 0,0 );
		glVertex3f((*y_coord)(0) / length_y_coord,
				   (*y_coord)(1) / length_y_coord,
				   (*y_coord)(2) / length_y_coord );

		glColor3fv(kCoordZColor);
		glVertex3f(0, 0, 0 );
		glVertex3f( (*z_coord)(0) / length_z_coord,
					(*z_coord)(1) / length_z_coord,
					(*z_coord)(2) / length_z_coord);


	}

	glEnd();



	glColor3fv(&kRobotIdColor[ robot->color() < kRobotIdColorNum ? robot->color() : 0 ][0]);
	if(!use_model_){

		draw_robot_sphere( rob_pos );
	}


	glPopMatrix();
}

void RobotRenderer::set_default_color(float r, float g, float b, float alpha){

	default_color_[0] = r;
	default_color_[1] = g;
	default_color_[2] = b;
	default_color_[3] = alpha;

}

void RobotRenderer::load_model(std::string & filename ){

	//TODO(kamil) implement this...

}



void RobotRenderer::draw_robot_sphere(const boost::shared_ptr<Vector3d> & pos) const{

		glCallList( compiled_list_ );

}



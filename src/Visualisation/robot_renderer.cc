/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Alexander Klaas <aklaas@uni-paderborn.de>
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
#include "../OpenGL/pg_glut.h"

#include "../Model/robot_data.h"
#include "../Model/identifier.h"
#include "../Model/robot_identifier.h"
#include "../Model/edge_identifier.h"
#include "../Model/message.h"
#include "../Model/message_identifier.h"
#include "../Model/edge.h"
#include "../Model/world_information.h"

#include "camera.h"
#include "simulation_renderer.h"
#include "robot_renderer.h"


namespace {

// constants for robot drawing

// values for spheres
const float kRobotRadius = 0.13;
const int kRobotSlices = 13;
const int kRobotStacks = 13;

// colors
const float kRobotColor[] = {0.0f,1.0f,0.0f,1.0f};
const float kVelColor[] = {1.0f,0.5f,0.0f,1.0f};
const float kAccelColor[] = {0.0f,0.5f,1.0f,1.0f};

const float kCogColor[] = {1.0f,1.0f,0.0f,1.0f};

const float kCoordXColor[] = {1.0f,0.0f,0.0f,1.0f};
const float kCoordYColor[] = {0.0f,1.0f,0.0f,1.0f};
const float kCoordZColor[] = {0.0f,0.0f,1.0f,1.0f};

// scale factors
const float kScaleVecs = 2.0;
const float kScaleCoordVecsRev = 2.0;

const float kMessageHeight = 0.15;
const float kMessageWidth = 0.15;
const float kMessageDepth = 0.15;
const unsigned int kDefaultMessageColor = 3;
const unsigned int kSelectionColor = 6;

}

unsigned int get_color_from_marker_information(boost::shared_ptr<WorldObject> object, unsigned int default_color=0, const std::string& color_name = "color") {
	if(object.get() && object->marker_information().has_key(color_name)) {
		boost::any color = object->marker_information().get_data(color_name);
		double * color_ptr = boost::any_cast<double>(&color);
		if(color_ptr)
			return static_cast<unsigned int>(*color_ptr) % kRobotIdColorNum;
	}
	return default_color;
}

RobotRenderer::RobotRenderer(SimulationRenderer * renderer): robot_size_(1.0){
	renderer_ = renderer;
	for (int i=0; i < 4; i++ ){
		default_color_[i] = kRobotColor[i];
	}
}

RobotRenderer::~RobotRenderer() {
	//Nothing to do atm
}

void RobotRenderer::init() {
	compiled_list_ = glGenLists(1);

	glNewList( compiled_list_, GL_COMPILE );
		PgGLUT::glutSolidSphere(kRobotRadius, kRobotSlices, kRobotStacks);
	glEndList();
}

void RobotRenderer::draw_robot(const boost::shared_ptr<RobotData> & robot ) const{
	boost::shared_ptr<Vector3d> rob_pos = robot->extrapolated_position();

	if (renderer_->render_cog()) {
		glBegin(GL_LINES);
			glColor3fv(kCogColor);
			glVertex3f(renderer_->cog()(0), renderer_->cog()(1), renderer_->cog()(2) );
			glVertex3f((*rob_pos)(0), (*rob_pos)(1), (*rob_pos) (2) );
		glEnd();
	}


	glPushMatrix();
		glTranslatef((*rob_pos)(0), (*rob_pos)(1), (*rob_pos)(2) );

	if (renderer_->render_local_coord_system()){

		glBegin(GL_LINES);

		boost::tuple<boost::shared_ptr<Vector3d>,
					 boost::shared_ptr<Vector3d>,
					 boost::shared_ptr<Vector3d> > local_coord = robot->coordinate_system_axis();

		boost::shared_ptr<Vector3d> x_coord = local_coord.get<0>();
		boost::shared_ptr<Vector3d> y_coord = local_coord.get<1>();
		boost::shared_ptr<Vector3d> z_coord = local_coord.get<2>();


		glColor3fv(kCoordXColor);
		glVertex3f(0, 0, 0);
		glVertex3f( (*x_coord)(0) ,
					(*x_coord)(1) ,
					(*x_coord)(2)  );

		glColor3fv(kCoordYColor);
		glVertex3f(0, 0,0 );
		glVertex3f((*y_coord)(0) ,
				   (*y_coord)(1) ,
				   (*y_coord)(2)  );

		glColor3fv(kCoordZColor);
		glVertex3f(0, 0, 0 );
		glVertex3f( (*z_coord)(0) ,
					(*z_coord)(1) ,
					(*z_coord)(2) );


		glEnd();

	}

	unsigned int rob_color = get_color_from_marker_information(robot, robot->color());
	glColor3fv(&kRobotIdColor[rob_color][0]);
	glPushName(SimulationRenderer::SELECTION_ROBOT);
	glPushName(robot->id()->id());
	draw_robot_sphere( rob_pos );
	glPopName();
	glPopName();

	glPopMatrix();

	double robot_radius = robot_size_ * kRobotRadius;

	// draw a selection box around the robot when selected
	boost::shared_ptr<RobotIdentifier> r_id = boost::dynamic_pointer_cast<RobotIdentifier>(renderer_->selected_object());
	if( r_id.get() && r_id->id() == robot->id()->id() ) {
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		glColor3fv(&kRobotIdColor[kSelectionColor][0]);
		renderer_->draw_box( *rob_pos, robot_radius*2, robot_radius*2, robot_radius*2);
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}

	// draw messages
	Vector3d pos;
	vector3d_set(pos, *rob_pos);
	pos[kXCoord] -= robot_radius/2;
	pos[kYCoord] -= robot_radius/2;
	pos[kZCoord] -= robot_radius/2;
	glPushName(SimulationRenderer::SELECTION_MESSAGE);
	std::map<std::size_t, boost::shared_ptr<Message> >::const_iterator it;
	for(size_t i=0; i<robot->get_number_of_messages(); ++i) {
		boost::shared_ptr<MessageIdentifier> id = robot->get_message(i);
		unsigned int msg_color = get_color_from_marker_information(renderer_->world_info()->get_according_message(id), kDefaultMessageColor);
		glColor3fv(&kRobotIdColor[msg_color][0]);

		glPushName(id->id());
		renderer_->draw_box(pos, robot_size_ * kMessageWidth, robot_size_ * kMessageHeight, robot_size_ * kMessageDepth);

		boost::shared_ptr<MessageIdentifier> m_id = boost::dynamic_pointer_cast<MessageIdentifier>(renderer_->selected_object());
		if( m_id.get() && m_id->id() == id->id() ) {
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			glColor3fv(&kRobotIdColor[kSelectionColor][0]);
			renderer_->draw_box( pos, robot_size_ * kMessageWidth * 1.01, robot_size_ * kMessageWidth * 1.01, robot_size_ * kMessageWidth * 1.01);
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		}
		pos[kYCoord] += robot_size_ * kMessageHeight * 1.1;
		glPopName();
	}
	glPopName();

	// draw edges
	glPushName(SimulationRenderer::SELECTION_EDGE);
	std::vector<boost::shared_ptr<EdgeIdentifier> >::const_iterator it_edge;
	for(it_edge = robot->get_edges().begin(); it_edge != robot->get_edges().end(); ++it_edge) {
		boost::shared_ptr<Edge> edge = renderer_->world_info()->get_according_edge(*it_edge);
		boost::shared_ptr<Vector3d> pos1 = renderer_->world_info()->get_according_robot_data(edge->robot1()).extrapolated_position();
		boost::shared_ptr<Vector3d> pos2 = renderer_->world_info()->get_according_robot_data(edge->robot2()).extrapolated_position();

		glPushName((*it_edge)->id());

		unsigned int edge_color = get_color_from_marker_information(edge, robot->color());
		float d = robot_radius/vector3d_distance(*pos1, *pos2);

		// make lines thicker when edge is selected
		boost::shared_ptr<EdgeIdentifier> e_id = boost::dynamic_pointer_cast<EdgeIdentifier>(renderer_->selected_object());
		if( e_id.get() && e_id->id() == (*it_edge)->id() ) {
			float lineWidth = 0;
			glGetFloatv(GL_LINE_WIDTH, &lineWidth);
			glDepthMask(GL_FALSE);
			glLineWidth(lineWidth*5);
			if(dynamic_cast<UndirectedEdge*>(edge.get()) != NULL) {
				renderer_->draw_line(*pos1, *pos2, kSelectionColor);
			} else {
				renderer_->draw_arrow(vector3d_interpolate(*pos1, *pos2, d), vector3d_interpolate(*pos1, *pos2, 1.0-d*0.8), kSelectionColor, kSelectionColor, robot_radius*0.9, true);
			}
			glLineWidth(lineWidth);
			glDepthMask(GL_TRUE);
		}

		if(dynamic_cast<UndirectedEdge*>(edge.get()) != NULL) {
			// For undirected edges only draw a line.
			renderer_->draw_line(vector3d_interpolate(*pos1, *pos2, d), vector3d_interpolate(*pos1, *pos2, 1.0-d), edge_color);
		} else {
			// otherwise draw an arrow.
			renderer_->draw_arrow(vector3d_interpolate(*pos1, *pos2, d), vector3d_interpolate(*pos1, *pos2, 1.0-d), edge_color, edge_color+1, robot_radius*0.75);
		}
		glPopName();
	}
	glPopName();
}

void RobotRenderer::set_default_color(float r, float g, float b, float alpha){
	default_color_[0] = r;
	default_color_[1] = g;
	default_color_[2] = b;
	default_color_[3] = alpha;
}

inline void RobotRenderer::draw_robot_sphere(const boost::shared_ptr<Vector3d> & pos) const{
	glScalef(robot_size_, robot_size_, robot_size_);
	glCallList( compiled_list_ );
}



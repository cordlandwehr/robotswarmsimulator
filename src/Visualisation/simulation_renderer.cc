/*
 * simulation_renderer.cc
 *
 *  Created on: 18.01.2009
 *      Author: kamil
 */

#include <ctime>
#include <cstdio>

#define GL_GLEXT_PROTOTYPES 1


#include <boost/graph/strong_components.hpp>
#include <boost/graph/graph_utility.hpp>

#include "../OpenGL/gl_headers.h"
#include "../OpenGL/glu_headers.h"
#include "../OpenGL/glut_headers.h"

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

const int kDefHeight = 500;
const int kDefWidth = 500;
const int kTextSpacing=15;

const float kMarkerPointSize = 2.0;


const std::string kSkyBoxTexName[] = {"resources/Textures/skybox/mountain/","resources/Textures/skybox/mars/", "resources/Textures/skybox/island/", "resources/Textures/skybox/space/"};
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
  render_acceleration_(false), render_velocity_(false), render_help_(false), render_about_(false), render_sky_box_(true) {

	render_visibility_graph_=false;

	robot_renderer_ = boost::shared_ptr<RobotRenderer>( new RobotRenderer( this ) );


	cameras_[1]=boost::shared_ptr<Camera>(new MoveableCamera());
	cameras_[0]=boost::shared_ptr<Camera>(new FollowSwarmCamera());
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
	std::string str("resources/Textures/logo.bmp");
	tex_.load(str);


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
	for(int i = 0; i < 4;i++){
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
	if (render_visibility_graph_ && world_info_!=world_info) calculate_visibility_graph(world_info);
	world_info_=world_info;


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

	if (render_about_){
			draw_about();
	}


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

	if (render_visibility_graph_){
		draw_visibility_graph();
	}

	if (render_help_){
		draw_help();
	}


	robot_renderer_->set_extrapolate( extrapolate_ );
	// draw all robots
	std::vector<boost::shared_ptr<RobotData> >::const_iterator it_robot;
	for(it_robot = world_info->robot_data().begin(); it_robot != world_info->robot_data().end(); ++it_robot){
		robot_renderer_->draw_robot( *it_robot );
	}

	glFlush();
	glutSwapBuffers();


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

void SimulationRenderer::keyboard_func(unsigned char key, int x, int y){
	switch(key){
		case 'm':
		    	use_mouse_ = !use_mouse_;
			break;
		case 'c':
				if (active_camera_index_+1 == cameras_.size())
					active_camera_index_=0;
				else active_camera_index_++;
			break;


		case 'w':
				cameras_[active_camera_index_]->move_up();
			break;

		case 's':
				cameras_[active_camera_index_]->move_down();
			break;


		case 'g':
				switch_render_cog();
			break;

		case 'v':
				switch_render_velocity();
			break;

		case 'b':
				switch_render_acceleration();
			break;

		case 'k':
				switch_render_coord_system();
			break;

		case 'h':
				render_help_=!render_help_;
			break;

		case 'l':
				switch_render_local_coord_system();
			break;
		case 't':
				actuall_skybox_ = (actuall_skybox_ +1 ) % sky_box_.size();
			break;

		case 'z':
				render_visibility_graph_=!render_visibility_graph_;
				calculate_visibility_graph(world_info_);
			break;
		default:
			break;
	}
}


void SimulationRenderer::keyboard_special_func(int key, int x, int y){
	switch(key){
		case GLUT_KEY_LEFT:
				cameras_[active_camera_index_]->strafe_left();

			break;
		case GLUT_KEY_RIGHT:
				cameras_[active_camera_index_]->strafe_right();

			break;

		case GLUT_KEY_UP:

				cameras_[active_camera_index_]->move_forward();
			break;

		case GLUT_KEY_DOWN:
				cameras_[active_camera_index_]->move_backward();
			break;

		case GLUT_KEY_F1:
				render_about_=!render_about_;
			break;

		default:

			break;
	}

}


int SimulationRenderer::font_bitmap_string(const std::string & str) {
	std::size_t	len= str.length();

	for(std::size_t i=0;i<len;i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);
	}

	return 1;
}

void SimulationRenderer::draw_visibility_graph(){
//in vis graph, ids of vertices correspond to ids of robots
	if (vis_graph_){
	boost::graph_traits< boost::adjacency_list <> >::edge_iterator i, end;
	Vector3d source_pos, target_pos;
	//iterate through the edges and draw lines
	//get color from component table so that each component has its own color
	//add 0 or 1 so that the first component is not green to indicate the graph is not connected
	 for (boost::tie(i, end) = boost::edges(*vis_graph_); i != end; ++i) {
		 source_pos=*world_info_->robot_data()[boost::source(*i,*vis_graph_)]->extrapolated_position(extrapolate_);
		 target_pos=*world_info_->robot_data()[boost::target(*i,*vis_graph_)]->extrapolated_position(extrapolate_);
		 draw_line(source_pos,target_pos, components_[boost::source(*i,*vis_graph_)]+vis_graph_is_connected_);
	 }
	}
}

void SimulationRenderer::calculate_visibility_graph(const boost::shared_ptr<WorldInformation> world_info){
	std::vector<boost::shared_ptr<RobotIdentifier> > visible_robots;
	std::vector<boost::shared_ptr<RobotData> >::const_iterator it_robot;
//init the object variables
	if (!(vis_graph_)) vis_graph_=boost::shared_ptr< boost::adjacency_list <> >(new boost::adjacency_list<>(world_info->robot_data().size()));
	 if (components_.size()==0) components_.resize(world_info->robot_data().size());

//reset the graph each time or else old edges would be retained
	 (*vis_graph_).clear();

//go through all the robots and their visible neighbors, create edges in graph
	for(it_robot = world_info->robot_data().begin(); it_robot != world_info->robot_data().end(); ++it_robot){
		boost::shared_ptr<const View> view=(*it_robot)->view().lock();

		if (view){
				 visible_robots=view->get_visible_robots((*it_robot)->robot());

				 BOOST_FOREACH(boost::shared_ptr<RobotIdentifier> cur_id, visible_robots) {
					 boost::add_edge((*it_robot)->id()->id(), cur_id->id(), *vis_graph_);
				 }
		}
	}
//calculate connected components
	 int number_connected_components=boost::strong_components((*vis_graph_),&components_[0]);

	 if (number_connected_components==1){
		std::cout<<"visibility graph IS connected"<<std::endl;
	 }
	 else{
		 //single unconnected robots may not get added to the graph - offset accounts for that
		 int offset=world_info->robot_data().size()-boost::num_vertices(*vis_graph_);
		std::cout<<"visibility graph is NOT connected ("<<number_connected_components+offset<<" components)"<<std::endl;
	 }
//set 0 if graph is connected, 1 if it is not
	 vis_graph_is_connected_= (number_connected_components==1 ? 0 : 1);
}

void SimulationRenderer::draw_line(Vector3d pos1, Vector3d pos2, int colorcode){
	glBegin(GL_LINES);
				glColor3fv(&kRobotIdColor[colorcode % kRobotIdColorNum ][0]);
				glVertex3f(pos1(0),pos1(1), pos1(2) );
				glVertex3f(pos2(0), pos2(1), pos2(2) );
			glEnd();


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

	float pos_x = box->position()(0);
	float pos_y = box->position()(1);
	float pos_z = box->position()(2);

	float width = box->width()/2;
	float height = box->height()/2;
	float depth = box->depth()/2;


	glBegin(GL_QUADS);	// Start Drawing The Cube

		     //TOP
			glVertex3f(pos_x+ width,pos_y+ height,pos_z-depth);
			glVertex3f(pos_x-width, pos_y+height,pos_z-depth);
			glVertex3f(pos_x-width,pos_y+ height,pos_z+ depth);
			glVertex3f(pos_x +width,pos_y+ height,pos_z+ depth);

			// Bottom
			glVertex3f(pos_x+ width,pos_y-height, pos_z+depth);
			glVertex3f(pos_x-width,pos_y-height,pos_z+ depth);
			glVertex3f(pos_x-width,pos_y-height,pos_z-depth);
			glVertex3f(pos_x+ width,pos_y-height,pos_z-depth);

			//Front
			glVertex3f(pos_x+ width,pos_y+ height,pos_z+ depth);
			glVertex3f(pos_x-width, pos_y+height,pos_z+ depth);
			glVertex3f(pos_x-width,pos_y-height,pos_z+ depth);
			glVertex3f(pos_x+ width,pos_y-height,pos_z+ depth);

			//Back
			glVertex3f(pos_x+ width,pos_y-height,pos_z-depth);
			glVertex3f(pos_x-width,pos_y-height,pos_z-depth);
			glVertex3f(pos_x-width, pos_y+height,pos_z-depth);
			glVertex3f(pos_x +width,pos_y+ height,pos_z-depth);

			//Left

			glVertex3f(pos_x-width, pos_y+height, pos_z+ depth);
			glVertex3f(pos_x-width, pos_y+height,pos_z- depth);
			glVertex3f(pos_x-width,pos_y-height,pos_z- depth);
			glVertex3f(pos_x-width,pos_y-height,pos_z+ depth);

			//Right
			glVertex3f(pos_x+width,pos_y+ height, pos_z- depth);
			glVertex3f(pos_x+ width,pos_y+ height,pos_z+ depth);
			glVertex3f(pos_x+ width,pos_y-height, pos_z+ depth);
			glVertex3f(pos_x+ width,pos_y-height,pos_z- depth);
		glEnd();


}



void SimulationRenderer::draw_sphere(const Sphere*  sphere){

	Vector3d pos = sphere->position();
	double radius = sphere->radius();

	glPushMatrix();
		glTranslatef(pos(0),pos(1),pos(2));

		glutSolidSphere(radius, kSphereSlices, kSphereStacks);

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

void SimulationRenderer::draw_help(){
boost::array<std::string, 15> helptext;
	helptext[0]="Arrow keys to navigate";
	helptext[1]="W and S to go up and down";
	helptext[2]="M to toggle mouse control (rotate view) (Works only in the free camera)";
	helptext[3]="      Press mouse button and move mouse to rotate view.";
	helptext[4]="SPACE to pause the Simulator";
	helptext[5]="Q to exit";
	helptext[6]="H to toggle this help screen";
	helptext[7]="-, +, / and * to de-/increase speed";
	helptext[8]="C to change camera";
	helptext[9]="K/L to display global/local coordinate axes";
	helptext[10]="B and V to display robot's acceleration and velocity";
	helptext[11]="G to display center of gravity";
	helptext[12]="Z to display visibility graph";
	helptext[13]="F1 to display About screen";
	helptext[14] = "t to switch skybox";

	for (unsigned int i=0;i<helptext.size();i++){
		draw_text2d(10,screen_height_-50-i*kTextSpacing,helptext[i]);
	}

}

void SimulationRenderer::draw_about(){
	//render logo as textured quad
	if( tex_.loaded() ){
		glDisable(GL_LIGHTING);

		glEnable(GL_TEXTURE_2D);
		tex_.bind();
		glPushMatrix();
		glLoadIdentity();
		glBegin(GL_QUADS);
			   glColor3f(1,1,1);
			   glTexCoord2f(0, 1); glVertex3f(-1, 0.44f,-1.1f);
			   glTexCoord2f(1, 1); glVertex3f(1, 0.44f,-1.1f);
			   glTexCoord2f(1, 0); glVertex3f(1, -0.44f,-1.1f);
			   glTexCoord2f(0, 0); glVertex3f(-1,-0.44f,-1.1f);
		glEnd();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D );

		glEnable( GL_LIGHTING );
	}
	//render text as bitmap font
	boost::array<std::string,6> abouttext;

	abouttext[0]="This RobotSwarmSimulator was developed as part of the university";
	abouttext[1]="project group \"Schlaue Schwaerme\" by Alexander Klaas, Andreas Cord-Landwehr,";
	abouttext[2]="Christoph Raupach, Christoph Weddemann, Daniel Warner, Daniel Wonisch,";
	abouttext[3]="Kamil Swierkot, Marcus Maertens, Martina Huellmann, Peter Kling and Sven Kurras.";
	abouttext[4]="The University of Paderborn, Research group \"Algorithms and Complexity\"";
	abouttext[5]="Contact: der-schwarm@lists.uni-paderborn.de";
	for (unsigned int i=0;i<abouttext.size();i++){
		draw_text2d(10,screen_height_-50-i*kTextSpacing,abouttext[i]);
	}
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




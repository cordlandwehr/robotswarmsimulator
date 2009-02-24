/*
 * simulation_renderer.cc
 *
 *  Created on: 18.01.2009
 *      Author: kamil
 */

#include <ctime>
#include <cstdio>

#define GL_GLEXT_PROTOTYPES 1

#include "../OpenGL/gl_headers.h"
#include "../OpenGL/glu_headers.h"
#include "../OpenGL/glut_headers.h"

#include "../Model/world_information.h"
#include "../Model/world_object.h"
#include "../Model/obstacle.h"
#include "../Model/box.h"
#include "../Model/sphere.h"
#include "../Model/robot_data.h"



#include "camera.h"
#include "robot_renderer.h"
#include "simulation_renderer.h"


// some simple constants

const float kObstacleColor[] = {1.0f,0.0f,0.0f,1.0f};
const float kRobotColor[] = {0.0f,1.0f,0.0f,1.0f};
const float kTextColor[] = {1.0f,0.0f,1.0f,1.0f};
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


const std::string kSkyBoxTexName("data/tex/skybox/");



SimulationRenderer::SimulationRenderer() :
									render_cog_(false), render_coord_system_(true), render_acceleration_(false),
									render_velocity_(true), render_local_coord_system_(true), projection_type_(PROJ_PERSP) {



	robot_renderer_ = boost::shared_ptr<RobotRenderer>( new RobotRenderer( this ) );


	cameras_[0]=boost::shared_ptr<Camera>(new MoveableCamera());
	cameras_[1]=boost::shared_ptr<Camera>(new FollowSwarmCamera());

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

	sky_box_.reset( new SkyBox() );

	sky_box_->init( kSkyBoxTexName);
	robot_renderer_->init();

	//std::string str("data/tex/skybox/back.bmp");
	//tex_.load(str);

	resize(x,y);
}

void SimulationRenderer::resize(int width, int height){

	if(height < 1)
		height = 1;

	screen_width_ = width;
	screen_height_ = height;

	use_mouse_ = false;


	// Set the viewport to be the entire window
	glViewport(0, 0, width, height);

	setup_projection();

	glLoadIdentity();

	cameras_[active_camera_index_]->set_screen_height(height);
	cameras_[active_camera_index_]->set_screen_width(width);

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



	//cameras_[active_camera_index_]->look();


	glMatrixMode(GL_MODELVIEW);
}

void SimulationRenderer::draw(double extrapolate, const boost::shared_ptr<WorldInformation> &world_info){
	this->extrapolate_ = extrapolate;
	float start_time = std::clock();

	// We draw the time in the upper left corner
	char buf[50];
	std::sprintf(buf,"Time: %f\n",extrapolate + world_info->time() );
	std::string time(buf);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();



	cameras_[active_camera_index_]->update(world_info->markers(), world_info->obstacles(), world_info->robot_data(),extrapolate );
	cameras_[active_camera_index_]->look_rot();
	//sky_box_->draw();
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

		draw_cog(world_info);
	}

	if (render_help_){
		draw_help();
	}
	if (render_about_){
		draw_about();
	}
	// draw all robots
	std::vector<boost::shared_ptr<RobotData> >::const_iterator it_robot;
	for(it_robot = world_info->robot_data().begin(); it_robot != world_info->robot_data().end(); ++it_robot){
		draw_robot(*it_robot);
	}


	glFlush();
	glutSwapBuffers();

	float end_time = std::clock();
	float ticks = (end_time - start_time) / CLOCKS_PER_SEC;
	std::printf("Time for rendering: %f \n", ticks);
}

void SimulationRenderer::mouse_func(int button, int state, int x, int y){
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
				if (active_camera_index_==cameras_.size()-1)
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
	GLfloat pix;
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

void SimulationRenderer::draw_robot(const boost::shared_ptr<RobotData> & robot){
	robot_renderer_->draw_robot( robot, extrapolate_ );

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
boost::array<std::string, 12> helptext;
	helptext[0]="Arrow keys to navigate";
	helptext[1]="W and S to go up and down";
	helptext[2]="M to toggle mouse control (rotate view)";
	helptext[3]="SPACE to pause the Simulator";
	helptext[4]="Q to exit";
	helptext[5]="H to toggle this help screen";
	helptext[6]="-, +, / and * to de-/increase speed";
	helptext[7]="C to change camera";
	helptext[8]="K to display global coordinate axes";
	helptext[9]="B and V to display robot's acceleration and velocity";
	helptext[10]="G to display center of gravity";
	helptext[11]="F1 to display About screen";

	for (int i=0;i<helptext.size();i++){
		draw_text2d(10,screen_height_-50-i*kTextSpacing,helptext[i]);
	}

}

void SimulationRenderer::draw_about(){
	boost::array<std::string,6> abouttext;

	abouttext[0]="This RobotSwarmSimulator was developed as part of the university";
	abouttext[1]="project group \"Schlaue Schwaerme\" by Alexander Klaas, Andreas Cord-Landwehr,";
	abouttext[2]="Christoph Raupach, Christoph Weddemann, Daniel Warner, Daniel Wonisch,";
	abouttext[3]="Kamil Swierkot, Marcus Märtens, Martina Hüllmann, Peter Kling and Sven Kurras.";
	abouttext[4]="The University of Paderborn, Research group \"Algorithms and Complexity\"";
	abouttext[5]="Contact: der-schwarm@lists.uni-paderborn.de";
	for (int i=0;i<abouttext.size();i++){
		draw_text2d(10,screen_height_-50-i*kTextSpacing,abouttext[i]);
	}
}

void SimulationRenderer::draw_cog(const boost::shared_ptr<WorldInformation> world_info ){

	cog_.insert_element(kXCoord, 0);
	cog_.insert_element(kYCoord, 0);
	cog_.insert_element(kZCoord, 0);

	int num = 0;

	std::vector<boost::shared_ptr<RobotData> >::const_iterator it_robot;
	for(it_robot = world_info->robot_data().begin(); it_robot != world_info->robot_data().end(); ++it_robot){
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

	glLineWidth(kCoordLineWidth);

	glBegin(GL_LINES);
		glColor3fv(kCoordXColor);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(1.0f,.0f,.0f);

		glColor3fv(kCoordYColor);
		glVertex3f(0.0f,.0f,.0f);
		glVertex3f(.0f,1.0f,.0f);

		glColor3fv(kCoordZColor);
		glVertex3f(.0f,.0f, .0f);
		glVertex3f(0.0f,.0f,1.0f);

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




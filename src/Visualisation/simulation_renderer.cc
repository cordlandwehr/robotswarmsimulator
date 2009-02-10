/*
 * simulation_renderer.cc
 *
 *  Created on: 18.01.2009
 *      Author: kamil
 */

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
#include "font.h"
#include "robot_renderer.h"
#include "simulation_renderer.h"

// some simple constants

const float kObstacleColor[] = {1.0f,0.0f,0.0f,1.0f};
const float kRobotColor[] = {0.0f,1.0f,0.0f,1.0f};
const float kTextColor[] = {0.0f,0.0f,1.0f,1.0f};
const float kMarkerColor[] = {1.0f,1.0f,0.0f,1.0f};

const int kSphereSlices = 30;
const int kSphereStacks = 30;

const int kDefHeight = 300;
const int kDefWidth = 300;


const float kMarkerPointSize = 2.0;



SimulationRenderer::SimulationRenderer(boost::shared_ptr<Camera> & camera) : camera_(camera) {

	robot_renderer_ = boost::shared_ptr<RobotRenderer>( new RobotRenderer( this ) );


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


}

void SimulationRenderer::init(){

	init(kDefWidth, kDefHeight );

}

void SimulationRenderer::init(int x, int y){

	glClearColor(0.0f,0.0f,0.0f,0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


	glPointSize(kMarkerPointSize);


	resize(x,y);
}

void SimulationRenderer::resize(int width, int height){

	if(height < 1)
		height = 1;

	screen_width_ = width;
	screen_height_ = height;


	float ratio = 1.0* width / height;
	use_mouse_ = false;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, width, height);

	// Set the correct perspective.
	gluPerspective(60,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	camera_->set_screen_height(height);
	camera_->set_screen_width(width);

	camera_->look();


}

void SimulationRenderer::draw(double extrapolate, boost::shared_ptr<WorldInformation> world_info){
	this->extrapolate_ = extrapolate;

	// We draw the time in the upper left corner
	char buf[50];
	std::sprintf(buf,"Time: %f\n",extrapolate);
	std::string time(buf);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	camera_->update(world_info->markers(), world_info->obstacles(), world_info->robot_data() );
	camera_->look();

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

	// draw all robots
	std::vector<boost::shared_ptr<RobotData> >::const_iterator it_robot;
	for(it_robot = world_info->robot_data().begin(); it_robot != world_info->robot_data().end(); ++it_robot){
		draw_robot(*it_robot);
	}

	// Print time
	draw_text2d(2,2,time);

	glutSwapBuffers();
	glutPostRedisplay();
}

void SimulationRenderer::mouse_func(int button, int state, int x, int y){


	if(use_mouse_){
		camera_->set_view_by_mouse(x,y);
	}

}


void SimulationRenderer::keyboard_func(unsigned char key, int x, int y){

	switch(key){
		case GLUT_KEY_LEFT:
				camera_->strafe_left();

			break;
		case GLUT_KEY_RIGHT:
				camera_->strafe_right();

			break;

		case GLUT_KEY_UP:

				camera_->move_forward();
			break;

		case GLUT_KEY_DOWN:
				camera_->move_backward();
			break;

		case 'm':
				use_mouse_ = !use_mouse_;
			break;
		default:

			break;
	}

}


int SimulationRenderer::font_bitmap_string(const std::string & str) {

	std::size_t	len= str.length();

	GLint swap_bytes, lsb_first, row_length;
	GLint skip_rows, skip_pixels, alignment;


	glGetIntegerv(GL_UNPACK_SWAP_BYTES, &swap_bytes);
	glGetIntegerv(GL_UNPACK_LSB_FIRST, &lsb_first);
	glGetIntegerv(GL_UNPACK_ROW_LENGTH, &row_length);
	glGetIntegerv(GL_UNPACK_SKIP_ROWS, &skip_rows);
	glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &skip_pixels);
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
	glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
	glPixelStorei(GL_UNPACK_LSB_FIRST, GL_FALSE);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for(std::size_t i=0;i<len;i++) {

		char ch = str[i];

		if(ch==0)	/* end of string */
			break;

		if(ch < kFONTFIRST)
			continue;

		if(ch > kFONTLAST)
			continue;

		ch -= kFONTFIRST;

		glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, &bitmap_font[ch][1]);

	}



	glPixelStorei(GL_UNPACK_SWAP_BYTES, swap_bytes);
	glPixelStorei(GL_UNPACK_LSB_FIRST, lsb_first);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, row_length);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, skip_rows);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, skip_pixels);
	glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

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


    // set up the state we need

	glDisable(GL_TEXTURE_2D);
	//glColor3f(1.0f,1.0f,1.0f);

    glRasterPos2i(x- screen_width_, y - screen_height_);
	glColor4fv(text_color_);


	font_bitmap_string(str);


    // restore OpenGL state

    glPopAttrib();
    glColor4fv(curcolor);
    if(texturing == GL_TRUE) {
        glEnable(GL_TEXTURE_2D);

    }
    glRasterPos2i(x ,y);

}


void SimulationRenderer::draw_text3d(const Vector3d & vector, const std::string &str ){

	int proj_x = 0;
	int proj_y = 0;
	//TODO: Calculate Projection of Point vector onto the screen

	// Is Projection visible?
	if(proj_x < 0 || proj_y < 0 ||
			proj_y > camera_->screen_height() || proj_x > camera_->screen_width() ){

		return;
	}

	draw_text2d(proj_x, proj_y,str);

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

void SimulationRenderer::set_camera(boost::shared_ptr<Camera> & new_camera){
		camera_.reset((Camera*) 0);
		camera_ = new_camera;

		camera_->set_screen_height(screen_height_);
		camera_->set_screen_width(screen_width_);

	}


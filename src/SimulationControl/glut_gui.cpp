/*
 * glut_gui.cpp
 *
 *  Created on: 20.01.2009
 *      Author: Alexander
 */
#include "glut_gui.h"

GlutGUI::GlutGUI() {
	// TODO Auto-generated constructor stub

}

GlutGUI::~GlutGUI() {
	// TODO Auto-generated destructor stub
}

void display() {
	//use singleton pattern?
	SimulationControl::getInstance()->process_simulation();
	glClear( GL_COLOR_BUFFER_BIT ); /* Clear the screen with the clear color */
	//glutSwapBuffers(); /* Double buffering */
}

void GlutGUI::initGUI(int argc, char **argv){
  	    /* Glut setup function calls */
		glutInit( &argc, argv );
		glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
		glutInitWindowPosition( 100, 100 );
		glutInitWindowSize( 640, 480 );
		glutCreateWindow( argv[0] );
		glutDisplayFunc( display );
		glutMainLoop();
}

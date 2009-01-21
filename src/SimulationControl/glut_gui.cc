/*
 * glut_gui.cc
 *
 *  Created on: 20.01.2009
 *      Author: Alexander
 */

// TODO(peter) Has windows an own preprocessor variable in common compilers? Something like '__windows__'?
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


#include "glut_gui.h"

GlutGUI::GlutGUI() {
//	SimulationControl::getInstance()->create_new_simulation("foobar");
//	SimulationControl::getInstance()->start_simulation();
}

GlutGUI::~GlutGUI() {
	// TODO Auto-generated destructor stub
}

void display() {
	//use singleton pattern?
//	SimulationControl::getInstance()->process_simulation();
//	glClear( GL_COLOR_BUFFER_BIT ); /* Clear the screen with the clear color */
	//glutSwapBuffers(); /* Double buffering */
}

void GlutGUI::initGUI(int argc, char **argv){
  	    /* Glut setup function calls */
//		glutInit( &argc, argv );
//		glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
//		glutInitWindowPosition( 100, 100 );
//		glutInitWindowSize( 640, 480 );
//		glutCreateWindow( argv[0] );
//		glutDisplayFunc( display );
//		glutMainLoop();
}

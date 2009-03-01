/*
 * gl_headers.h
 *
 *  Created on: 21.01.2009
 *      Author: peter
 */

#ifndef GLHEADERS_H_
#define GLHEADERS_H_

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN 1
	#include <windows.h>
#endif
#ifdef __APPLE__
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
#endif

#endif /* GLHEADERS_H_ */

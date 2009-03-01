/*
 * glu_headers.h
 *
 *  Created on: 21.01.2009
 *      Author: peter
 */

#ifndef GLUHEADERS_H_
#define GLUHEADERS_H_

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN 1
	#include <windows.h>
#endif
#ifdef __APPLE__
    #include <OpenGL/glu.h>
#else
    #include <GL/glu.h>
#endif

#endif /* GLUHEADERS_H_ */

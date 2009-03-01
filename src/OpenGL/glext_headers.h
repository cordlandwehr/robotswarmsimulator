/*
 * glext_headers.h
 *
 *  Created on: 28.02.2009
 *      Author: kamil
 *
 *  NOTE:
 *  The original glext.h can be found
 *  		http://www.opengl.org/registry/api/glext.h
 *
 */

#ifndef GLEXT_HEADERS_H_
#define GLEXT_HEADERS_H_

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN 1
	#include <windows.h>
#endif
#ifdef __APPLE__
    #include <OpenGL/glext.h>
#else
    #include <GL/glext.h>
#endif

#endif /* GLEXT_HEADERS_H_ */

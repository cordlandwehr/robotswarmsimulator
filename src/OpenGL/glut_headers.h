/*
 * glut_headers.h
 *
 *  Created on: 21.01.2009
 *      Author: peter
 */

#ifndef GLUTHEADERS_H_
#define GLUTHEADERS_H_

#ifdef WIN32

	#include <windows.h>
#endif
#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#endif /* GLUTHEADERS_H_ */

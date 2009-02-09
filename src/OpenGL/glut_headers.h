/*
 * glut_headers.h
 *
 *  Created on: 21.01.2009
 *      Author: peter
 */

#ifndef GLUTHEADERS_H_
#define GLUTHEADERS_H_

// TODO(peter) Does windows have an own preprocessor variable in common compilers? Something like '__windows__'?
// TODO(peter) Does windows really need this 'windows.h' include?! (just copied it from an older commit of Alexander)
// TODO(peter) Under MacOSX one normally needs <GLUT/glut.h>. Linux and windows should be <GL/glut.h>? I'm not sure...
#if !defined(__linux__) && !defined(__APPLE__)
#include <windows.h>
#endif
#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#endif /* GLUTHEADERS_H_ */

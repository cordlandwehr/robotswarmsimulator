/*
 * GLHeaders.h
 *
 *  Created on: 21.01.2009
 *      Author: peter
 */

#ifndef GLHEADERS_H_
#define GLHEADERS_H_

// TODO(peter) Does windows have an own preprocessor variable in common compilers? Something like '__windows__'?
// TODO(peter) Does windows really need this 'windows.h' include?! (just copied it from an older commit of Alexander)
// TODO(peter) Under MacOSX one normally needs <OpenGL/gl.h>. Linux and windows should be <GL/gl.h>? I'm not sure...
#if !defined(__linux__) && !defined(__APPLE__)
#include <windows.h>
#endif
#ifdef __APPLE__
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
#endif

#endif /* GLHEADERS_H_ */

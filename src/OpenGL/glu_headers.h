/*
 * glu_headers.h
 *
 *  Created on: 21.01.2009
 *      Author: peter
 */

#ifndef GLUHEADERS_H_
#define GLUHEADERS_H_

// TODO(peter) Does windows have an own preprocessor variable in common compilers? Something like '__windows__'?
// TODO(peter) Does windows really need this 'windows.h' include?! (just copied it from an older commit of Alexander)
// TODO(peter) Under MacOSX one normally needs <OpenGL/glu.h>. Linux and windows should be <GL/glu.h>? I'm not sure...
#if !defined(__linux__) && !defined(__APPLE__)
#include <windows.h>
#endif
#ifdef __APPLE__
    #include <OpenGL/glu.h>
#else
    #include <GL/glu.h>
#endif

#endif /* GLUHEADERS_H_ */

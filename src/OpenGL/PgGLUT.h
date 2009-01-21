/*
 * PgGLUT.h
 *
 *  Created on: 21.01.2009
 *      Author: peter
 */

#ifndef PGGLUT_H_
#define PGGLUT_H_


#include <string>

#include <boost/function.hpp>

#include "GLHeaders.h";
#include "GLUTHeaders.h";


/**
 * \brief C++ wrapper for the OpenGL Toolkit (GLUT) Interface.
 *
 * For the documentation of the 'glut*Func(...)' callbacks, see the GLUT documentation. Note that this wrapper supports
 * class member methods as callbacks.
 *
 * Currently supported callbacks:
 *  - ...
 *
 * \see http://www.opengl.org/resources/libraries/glut/spec3/spec3.html
 */
// didn't use formatting from our codeguideline for interface methods to stay compatible with original GLUT interface
namespace PgGLUT {
	// some intern methods/data
	namespace {
		bool initialized = false;

		boost::function<void ()> display_callback_;
		boost::function<void (int width, int height)> reshape_callback_;
		boost::function<void (unsigned char key, int x, int y)> keyboard_callback_;
		boost::function<void (int button, int state, int x, int y)> mouse_callback_;

		// delegates to be registered as GLUT callbacks
		void display_callback_delegate() { display_callback_(); }
		void keyboard_callback_delegate(unsigned char key, int x, int y) { keyboard_callback_(key, x, y); }
		void mouse_callback_delegate(int button, int state, int x, int y) { mouse_callback_(button, state, x, y); }
		void reshape_callback_delegate(int width, int height) { reshape_callback_(width, height); }
	}

	/**
	 * \brief Initializes the GLUT OpenGL context.
	 *
	 * Note that calling this method more than once has no effect.
	 * \see http://www.opengl.org/resources/libraries/glut/spec3/spec3.html
	 */
	void init(const std::string& window_name, int& argc, char** argv) {
		if (!initialized) {
			::glutInit(&argc, argv);
			::glutCreateWindow(window_name.c_str());
			initialized = true;
		}
	}

	/**
	 * \brief Starts the GLUT main loop.
	 *
	 * Checks wether GLUT was initialized using PgGLUT::init. If so, the main loop is entered. If not, a warning is
	 * issued and the method returns.
	 */
	void glutMainLoop() {
		if (!initialized) {
			std::cerr << "Warning: cannot enter GLUT main loop (GLUT not initialized)" << std::endl;
			return;
		}
		::glutMainLoop();
	}

	/**
	 * \brief Wrapper around GLUT method (supports class member methods).
	 * \see http://www.opengl.org/resources/libraries/glut/spec3/spec3.html
	 */
	void glutDisplayFunc(boost::function<void ()> func) {
		display_callback_ = func;
		::glutDisplayFunc(&display_callback_delegate);
	}

	/**
	 * \brief Wrapper around GLUT method (supports class member methods).
	 * \see http://www.opengl.org/resources/libraries/glut/spec3/spec3.html
	 */
	void glutKeyboardFunc(boost::function<void (unsigned char key, int x, int y)> func) {
		keyboard_callback_ = func;
		::glutKeyboardFunc(&keyboard_callback_delegate);
	}

	/**
	 * \brief Wrapper around GLUT method (supports class member methods).
	 * \see http://www.opengl.org/resources/libraries/glut/spec3/spec3.html
	 */
	void glutMouseFunc(boost::function<void (int button, int state, int x, int y)> func) {
		mouse_callback_ = func;
		::glutMouseFunc(&mouse_callback_delegate);
	}

	/**
	 * \brief Wrapper around GLUT method (supports class member methods).
	 * \see http://www.opengl.org/resources/libraries/glut/spec3/spec3.html
	 */
	void glutReshapeFunc(boost::function<void (int width, int height)> func) {
		reshape_callback_ = func;
		::glutReshapeFunc(&reshape_callback_delegate);
	}
};

#endif /* PGGLUT_H_ */

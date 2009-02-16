/*
 * pg_glut.cc
 *
 *  Created on: 09.02.2009
 *      Author: peter
 */

#include "pg_glut.h"

#include <string>
#include <iostream>

#include "gl_headers.h"
#include "glut_headers.h"


namespace PgGLUT {
	// some intern methods/data
	namespace {
		bool initialized = false;

		boost::function<void ()> display_callback_;
		boost::function<void (int width, int height)> reshape_callback_;
		boost::function<void (unsigned char key, int x, int y)> keyboard_callback_;
		boost::function<void (int button, int state, int x, int y)> mouse_callback_;

		boost::function<void (int value)> timer_callback_;

		// delegates to be registered as GLUT callbacks
		void display_callback_delegate() { display_callback_(); }
		void keyboard_callback_delegate(unsigned char key, int x, int y) { keyboard_callback_(key, x, y); }
		void mouse_callback_delegate(int button, int state, int x, int y) { mouse_callback_(button, state, x, y); }
		void reshape_callback_delegate(int width, int height) { reshape_callback_(width, height); }

		void timer_callback_delegate(int value) {timer_callback_(value);}
	}

	void init(const std::string& window_name, int& argc, char** argv, int width, int height, int x, int y) {
		if (!initialized) {
			::glutInit(&argc, argv);
			::glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
			::glutInitWindowSize(width, height);
			::glutInitWindowPosition(x, y);
			::glutCreateWindow(window_name.c_str());
			initialized = true;
		}
	}

	void glutMainLoop() {
		if (!initialized) {
			std::cerr << "Warning: can not enter GLUT main loop (GLUT not initialized)" << std::endl;
			return;
		}
		::glutMainLoop();
	}

	void glutDisplayFunc(boost::function<void ()> func) {
		display_callback_ = func;
		::glutDisplayFunc(&display_callback_delegate);
	}

	void glutKeyboardFunc(boost::function<void (unsigned char key, int x, int y)> func) {
		keyboard_callback_ = func;
		::glutKeyboardFunc(&keyboard_callback_delegate);
	}

	void glutMouseFunc(boost::function<void (int button, int state, int x, int y)> func) {
		mouse_callback_ = func;
		::glutMouseFunc(&mouse_callback_delegate);
	}

	void glutReshapeFunc(boost::function<void (int width, int height)> func) {
		reshape_callback_ = func;
		::glutReshapeFunc(&reshape_callback_delegate);
	}

	void glutTimerFunc(unsigned int msec, boost::function<void (int value)> func, int value) {
		timer_callback_ = func;
		::glutTimerFunc(msec, &timer_callback_delegate, value);
	}
}

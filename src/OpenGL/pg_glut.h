/*
 * pg_glut.h
 *
 *  Created on: 21.01.2009
 *      Author: peter
 */

#ifndef PGGLUT_H_
#define PGGLUT_H_


#include <string>

#include <boost/function.hpp>


/**
 * \brief C++ wrapper for the OpenGL Toolkit (GLUT) Interface.
 *
 * For the documentation of the 'glut*Func(...)' callbacks, see the GLUT documentation. Note that this wrapper supports
 * class member methods as callbacks.
 *
 * Currently supported callbacks:
 *  - display callback
 *  - keyboard callback
 *  - special keys callback
 *  - mouse callback
 *  - reshape callback
 *  - timer callback
 *
 * \see http://www.opengl.org/resources/libraries/glut/spec3/spec3.html
 */
// doesn't use formatting from our codeguideline for interface methods to stay consistent with original GLUT interface
namespace PgGLUT {
	/**
	 * \brief Initializes the GLUT OpenGL context.
	 *
	 * Note that calling this method more than once has no effect.
	 * \see http://www.opengl.org/resources/libraries/glut/spec3/spec3.html
	 */
	void init(const std::string& window_name, int& argc, char** argv, int width=500, int height=500, int x=0, int y=0);

	/**
	 * \brief Starts the GLUT main loop.
	 *
	 * Checks wether GLUT was initialized using PgGLUT::init. If so, the main loop is entered. If not, a warning is
	 * issued and the method returns.
	 */
	void glutMainLoop();

	/**
	 * \brief Wrapper around GLUT method (supports class member methods).
	 * \see http://www.opengl.org/resources/libraries/glut/spec3/spec3.html
	 */
	void glutDisplayFunc(boost::function<void ()> func);

	/**
	 * \brief Wrapper around GLUT method (supports class member methods).
	 * \see http://www.opengl.org/resources/libraries/glut/spec3/spec3.html
	 */
	void glutSpecialFunc(boost::function<void (int key, int x, int y)> func);
	/**
		 * \brief Wrapper around GLUT method (supports class member methods).
		 * \see http://www.opengl.org/resources/libraries/glut/spec3/spec3.html
		 */
	void glutKeyboardFunc(boost::function<void (unsigned char key, int x, int y)> func);



		/**
	 * \brief Wrapper around GLUT method (supports class member methods).
	 * \see http://www.opengl.org/resources/libraries/glut/spec3/spec3.html
	 */
	void glutMouseFunc(boost::function<void (int button, int state, int x, int y)> func);

	/**
	 * \brief Wrapper arround GLUT method (supports class member methods)
	 * \see http://www.opengl.org/resources/libraries/glut/spec3/spec3.html
	 */
	void glutMotionFunc ( boost::function<void (int x, int y)> func);
	/**
	 * \brief Wrapper around GLUT method (supports class member methods).
	 * \see http://www.opengl.org/resources/libraries/glut/spec3/spec3.html
	 */
	void glutReshapeFunc(boost::function<void (int width, int height)> func);

	/**
	 * \brief Wrapper around GLUT method (supports class member methods).
	 * \see http://www.opengl.org/resources/libraries/glut/spec3/spec3.html
	 */
	void glutTimerFunc(unsigned int msec, boost::function<void (int value)> func, int value);
};

#endif /* PGGLUT_H_ */

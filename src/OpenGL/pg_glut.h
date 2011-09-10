/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Alexander Klaas <aklaas@uni-paderborn.de>
	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>

	RobotSwarmSimulator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RobotSwarmSimulator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RobotSwarmSimulator.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * pg_glut.h
 *
 *  Created on: 25.08.2011
 *      Author: Sascha brandt
 */
#include "gl_headers.h"

#ifndef PGGLUT_H_
#define PGGLUT_H_


/**
 * \brief glut independend clones of some glut functions
 */
namespace PgGLUT {

/**
 * Draw a bitmap character.
 *
 * \note for now the font is set to Helvetica12.
 */
void glutBitmapCharacter( int character );

/**
 * Draw a solid sphere.
 */
void glutSolidSphere(GLdouble radius, GLint slices, GLint stacks);

/**
 * Draw a wireframe cone.
 */
void glutWireCone(GLdouble base, GLdouble height, GLint slices, GLint stacks);

/**
 * Draw a solid cone.
 */
void glutSolidCone(GLdouble base, GLdouble height, GLint slices, GLint stacks);

};

#endif /* PGGLUT_H_ */

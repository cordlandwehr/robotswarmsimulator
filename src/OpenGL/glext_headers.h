/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>

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

	#include <windows.h>
#endif
#ifdef __APPLE__
    #include <OpenGL/glext.h>
#else
    #include <GL/glext.h>
#endif

#endif /* GLEXT_HEADERS_H_ */

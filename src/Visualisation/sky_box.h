/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
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
 * sky_box.h
 *
 *  Created on: 21.02.2009
 *      Author: kamil
 */

#ifndef SKY_BOX_H_
#define SKY_BOX_H_

#include <string>

#include "texture.h"

/**
 * This represents a skybox.
 * It renders a sky in infinity.
 *
 *
 */
class SkyBox{
 public:

	SkyBox();
	~SkyBox();

	/**
	 * This method loads all textures.
	 */
	bool init( std::string path_to_tex);

	/**
	 * This draws the sky box.
	 */
	void draw();

 private:

	 /**
	  *
	  */
	 enum TexNum {
		 TEX_FRONT,
		 TEX_BACK,
		 TEX_UP,
		 TEX_DOWN,
		 TEX_RIGHT,
		 TEX_LEFT
	 };

	 /**
	  * Saves the textures needed by the skybox
	  */
	 Texture texture_[6];

	 /**
	  * Indicates whether the sky box can be used.
	  */
	 bool can_use_ ;

	 /**
	  * Index for the compiled list
	  */
	 unsigned int compiled_list_;
};

#endif /* SKY_BOX_H_ */

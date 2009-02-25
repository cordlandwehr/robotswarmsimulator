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


	 Texture texture_[6];
	 bool can_use_ ;
	 unsigned int compiled_list_;

};

#endif /* SKY_BOX_H_ */
/*
 * sky_box.cc
 *
 *  Created on: 21.02.2009
 *      Author: kamil
 */
#include <iostream>

#include "../OpenGL/gl_headers.h"
#include "../OpenGL/glu_headers.h"
#include "../OpenGL/glut_headers.h"

#include "texture.h"
#include "sky_box.h"
#include <Utilities/console_output.h>


float kSkyBoxSize = 6.0f;

namespace texnames {

	std::string tex_names[6] = {"front.bmp", "back.bmp", "up.bmp", "down.bmp", "right.bmp", "left.bmp" };


};

SkyBox::SkyBox(){



}

SkyBox::~SkyBox(){

}

bool SkyBox::init( std::string path_to_tex){

	// Load Textures...
	for( int i = 0; i < 6 ; i++){
		std::string file = path_to_tex;
		file = file.append( texnames::tex_names[i] );
		ConsoleOutput::out_warning( "[VISUALIZATION] Cannot load skybox texture: " + file );

		texture_[i].load(file);
		if( texture_[i].loaded() == false){
			ConsoleOutput::out_warning( "[VISUALIZATION] Cannot load skybox texture: " + file );
			can_use_ = false;
			return false;
		}

	}



	// Create Compiled List
	compiled_list_ = glGenLists( 1);

	glNewList(compiled_list_, GL_COMPILE );
	glDepthMask( GL_FALSE );
	glDisable( GL_LIGHTING );
	glEnable( GL_TEXTURE_2D );

	glColor3f(1.0f,1.0f,1.0f);
	 // Front
	texture_[TEX_FRONT].bind();
	glBegin(GL_QUADS);
	  glTexCoord2f(1, 0); glVertex3f(-kSkyBoxSize, -kSkyBoxSize, kSkyBoxSize );
	  glTexCoord2f(1, 1); glVertex3f(-kSkyBoxSize, kSkyBoxSize, kSkyBoxSize );
	  glTexCoord2f(0, 1); glVertex3f(kSkyBoxSize, kSkyBoxSize, kSkyBoxSize );
	  glTexCoord2f(0, 0); glVertex3f(kSkyBoxSize, -kSkyBoxSize, kSkyBoxSize );
	 glEnd();

	// BACK
	texture_[TEX_BACK].bind();
	 glBegin(GL_QUADS);
	  glTexCoord2f(0, 0); glVertex3f(-kSkyBoxSize, -kSkyBoxSize, -kSkyBoxSize);
	  glTexCoord2f(0, 1); glVertex3f(-kSkyBoxSize, kSkyBoxSize, -kSkyBoxSize);
	  glTexCoord2f(1, 1); glVertex3f(kSkyBoxSize, kSkyBoxSize, -kSkyBoxSize);
	  glTexCoord2f(1, 0); glVertex3f(kSkyBoxSize, -kSkyBoxSize, -kSkyBoxSize);
	 glEnd();

	// UP
	texture_[TEX_UP].bind();
	 glBegin(GL_QUADS);
	  glTexCoord2f(0, 0); glVertex3f(-kSkyBoxSize, kSkyBoxSize , -kSkyBoxSize);
	  glTexCoord2f(0, 1); glVertex3f(-kSkyBoxSize, kSkyBoxSize , kSkyBoxSize);
	  glTexCoord2f(1, 1); glVertex3f(kSkyBoxSize, kSkyBoxSize , kSkyBoxSize);
	  glTexCoord2f(1, 0); glVertex3f(kSkyBoxSize, kSkyBoxSize , -kSkyBoxSize);
	 glEnd();


	// DOWN
	texture_[TEX_DOWN].bind();
	 glBegin(GL_QUADS);
	  glTexCoord2f(1, 1); glVertex3f(kSkyBoxSize, -kSkyBoxSize, -kSkyBoxSize);
	  glTexCoord2f(1, 0); glVertex3f(kSkyBoxSize, -kSkyBoxSize, kSkyBoxSize);
	  glTexCoord2f(0, 0); glVertex3f(-kSkyBoxSize, -kSkyBoxSize, kSkyBoxSize);
	  glTexCoord2f(0, 1); glVertex3f(-kSkyBoxSize, -kSkyBoxSize, -kSkyBoxSize);
	 glEnd();

	// RIGHT
	texture_[TEX_RIGHT].bind();
	 glBegin(GL_QUADS);
	  glTexCoord2f(0, 0); glVertex3f(kSkyBoxSize, -kSkyBoxSize, -kSkyBoxSize);
	  glTexCoord2f(1, 0); glVertex3f(kSkyBoxSize, -kSkyBoxSize, kSkyBoxSize);
	  glTexCoord2f(1, 1); glVertex3f(kSkyBoxSize, kSkyBoxSize, kSkyBoxSize);
	  glTexCoord2f(0, 1); glVertex3f(kSkyBoxSize, kSkyBoxSize, -kSkyBoxSize);
	 glEnd();

	// LEFT
	 texture_[TEX_LEFT].bind();
	  glBegin(GL_QUADS);
	   glTexCoord2f(1, 0); glVertex3f(-kSkyBoxSize , -kSkyBoxSize, -kSkyBoxSize);
	   glTexCoord2f(0, 0); glVertex3f(-kSkyBoxSize , -kSkyBoxSize, kSkyBoxSize);
	   glTexCoord2f(0, 1); glVertex3f(-kSkyBoxSize , kSkyBoxSize, kSkyBoxSize);
	   glTexCoord2f(1, 1); glVertex3f(-kSkyBoxSize , kSkyBoxSize, -kSkyBoxSize);
	  glEnd();

	  glDisable(GL_TEXTURE_2D );
	  glEnable( GL_LIGHTING );
	  glDepthMask( GL_TRUE );
	glEndList();
	can_use_ = true;
	return true;

}

void SkyBox::draw(){


	if(can_use_){

		glCallList( compiled_list_ );

	}


}

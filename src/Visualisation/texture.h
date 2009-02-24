/*
 * texture.h
 *
 *  Created on: 14.02.2009
 *      Author: kamil
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <string>

#include <boost/smart_ptr.hpp>

#include "../OpenGL/gl_headers.h"


/**
 * This is a simple texture object. It can load bmp and tga files.
 *
 * To load a texture create an object of this and use the load method. One can check
 * whether the texture could be loaded by using the loaded method.
 *
 * To bind a texture in OpengGL call the bind method before drawing the mesh.
 *
 */
class Texture {
 public:
	 Texture(){}
	 ~Texture(){}

	 /**
	  * This method is used to load a texture.
	  * \param texture_file The name of the texture file.
	  */
	 void load(std::string & texture_file );

	 /**
	  * Used to bind the texture to the current opengl context.
	  */
	 void bind();

	 /**
	  * Returns the width of the loaded texture.
	  */
	 int width(){ return width_;}

	 /**
	  * Returns the height of the loaded texture.
	  */
	 int height(){return height_; }

	 /**
	  * Returns whether a texture was loaded.
	  */
	 bool loaded(){ return loaded_ ;}

	 /**
	  * The file name of the texture.
	  */
	 const std::string & file_name(){return file_name_;}

 private:

	 /**
	  * \enum FileType
	  *
	  * The file extension.
	  */
	 enum FileType {
		 BMP,
		 TGA
	 };

	 /**
	  * This is used to load a texture in bmp format.
	  */
	 void load_bmp();

	 /**
	  * This is used t load a texture in tga format.
	  */
	 void load_tga();

	 /**
	  * Helper method to get the file extension, so the right loading method is called.
	  * The filename must be stored in the file_name_ attribute.
	  */
	 FileType determine_extension();

	 std::string file_name_;
	 bool loaded_;
	 int width_;
	 int height_;

	 /**
	  * \var tex_id_
	  * This saves the texture id.
	  */
	 GLuint tex_id_;

	 /**
	  * \var data_
	  * Stores the texture data as in RGB.
	  * Each value is an unsigned char.
	  */
	 boost::scoped_array<unsigned char> data_;

};

#endif /* TEXTURE_H_ */

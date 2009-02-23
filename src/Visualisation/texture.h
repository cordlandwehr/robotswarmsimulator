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

class Texture {
 public:
	 Texture(){}
	 ~Texture(){}

	 void load(std::string & texture_file );
	 void bind();

	 int width(){ return width_;}
	 int height(){return height_; }
	 bool loaded(){ return loaded_ ;}
	 const std::string & file_name(){return file_name_;}

 private:

	 enum FileType {
		 BMP,
		 TGA
	 };

	 void load_bmp();
	 void load_tga();
	 FileType determine_extension();

	 std::string file_name_;
	 bool loaded_;
	 int width_;
	 int height_;

	 GLuint tex_id_;

	 boost::scoped_array<unsigned char> data_;

};

#endif /* TEXTURE_H_ */

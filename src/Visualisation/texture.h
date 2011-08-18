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
 * This is a simple texture object. It can load BMP and uncompressed TGA files.
 *
 * To load a texture create an object of this and use the load method. One can check
 * whether the texture could be loaded by using the loaded method.
 *
 * To bind a texture in OpengGL call the bind method before drawing the mesh.
 *
 */
class Texture {
 public:
	 Texture(): loaded_(false){}
	 ~Texture();

	 /**
	  * This method is used to load a texture.
	  *
	  * The file can be a windows BMP or an uncompressed TGA file.
	  *
	  * \param texture_file The name of the texture file.
	  */
	 void load(std::string & texture_file );

	 /**
	  * Used to bind the texture to the current opengl context.
	  */
	 void bind() const;

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
		 TGA,
		 UNKNOWN_FILE_TYPE
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

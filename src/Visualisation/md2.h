/*
 * md2.h
 *
 *  Created on: 13.02.2009
 *      Author: kamil
 */

#ifndef MD2_H_
#define MD2_H_

#include <string>

#include <boost/smart_ptr.hpp>

#include "texture.h"

/**
 * This class represents a 3d model. It can load a model in the MD2 format.
 *
 * The model can be loaded using the load_model method. The texture must have
 * the same name as the model. It can be in tga or bmp format.
 *
 * To draw the model use the draw_model method. It draws the model in
 * the origin. Right now the animation of the model is unused. So the first frame
 * will be drawn every time.
 *
 */
class MD2 {
 public:

	 /**
	  * \brief Loads a md2 model.
	  *
	  * The texture must have the same name as the model file.
	  * \param model_file The model file
	  *
	  * \return True when the model could be loaded.
	  */
	 bool load_model(std::string & model_file );

	 /**
	  * \brief Draws the model in the origin
	  */
	 void draw_model();

 private:

	 void draw_model_simple();
	 void draw_model_vbo();
	 void draw_model_lists();


	 void setup_lists();
	 void try_setup_vbo();

	 /**
	  * \enum DrawType
	  *
	  * Indicator values for choosing the drawing routine.
	  */
	 enum DrawType {
		 DRAW_SIMPLE,
		 DRAW_LISTS,
		 DRAW_VBO
	 };

	 /**
	  * A MD2 format header.
	  */
	 struct Header{
	 	int ident;
	 	int version;
	 	int skin_width;
	 	int skin_height;
	 	int frame_size;
	 	int num_skins;
	 	int num_point;
	 	int num_tex_coords;
	 	int num_triangles;
	 	int num_gl_cmds;
	 	int num_frames;
	 	int offset_st;
	 	int offset_triangles;
	 	int offset_frames;
	 	int offset_gl_cmd;
	 	int offset_end;
	 };

	 /**
	  * Textcoord struct to hold the texture coordinates for a given vertex.
	  * This one is used for drawing.
	  */
	 struct Texcoord{
		 float s;
		 float t;
	 };

	 /**
	  * Textcoord struct to hold the texture coordinates for a given vertex.
	  * This one is used during loading the mesh.
	  */
	 struct STIndex{
		 short s;
		 short t;
	 };

	 /**
	  * A Point in the model file.
	  * The normal_index is unused because it is an index to an array stored in quake2.
	  */
	 struct Point {
		 unsigned char v[3];
		 unsigned char normal_index;
	 };

	 /**
	  * The frame struct holds all transformation for a given frame.
	  * The fp is an array to the point used in this frame
	  */
	 struct Frame {
		 float scale[3];
		 float translate[3];
		 char name[16];
		 Point fp[1];
	 };

	 /**
	  * a simple vector, so we can use vertex lists to draw the object
	  */
	 struct Vec{
		 float p[3];
	 };

	 /**
	  * A simple entry in the mesh. This represents a triangle given by the points
	  * with indices in vec_index and texture coordinates stored in the st list with
	  * indices st_index
	  */
	 struct Mesh {
		 unsigned short vec_index[3];
		 unsigned short st_index[3];
	 };


	 std::size_t num_frames_;
	 std::size_t num_point_;
	 std::size_t num_triangles_;
	 std::size_t num_st_;
	 std::size_t frame_size_;
	 std::size_t frame_index_; // currently unused
	 std::size_t next_frame_; // currently unused

	 float interpolate_;
	 /**
	  *
	  */
	 boost::scoped_array<Mesh>  triangle_index_;
	 boost::scoped_array<Texcoord> st_index_;
	 boost::scoped_array<Vec> point_list_;

	 boost::scoped_array<Vec> normal_list_;

	 Texture texture_;


	 DrawType  draw_type;

	 /**
	  * used for scaling.
	  */
	 float max_dim_size_;


};

#endif /* MD2_H_ */
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


class MD2 {
 public:

	 bool load_model(std::string & model_file );

	 void draw_model();

 private:

	 void draw_model_simple();
	 void draw_model_vbo();
	 void draw_model_lists();


	 void setup_lists();
	 void try_setup_vbo();

	 enum DrawType {
		 DRAW_SIMPLE,
		 DRAW_LISTS,
		 DRAW_VBO
	 };

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

	 struct Texcoord{
		 float s;
		 float t;
	 };

	 struct STIndex{
		 short s;
		 short t;
	 };

	 struct Point {
		 unsigned char v[3];
		 unsigned char normal_index;
	 };

	 struct Frame {
		 float scale[3];
		 float translate[3];
		 char name[16];
		 Point fp[1];
	 };

	 struct Vec{
		 float p[3];
	 };

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
	 boost::scoped_array<Mesh>  triangle_index_;
	 boost::scoped_array<Texcoord> st_index_;
	 boost::scoped_array<Vec> point_list_;

	 boost::scoped_array<Vec> normal_list_;

	 Texture texture_;

	 DrawType  draw_type;

	 float max_dim_size_;


};

#endif /* MD2_H_ */

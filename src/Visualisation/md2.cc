/*
 * md2.cc
 *
 *  Created on: 14.02.2009
 *      Author: kamil
 */

#include <cstdio>
#include <cstdlib>



#include "../OpenGL/gl_headers.h"
#include "../OpenGL/glu_headers.h"
#include "../OpenGL/glut_headers.h"

#include "md2.h"
#include "texture.h"


bool MD2::load_model(std::string & model_file ){

	std::FILE * fp = std::fopen( model_file.c_str(), "rb");
	if(fp == NULL)
		return false;

	std::fseek( fp, 0, SEEK_END );
	std::size_t file_size = std::ftell(fp);
	std::fseek( fp, 0, SEEK_SET );

	boost::scoped_array<char> buffer(new char [file_size + 1] );
	std::fread(buffer.get(), sizeof(char), file_size, fp );


	Header * header = reinterpret_cast<Header*>( buffer.get() );

	 point_list_.reset( new Vec [ header->num_point * header->num_frames ] );

	this->num_point_ = header->num_point;
	this->num_frames_ = header->num_frames;
	this->frame_size_ = header->frame_size;


	for( int i = 0; i < num_frames_; i++ ){
		Frame * frame = reinterpret_cast<Frame*>( &buffer[ header->offset_frames + frame_size_ * i ] );

		Vec * vl_ptr = &point_list_[ num_point_ * i ];
		for(int j = 0; j < num_point_ ; j++ ){

			vl_ptr[j].p[0] = frame->scale[0] * frame->fp[j].v[0] + frame->translate[0];
			vl_ptr[j].p[1] = frame->scale[1] * frame->fp[j].v[1] + frame->translate[1];
			vl_ptr[j].p[2] = frame->scale[2] * frame->fp[j].v[2] + frame->translate[2];

		}
	}

	num_st_ = header->num_tex_coords;
	st_index_.reset( new Texcoord[ num_st_ ] );


	STIndex * st_ptr = reinterpret_cast<STIndex* >(&buffer[header->offset_st ] );
	for( int i  = 0; i < num_st_; i++ ){
		st_index_[i].s = 1.0 * st_ptr[i].s / texture_.width();
		st_index_[i].t = 1.0 * st_ptr[i].t / texture_.height();
	}

	triangle_index_.reset( new Mesh[ num_triangles_ ] );
	Mesh * index_ptr = reinterpret_cast<Mesh*>(&buffer[ header->offset_triangles ] );

	for(int j = 0; j < num_frames_; j++ ){

		for( int i = 0; i < num_triangles_; i++ ){

			triangle_index_[i].vec_index[0] = index_ptr[i].vec_index[0];
			triangle_index_[i].vec_index[1] = index_ptr[i].vec_index[1];
			triangle_index_[i].vec_index[2] = index_ptr[i].vec_index[2];

			triangle_index_[i].st_index[0] = index_ptr[i].st_index[0];
			triangle_index_[i].st_index[1] = index_ptr[i].st_index[1];
			triangle_index_[i].st_index[2] = index_ptr[i].st_index[2];

		}

	}

	std::fclose( fp );

	try_setup_vbo();

	return true;

}

void MD2::try_setup_vbo(){

	draw_type = DRAW_SIMPLE;
}

void MD2::draw_model(){

	if( draw_type == DRAW_SIMPLE ){

		draw_model_simple();

	} else {

		draw_model_vbo();

	}

}

void MD2::draw_model_simple(){

	int frame = 0;
	Vec * vecs = &point_list_[ num_point_ * frame ];

	texture_.bind();

	glBegin( GL_TRIANGLES );
		for( int i = 0; i < num_triangles_; i++){

			glTexCoord2f(st_index_[ triangle_index_[i].st_index[ 0 ] ].s, st_index_[ triangle_index_[i].st_index[ 0 ] ].t );
			glVertex3fv( vecs[ triangle_index_[i].vec_index[0] ].p );

			glTexCoord2f(st_index_[ triangle_index_[i].st_index[ 2 ] ].s, st_index_[ triangle_index_[i].st_index[ 2 ] ].t );
			glVertex3fv( vecs[ triangle_index_[i].vec_index[2] ].p );

			glTexCoord2f(st_index_[ triangle_index_[i].st_index[ 1 ] ].s, st_index_[ triangle_index_[i].st_index[ 1 ] ].t );
			glVertex3fv( vecs[ triangle_index_[i].vec_index[1] ].p );

		}

	glEnd();
}

void MD2::setup_lists(){
 //TODO
}

void MD2::draw_model_lists(){
 //TODO
}

void MD2::draw_model_vbo(){
 //TODO

}

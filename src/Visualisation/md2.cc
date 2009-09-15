/*
 * md2.cc
 *
 *  Created on: 14.02.2009
 *      Author: kamil
 */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cmath>


#include "../OpenGL/gl_headers.h"
#include "../OpenGL/glu_headers.h"
#include "../OpenGL/glut_headers.h"
#include "../OpenGL/glext_headers.h"

#include "md2.h"
#include "texture.h"

const float kScale  = 0.01;

bool MD2::load_model(const std::string & model_file ){

	std::string texture_name( model_file );

	std::size_t length = texture_name.size();

	//Change file extension to bmp

	texture_name[length-3] = 'b';
	texture_name[length-2] = 'm';
	texture_name[length-1] = 'p';

	std::cout << "MD2: Loading texture for model from file " << texture_name << "!" <<std::endl;

	texture_.load( texture_name );

	std::cout << "MD2: Loading model file " << model_file << std::endl;

	bool model_loaded = load_model_file( model_file );



	if( !model_loaded ){
		std::cerr << "Can't load model file " << model_file << "!" << std::endl;
	}

	return true;

}

void MD2::try_setup_vbo() {
	draw_type = DRAW_LISTS;
}


bool MD2::load_model_file(const std::string & model_file ){

	std::FILE * fp = std::fopen( model_file.c_str(), "rb");
	if(fp == NULL){
		 std::cerr << "Can't open file!" << std::endl;
		return false;
	}
	std::fseek( fp, 0, SEEK_END );

	std::size_t file_size = std::ftell(fp);
	std::cout << "file size: " << file_size << std::endl;

	std::fseek( fp, 0, SEEK_SET );
	boost::scoped_ptr<Header> header( new Header );

	boost::scoped_array<unsigned char> buffer(new unsigned char [file_size + 1] );
	//std::size_t bytes_read = std::fread(buffer.get(), sizeof(char), file_size, fp );
	std::fread(header.get() , 1 , sizeof(Header),fp);

	if( header->version != 8){

		std::cerr << "Wrong version!"<<std::endl;
		std::fclose( fp );
		return false;
	}


	point_list_.reset( new Vec [ header->num_point * header->num_frames ] );

	this->num_point_ = header->num_point;
	this->num_frames_ = header->num_frames;
	this->frame_size_ = header->frame_size;
	this->num_triangles_ = header->num_triangles;



	for( unsigned int i = 0; i < num_frames_; i++ ){

		std::fseek( fp , header->offset_frames + i * frame_size_ , SEEK_SET);
		// TODO(peter): I guess the side effect is needed, but br itself not; please recheck and adapt as necessary
		//std::size_t br = std::fread( buffer.get(), 1, frame_size_  , fp);
		std::fread( buffer.get(), 1, frame_size_  , fp);

		Frame * frame = (Frame*)( buffer.get());

		Vec * vl_ptr = &point_list_[ num_point_ * i ];


		for( unsigned int j = 0; j < num_point_ ; j++ ){

			vl_ptr[j].p[0] = kScale *( frame->scale[0] * frame->fp[j].v[0] + frame->translate[0]);
			//Quake uses the z axis as up, we use the y axis as up, so switch the axis
			vl_ptr[j].p[2] = kScale * (frame->scale[1] * frame->fp[j].v[1] + frame->translate[1]);
			vl_ptr[j].p[1] = kScale *(frame->scale[2] * frame->fp[j].v[2] + frame->translate[2]);


		}
	}

	num_st_ = header->num_tex_coords;
	st_index_.reset( new Texcoord[ num_st_ ] );

	std::fseek(fp , header->offset_st , SEEK_SET);
	std::fread( buffer.get(), sizeof(STIndex), header->num_tex_coords , fp);

	STIndex * st_ptr = reinterpret_cast<STIndex* >( buffer.get() );
	for(unsigned int i  = 0; i < num_st_; i++ ) {
		std::cout << "s: " << st_index_[i].s << "\tt: " << st_ptr[i].t << std::endl;
		st_index_[i].s = 1.0 * st_ptr[i].s / header->skin_width;
		st_index_[i].t = 1.0 - 1.0 * st_ptr[i].t / header->skin_height;
	}

	triangle_index_.reset( new Mesh[ num_triangles_ ] );

	std::fseek(fp ,header->offset_triangles , SEEK_SET );
	std::fread( buffer.get(), sizeof(Mesh), header->num_triangles , fp);

	Mesh * index_ptr = reinterpret_cast<Mesh*>(buffer.get());


	for(unsigned int j = 0; j < num_frames_; j++ ){


		for(unsigned int i = 0; i < num_triangles_; i++ ){

			triangle_index_[i].vec_index[0] = index_ptr[i].vec_index[0];
			triangle_index_[i].vec_index[1] = index_ptr[i].vec_index[1];
			triangle_index_[i].vec_index[2] = index_ptr[i].vec_index[2];

			triangle_index_[i].st_index[0] = index_ptr[i].st_index[0];
			triangle_index_[i].st_index[1] = index_ptr[i].st_index[1];
			triangle_index_[i].st_index[2] = index_ptr[i].st_index[2];

		}

	}

	std::fclose( fp );

	calculate_normals();
	setup_lists();
	try_setup_vbo();

	return true;


}

void MD2::calculate_normals(){

	normal_list_.reset( new Vec[ num_point_ * num_frames_ ] );

	for(unsigned int frame = 0; frame < num_frames_; frame++){


		Vec * vecs = &point_list_[ num_point_ * frame ];
		Vec * normals = &normal_list_[num_point_ * frame ];

		for(unsigned int i = 0; i < num_triangles_; i++){

					Vec v1 = vec_sub(vecs[ triangle_index_[i].vec_index[2] ], vecs[ triangle_index_[i].vec_index[0] ]);
					Vec v2 = vec_sub(vecs[ triangle_index_[i].vec_index[1] ] ,vecs[ triangle_index_[i].vec_index[0] ]);
					Vec normal = calculate_normal(v1,v2);

					normals[ triangle_index_[i].vec_index[0] ].p[0] = normal.p[0];
					normals[ triangle_index_[i].vec_index[0] ].p[1] = normal.p[1];
					normals[ triangle_index_[i].vec_index[0] ].p[2] = normal.p[2];

					normals[ triangle_index_[i].vec_index[1] ].p[0] = normal.p[0];
					normals[ triangle_index_[i].vec_index[1] ].p[1] = normal.p[1];
					normals[ triangle_index_[i].vec_index[1] ].p[2] = normal.p[2];

					normals[ triangle_index_[i].vec_index[2] ].p[0] = normal.p[0];
					normals[ triangle_index_[i].vec_index[2] ].p[1] = normal.p[1];
					normals[ triangle_index_[i].vec_index[2] ].p[2] = normal.p[2];

		}
	}

}

void MD2::draw_model() const{

	if( draw_type == DRAW_SIMPLE ){

		draw_model_simple();

	}  else if ( draw_type == DRAW_LISTS ){
		draw_model_lists();
	}

}

void MD2::draw_model_simple() const{


	int frame = 1;
	Vec * vecs = &point_list_[ num_point_ * frame ];
	Vec * normals = &normal_list_[ num_point_ * frame ];

	glEnable( GL_TEXTURE_2D );
	glDisable( GL_LIGHTING );
	glColor3f(1.0f, 1.0f, 1.0f);
	texture_.bind();

	glBegin( GL_TRIANGLES );
		for(unsigned int i = 0; i < num_triangles_; i++){

			glNormal3fv(normals[ triangle_index_[i].vec_index[0] ].p );

			glTexCoord2f(st_index_[ triangle_index_[i].st_index[ 0 ] ].s, st_index_[ triangle_index_[i].st_index[ 0 ] ].t );
			glVertex3fv( vecs[ triangle_index_[i].vec_index[0] ].p );

			glTexCoord2f(st_index_[ triangle_index_[i].st_index[ 2 ] ].s, st_index_[ triangle_index_[i].st_index[ 2 ] ].t );
			glVertex3fv( vecs[ triangle_index_[i].vec_index[2] ].p );

			glTexCoord2f(st_index_[ triangle_index_[i].st_index[ 1 ] ].s, st_index_[ triangle_index_[i].st_index[ 1 ] ].t );
			glVertex3fv( vecs[ triangle_index_[i].vec_index[1] ].p );

		}

	glEnd();

	glDisable(GL_TEXTURE_2D );
	glEnable(GL_LIGHTING );
}

void MD2::setup_lists(){




	indices_list_.reset( new unsigned short [  num_triangles_ *3 +3] );
		unsigned short offset = 0 ;
		for(unsigned int i = 0; i < num_triangles_; i++){

			indices_list_[offset] =  triangle_index_[i].vec_index[0];
			indices_list_[offset +1 ] = triangle_index_[i].vec_index[2];
			indices_list_[offset + 2] = triangle_index_[i].vec_index[1];

			offset += 3;
		}



}

void MD2::draw_model_lists() const{

	glDisable(GL_LIGHTING );
	glEnable( GL_TEXTURE_2D );
	glColor3f(1.0f,1.0f,1.0f);
	texture_.bind();
	unsigned int frame = 1;


	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState(GL_NORMAL_ARRAY );

	glNormalPointer(GL_FLOAT, 0, normal_list_.get() );
	glVertexPointer(3, GL_FLOAT, 0, &point_list_[ frame * num_point_ ] );
	glTexCoordPointer(2,GL_FLOAT,0, st_index_.get() );

	glDrawElements(GL_TRIANGLES, num_triangles_ *3 , GL_UNSIGNED_SHORT, indices_list_.get() );

	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );

	glDisable( GL_TEXTURE_2D );
	glEnable(GL_LIGHTING );
}




MD2::Vec MD2::calculate_normal(Vec &v1,Vec &v2) const{

	Vec res;

	res.p[0] = v1.p[1]*v2.p[2] - v1.p[2]*v2.p[1];
	res.p[1] = v1.p[2]*v2.p[0] - v1.p[0]*v2.p[2];
	res.p[2] = v1.p[0]*v2.p[1] - v1.p[1]*v2.p[0];

	float length = std::sqrt( res.p[0]* res.p[0] + res.p[1]*res.p[1] + res.p[2]*res.p[2] );

	res.p[0] /= length;
	res.p[1] /= length;
	res.p[2] /= length;

	return res;
}

MD2::Vec MD2::vec_sub(Vec & v1 , Vec & v2) const{

	Vec ret;

	ret.p[0] = v1.p[0] - v2.p[0];
	ret.p[1] = v1.p[1] - v2.p[1];
	ret.p[2] = v1.p[2] - v2.p[2];

	return ret;
}

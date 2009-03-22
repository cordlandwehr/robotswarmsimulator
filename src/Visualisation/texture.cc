/*
 * texture.cc
 *
 *  Created on: 20.02.2009
 *      Author: kamil
 */
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <iostream>

#include "../OpenGL/gl_headers.h"
#include "../OpenGL/glu_headers.h"

#include "texture.h"
#include <Utilities/console_output.h>

#define BITMAP_ID 0x4D42
#define BI_RGB 0x0000
#define TGA_24 0x00
#define TGA_32 0x08
#define GL_CLAMP_TO_EDGE 0x812F


namespace fileheader {

typedef unsigned long DWORD, *PDWORD, *LPDWORD;
typedef long LONG, *PLONG, *LPLONG;
typedef unsigned short WORD, *PWORD, *LPWORD;

// Structs for opening an bitmap
typedef struct  _BITMAPINFOHEADER{
  DWORD  biSize;
  LONG   biWidth;
  LONG   biHeight;
  WORD   biPlanes;
  WORD   biBitCount;
  DWORD  biCompression;
  DWORD  biSizeImage;
  LONG   biXPelsPerMeter;
  LONG   biYPelsPerMeter;
  DWORD  biClrUsed;
  DWORD  biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef struct _BITMAPFILEHEADER {

  WORD    bfType;
  DWORD   bfSize;
  WORD    bfReserved1;
  WORD    bfReserved2;
  DWORD   bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;




typedef struct   _TGA{
	unsigned char imageIDLength;
	unsigned char colorMapType;
	unsigned char imageTypeCode;
	short int colorMapOrigin;
	short int colorMapLength;
	short int colorMapEntrySize;
	short int imageXOrigin;
	short int imageYOrigin;
	short int imageWidth;
	short int imageHeight;
	unsigned char bitCount;
	unsigned char imageDescriptor;



}TGAHEADER, *PTGAHEADER;

typedef struct  _TGADATA{
	unsigned char imageTypeCode;
	short int imageWidth;
	short int imageHeight;
	unsigned char bitCount;
	unsigned char *imageData;

}TGADATA;


};


Texture::~Texture(){

	if(loaded_){
		glDeleteTextures(1, &tex_id_ );
	}
}

void Texture::bind() const{

	if( loaded_ ){
		glEnable(GL_TEXTURE_2D );
		glBindTexture(GL_TEXTURE_2D, tex_id_ );
	} else  {
		glDisable( GL_TEXTURE_2D );
	}

}

void Texture::load(std::string & texture_file ){

	if( loaded_ ){
		// make clean...
		glDeleteTextures(1, &tex_id_ );
		tex_id_ = 0;
	}

	loaded_ = false;

	file_name_ = texture_file;

	FileType file_type = determine_extension();

	switch ( file_type ){
		case BMP:
				load_bmp();
			break;
		case TGA:
				load_tga();
			break;
		default:
				ConsoleOutput::out_warning( "Can not determine file format.");
			break;

	}

	if( loaded_ ){


		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &tex_id_);
		glBindTexture(GL_TEXTURE_2D, tex_id_ );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );



		//glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width_, height_,0, GL_RGB, GL_UNSIGNED_BYTE, data_.get() );

		gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, width_, height_, GL_RGB, GL_UNSIGNED_BYTE, data_.get() );

		glDisable( GL_TEXTURE_2D );

	} else {
		ConsoleOutput::out_warning( "Cannot load Texture from file: " + texture_file );
	}

}

Texture::FileType Texture::determine_extension(){

	std::size_t length = file_name_.size();
	if( length < 3 ){
		return UNKNOWN_FILE_TYPE;
	}

	if( file_name_[length - 3] == 'b' && file_name_[length - 2 ] == 'm' && file_name_[length - 1] =='p'){
		return BMP;
	} else if ( file_name_[ length - 3 ] == 't' && file_name_[ length-2] == 'g' && file_name_[length - 1]== 'a'){
		return TGA;
	}
	return UNKNOWN_FILE_TYPE;
}


void Texture::load_tga(){


	unsigned char header[6];

	// Uncompressed TGA Header
	unsigned char tga_header[12]={0,0,2,0,0,0,0,0,0,0,0,0};
	unsigned char tga_compare[12];



	std::FILE * fp = fopen(file_name_.c_str(), "rb");

	if( fp == NULL){

		ConsoleOutput::out_warning("Can't find file: " + file_name_ );

		loaded_ = false;
		return;
	}

	std::size_t bytes_read = std::fread(tga_compare, 1, sizeof(tga_compare), fp);

	if( bytes_read != 12){
		ConsoleOutput::out_warning("Read too few bytes.");
		std::fclose(fp);
		loaded_  = false;

		 return;
	}

	if( std::memcmp(tga_header,tga_compare,sizeof(tga_header)) != 0	) {

		ConsoleOutput::out_warning( "Header missmatch." );
		std::fclose(fp);
		loaded_ = false;

		return;
	}

	std::fread(header,1,sizeof(header),fp);



	width_  = header[1] * 256 + header[0];
	height_ = header[3] * 256 + header[2];

	if(	width_	<=0	||	height_	<=0	||	(header[4]!= TGA_24 && header[4]!= TGA_32)) {

		std::fclose(fp);
		loaded_ =false;

		return;
	}

	unsigned char bpp	= header[4];

	std::size_t bytes_per_pixel	= bpp/8;
	std::size_t image_size		= width_ * height_ * bytes_per_pixel;

	 data_.reset( new unsigned char [image_size] );

	if(	data_.get() ==NULL ) {

		std::fclose(fp);
		return;

	}

	if (std::fread(data_.get(), 1, image_size, fp)!= image_size){

	}

	for(std::size_t i=0; i< image_size; i+= bytes_per_pixel)
	{
		unsigned char temp= data_[i];
		data_[i] = data_[i + 2];
		data_[i + 2] = temp;
	}

	std::fclose(fp);
	loaded_ = true;
}



void Texture::load_bmp(){
	std::FILE * fp = std::fopen(file_name_.c_str(), "rb");

	if(fp == NULL){
		ConsoleOutput::out_warning( "Cannot load texture file: "+ file_name_ );
		return;
	}

	fileheader::BITMAPFILEHEADER bmp_header;
	std::fread(&bmp_header, 1, 2, fp);
	std::fread(&bmp_header.bfSize,1,12,fp );

	if( bmp_header.bfType != BITMAP_ID ){
		std::fclose(fp);
		return;
	}

	fileheader::BITMAPINFOHEADER bmp_info;
	std::fread( &bmp_info,1, sizeof( fileheader::BITMAPINFOHEADER ), fp );

	width_ = std::abs(bmp_info.biWidth);
	height_ = std::abs(bmp_info.biHeight);

	if( bmp_info.biSizeImage <= 0){
		bmp_info.biSizeImage = width_ * height_ * 3;
	}

	std::fseek(fp, bmp_header.bfOffBits, SEEK_SET );

	boost::scoped_array<unsigned char> bmp_data (  new unsigned char[ bmp_info.biSizeImage ]) ;

	if(bmp_data.get() == NULL ){

		std::fclose( fp );
		return;
	}

	std::size_t bytes_read = std::fread(bmp_data.get(), 1, bmp_info.biSizeImage , fp );


	data_.reset(   new unsigned char[ bmp_info.biSizeImage ] );

	for( unsigned int index = 0; index < bmp_info.biSizeImage -3  ; index+= 3){

		data_[index] = bmp_data[ index +2];
		data_[index+1] = bmp_data[ index + 1];
		data_[index+2] = bmp_data[ index ];
	}


	std::fclose( fp );
	loaded_ = true;

}


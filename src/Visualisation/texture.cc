/*
 * texture.cc
 *
 *  Created on: 20.02.2009
 *      Author: kamil
 */

#include <cstdio>

#include <iostream>

#include "../OpenGL/gl_headers.h"
#include "../OpenGL/glu_headers.h"

#include "texture.h"

#define BITMAP_ID 0x4D42
#define BI_RGB 0x0000
#define TGA_24 0x00
#define TGA_32 0x08

namespace fileheader {

typedef unsigned long DWORD, *PDWORD, *LPDWORD;
typedef long LONG, *PLONG, *LPLONG;
typedef unsigned short WORD, *PWORD, *LPWORD;

// Structs for opening an bitmap
typedef struct _BITMAPINFOHEADER{
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




typedef struct _TGA{
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

typedef struct _TGADATA{
	unsigned char imageTypeCode;
	short int imageWidth;
	short int imageHeight;
	unsigned char bitCount;
	unsigned char *imageData;

}TGADATA;


};


void Texture::bind(){

		glEnable(GL_TEXTURE_2D );
		glBindTexture(GL_TEXTURE_2D, tex_id_ );


}

void Texture::load(std::string & texture_file ){
	file_name_ = texture_file;

	FileType file_type = determine_extension();

	switch ( file_type ){
		case BMP:
				load_bmp();
			break;
		case TGA:
				load_tga();
			break;

	}

	if( loaded_ ){


		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &tex_id_);
		glBindTexture(GL_TEXTURE_2D, tex_id_ );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

		//glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width_, height_,0, GL_RGB, GL_UNSIGNED_BYTE, data_.get() );

		gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, width_, height_, GL_RGB, GL_UNSIGNED_BYTE, data_.get() );

		glDisable( GL_TEXTURE_2D );

	} else {
		std::cerr << "Can't load Texture from file '" << texture_file << "' !" <<std::endl;
	}

}

Texture::FileType Texture::determine_extension(){
	// TODO
	return BMP;
}

void Texture::load_tga(){

	std::FILE* fp = fopen(file_name_.c_str(), "rb");

	if(fp == NULL){
		return;
	}

	unsigned char ignore = 0;
	std::fread( &ignore, sizeof( ignore ), 1, fp );
	std::fread( &ignore, sizeof( ignore ), 1, fp );

	fileheader::TGAHEADER tga_header;

	std::fread( &tga_header.imageTypeCode, sizeof( unsigned char), 1, fp);

	if( tga_header.imageTypeCode != 2 && tga_header.imageTypeCode != 3 ){
		std::fclose( fp );
		return;
	}

	// ignore next 13 byte
	std::fseek(fp , SEEK_CUR , 13);

	// image size
	std::fread(&tga_header.imageWidth, sizeof(short int), 1, fp );
	std::fread(&tga_header.imageHeight, sizeof(short int),1, fp );

	// image depth

	std::fread(&tga_header.bitCount, sizeof( unsigned char),1, fp);
	std::fread(&ignore, sizeof( ignore ), 1, fp );

	unsigned int color_mode = tga_header.bitCount / 8;
	unsigned int image_size = tga_header.imageHeight * tga_header.imageWidth * color_mode;

	boost::scoped_array<unsigned char> tga_data( new unsigned char [ image_size ] );

	std::fread(tga_data.get(), sizeof(unsigned char) , image_size, fp );

	int index_rgb  = 0;
	data_.reset( new unsigned char [ image_size / color_mode ] );
	for(unsigned int index = 0; index < image_size; index+= color_mode ){

		data_[index_rgb] =  tga_data[index + 2];
		data_[index_rgb +1 ] =  tga_data[index + 1];
		data_[index_rgb + 2] =  tga_data[index];

		index_rgb+=3;
	}

	std::fclose( fp );
	loaded_ = true;

}

void Texture::load_bmp(){
	std::FILE * fp = std::fopen(file_name_.c_str(), "rb");

	if(fp == NULL){
		std::cerr << "Texture: Can't load file "<< file_name_ << std::endl;
		return;
	}

	fileheader::BITMAPFILEHEADER bmp_header;
	std::fread(&bmp_header, sizeof(bmp_header),1, fp);

	if( bmp_header.bfType != BITMAP_ID ){
		std::fclose(fp);
		return;
	}

	fileheader::BITMAPINFOHEADER bmp_info;
	std::fread( &bmp_info, sizeof( bmp_info ), 1, fp );

	width_ = bmp_info.biWidth;
	height_ = bmp_info.biHeight;



	std::fseek(fp, bmp_header.bfOffBits, SEEK_SET );

	boost::scoped_array<unsigned char> bmp_data(  new unsigned char[ bmp_info.biSizeImage ] );

	if(bmp_data.get() == NULL ){

		std::fclose( fp );
		return;
	}

	std::cout << bmp_info.biSizeImage<< " " << bmp_info.biSize<< std::endl;
	std::fread(bmp_data.get(), 1, bmp_info.biSizeImage , fp );



	data_.reset(   new unsigned char[ bmp_info.biSizeImage ] );

	for( unsigned int index = 0; index < bmp_info.biSizeImage  -3; index+= 3){


		data_[index] = bmp_data[ index ];
		data_[index+1] = bmp_data[ index + 2];
		data_[index+2] = bmp_data[ index + 1];

	}


	std::fclose( fp );
	loaded_ = true;

}


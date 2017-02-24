#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//https://en.wikipedia.org/wiki/BMP_file_format#Pixel_array_.28bitmap_data.29

using namespace std;

// 14 bytes
struct BMPHEADERFILE {

	unsigned short type;
	unsigned long size;
	unsigned short reserved1;
	unsigned short reserved2;
	unsigned long offsetBits;

};

// 40 bytes
struct BMPINFOHEADER {

	unsigned int size;               // Header size in bytes      
	int width, height;               // Width and height of image 
	unsigned short int planes;       // Number of colour planes   
	unsigned short int bits;         // Bits per pixel            
	unsigned int compression;        // Compression type          
	unsigned int imagesize;          // Image size in bytes       
	int xresolution, yresolution;    // Pixels per meter          
	unsigned int ncolours;           // Number of colours         
	unsigned int importantcolours;   // Important colours         

};

// 108 bytes
struct BMPV4HEADER {

	char bfType[2];       // "BM" 
	int bfSize;           // Size of file in bytes 
	int bfReserved;       // set to 0 
	int bfOffBits;        // Byte offset to actual bitmap data (= 54) 
	int biSize;           // Size of BITMAPINFOHEADER, in bytes (= 40) 
	int biWidth;          // Width of image, in pixels 
	int biHeight;         // Height of images, in pixels 
	short biPlanes;       // Number of planes in target device (set to 1) 
	short biBitCount;     // Bits per pixel (24 in this case) 
	int biCompression;    // Type of compression (0 if no compression) 
	int biSizeImage;      // Image size, in bytes (0 if no compression) 
	int biXPelsPerMeter;  // Resolution in pixels/meter of display device 
	int biYPelsPerMeter;  // Resolution in pixels/meter of display device 
	int biClrUsed;        // Number of colors in the color table (if 0, use maximum allowed by biBitCount) 
	int biClrImportant;   // Number of important colors.  If 0, all colors are important 

};

enum bmpHeaderType {

	bmpinfoheader,
	bmpv4header

};

class IBmp {

	private :

		struct BMPHEADERFILE mBmpHeader;
		struct BMPINFOHEADER mBmpInfo;
		unsigned char *mData;

		long getImageInfo(FILE* inputFile, long offset, int nbOfChars);
		
	public:
		
		IBmp() {
			mData = NULL;
		}

		~IBmp() {

			if (mData != NULL)
				free(mData);
		}

		void read(const char *bmpInput);
		void printData();
		int getWidth();
		int getHeight();
		unsigned char *getData();

};

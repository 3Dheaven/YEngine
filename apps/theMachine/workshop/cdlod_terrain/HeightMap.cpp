#include "HeightMap.h"

HeightMap::HeightMap(string filePath) 
{
	//mMap = new CTexture(filePath);
	mInputBMP.read(filePath.c_str());

	//Generate texture
	glGenTextures(1, &mTextureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	// Assign texture to mTextureID
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mInputBMP.getWidth(), mInputBMP.getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, mInputBMP.getData());
	//glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glBindTexture(GL_TEXTURE_2D, 0);
};

HeightMap::~HeightMap() 
{
	if (mMap != NULL)
	{
		delete mMap;
	}
};

int 
HeightMap::getWidth() 
{

	return mInputBMP.getWidth();

};

int 
HeightMap::getHeight() 
{

	return mInputBMP.getHeight();

};

GLuint 
HeightMap::getTextureID() 
{

	return mMap->mID;

};

float 
HeightMap::getValue(float x, float y) 
{

	//int i = (int)x % mInputBMP.getWidth();
	//int j = (int)y % mInputBMP.getHeight();
	unsigned char charval = mInputBMP.getData()[(int)x + (int)y*mInputBMP.getWidth()]; // Read gray color value
	// Value between -10.0 to 10.0
	float val = 20.0f*(charval / 255.0f - 0.5f);
	return val;

};

int 
HeightMap::getRealValue(int x, int y) 
{

	//int i = (int)x % mInputBMP.getWidth();
	//int j = (int)y % mInputBMP.getHeight();
	int index = x + y*mInputBMP.getWidth();
	if (index > mInputBMP.getWidth() * mInputBMP.getHeight() - 1)
		return -1;

	unsigned char charval = mInputBMP.getData()[index]; // Read gray color value
	// Value between -10.0 to 10.0
	//float val = 20.0f*(charval / 255.0f - 0.5f);
	return charval;

};

float 
HeightMap::getMaxValueInArea(float x, float y, float width, float height) 
{

	float maxVal = 0.0f;

	for (int i = x; i < x + width; i++) {

		for (int j = y; j < y + height; j++) {

			int val = getRealValue(i, j);
			if (val > maxVal)
				maxVal = val;

		}
	}

	return maxVal;

};

float 
HeightMap::getMinValueInArea(float x, float y, float width, float height) 
{

	float minVal = 255.0f;

	for (int i = x; i < x + width; i++) {

		for (int j = y; j < y + height; j++) {

			int val = getRealValue(i, j);
			if (val < minVal)
				minVal = val;

		}
	}

	return minVal;

};

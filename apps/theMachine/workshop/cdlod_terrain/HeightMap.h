#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <string>
#include "../../../../yengine/image/IBmp.h"
#include "../../../../yengine/image/CTexture.h"

using namespace std;

class HeightMap 
{
private:
	CTexture* mMap;
	IBmp mInputBMP;

public:
	GLuint mTextureID;
	HeightMap(string filePath);
	~HeightMap();
	int getWidth();
	int getHeight();
	GLuint getTextureID();
	float getValue(float x, float y);
	int getRealValue(int x, int y);
	float getMaxValueInArea(float x, float y, float width, float height);
	float getMinValueInArea(float x, float y, float width, float height);

};

#endif
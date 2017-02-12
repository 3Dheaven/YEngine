#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <GL/glew.h> 
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "../scene/CMesh.h"

using namespace std;

class CGraphicDriver
{
protected:
	int vbIdCounter;
	CShaderFactory *mCustomShader;

public:

	CGraphicDriver() : mCustomShader(nullptr)
	{
	}

	~CGraphicDriver()
	{
		if (mCustomShader != nullptr)
		{
			delete mCustomShader;
			mCustomShader = nullptr;
		}
	}

	CShaderFactory *getShader() const
	{
		return mCustomShader;
	}

	virtual void init(CMesh *mesh) const = 0;
	virtual void bindMaterial(CMesh *mesh, CShaderFactory * shader) const = 0;
	virtual void unbindMaterial(CMesh *mesh) const = 0;
	virtual void render(CMesh *mesh, CShaderFactory * shader) const = 0;
	virtual void clean(CMesh *mesh) const = 0;
	virtual void createShader(const string &vertexShader, const string &pixelShader) = 0;
};
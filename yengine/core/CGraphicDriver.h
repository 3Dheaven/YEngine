#pragma once

#include "wx/wxprec.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <windows.h>
#include "CShader.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "../scene/CMesh.h"

using namespace std;

class CGraphicDriver
{
protected:
	int vbIdCounter;
	CShader *mCustomShader;

public:

	CGraphicDriver();
	~CGraphicDriver();

	virtual CShader *getShader() const final;
	virtual void init(CMesh* mesh) const = 0;
	virtual void init() = 0;
	virtual void bindMaterial(CMesh* mesh, CShader* shader) const = 0;
	virtual void unbindMaterial(CMesh* mesh) const = 0;
	virtual void render(CMesh* mesh, CShader* shader) const = 0;
	virtual void render() = 0;
	virtual void clean(CMesh* mesh) const = 0;
	virtual void createShader(const string& vertexShader, const string& pixelShader) = 0;
};
#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <GL/glew.h> 
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "../CGraphicDriver.h"
#include "../../scene/CMesh.h"

using namespace std;

class CGLDriver : public CGraphicDriver
{
public:

	CGLDriver();
	~CGLDriver();

	void init(CMesh *mesh) const override;
	void bindMaterial(CMesh *mesh, CShader *shader) const override;
	void unbindMaterial(CMesh *mesh) const override;
	void render(CMesh *mesh, CShader *shader) const override;
	void clean(CMesh *mesh) const override;
	void createShader(const string &vertexShader, const string &pixelShader) override;
	CShader* getShader() const;
};
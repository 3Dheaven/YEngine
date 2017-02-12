#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <GL/glew.h> 
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "../image/CTexture.h"

using namespace std;

class CMaterial
{
	public:
		std::string mMaterialName;

		glm::vec3 mAmbientColor;
		glm::vec3 mDiffuseColor;
		glm::vec3 mSpecularColor;
		glm::vec3 mEmissiveColor;
		glm::vec3 mTransparentColor;

		float mAlpha;
		float mShineness;
		float mRefractionIndex;

		int mIlluminationModel;

		CTexture* mTextureAmbient;
		CTexture* mTextureDiffuse;
		CTexture* mTextureSpecular;
		CTexture* mTextureEmissive;
		CTexture* mTextureOpacity;
		CTexture* mTextureBump;
		CTexture* mTextureNormal;
		CTexture* mTextureDisp;

	public:
		CMaterial(std::string name);
		~CMaterial();
};


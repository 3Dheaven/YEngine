#include "CMaterial.h"

CMaterial::CMaterial(std::string name) :
	mMaterialName(name),
	mDiffuseColor(glm::vec3(0.5, 0.5, 0.5)),
	mAlpha(1.0),
	mShineness(0.0),
	mIlluminationModel(1),
	mRefractionIndex(1.0),
	mTransparentColor(glm::vec3(1.0))
{
	mTextureAmbient = NULL;
	mTextureDiffuse = NULL;
	mTextureSpecular = NULL;
	mTextureEmissive = NULL;
	mTextureOpacity = NULL;
	mTextureBump = NULL;
	mTextureNormal = NULL;
	mTextureDisp = NULL;
}

CMaterial::~CMaterial()
{
	if (mTextureAmbient != NULL)
		delete mTextureAmbient;

	if (mTextureDiffuse != NULL)
		delete mTextureDiffuse;

	if (mTextureSpecular != NULL)
		delete mTextureSpecular;

	if (mTextureEmissive != NULL)
		delete mTextureEmissive;

	if (mTextureOpacity != NULL)
		delete mTextureOpacity;

	if (mTextureBump != NULL)
		delete mTextureBump;

	if (mTextureNormal != NULL)
		delete mTextureNormal;

	if (mTextureDisp != NULL)
		delete mTextureDisp;
}

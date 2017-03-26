#include "CMaterial.h"

CMaterial::CMaterial()
{
	mTextureAmbient = NULL;
	mTextureDiffuse = NULL;
	mTextureSpecular = NULL;
	mTextureEmissive = NULL;
	mTextureOpacity = NULL;
	mTextureBump = NULL;
	mTextureNormal = NULL;
	mTextureDisp = NULL;
	mMaterialName = "";
}

CMaterial::CMaterial(const aiMaterial* mtl, std::string dir) :
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

	aiString str;

	if (AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, 0, &str))
	{
		string filename = string(str.C_Str());
		string filepath = dir + '/' + filename;
		mTextureDiffuse = new CTexture(filepath.c_str());
	}

	if (AI_SUCCESS == mtl->GetTexture(aiTextureType_HEIGHT, 0, &str))
	{
		string filename = string(str.C_Str());
		string filepath = dir + '/' + filename;
		mTextureNormal = new CTexture(filepath.c_str());
	}

	if (AI_SUCCESS == mtl->GetTexture(aiTextureType_SPECULAR, 0, &str))
	{
		string filename = string(str.C_Str());
		string filepath = dir + '/' + filename;
		mTextureSpecular = new CTexture(filepath.c_str());
	}

	aiColor4D ka, kd, ks;
	aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ka);
	aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &kd);
	aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &ks);
	mAmbientColor = glm::vec3(ka.r, ka.g, ka.b);
	mDiffuseColor = glm::vec3(kd.r, kd.g, kd.b);
	mSpecularColor = glm::vec3(ks.r, ks.g, ks.b);
};

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

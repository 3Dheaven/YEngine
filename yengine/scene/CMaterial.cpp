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

}

CMaterial::~CMaterial()
{
	
}

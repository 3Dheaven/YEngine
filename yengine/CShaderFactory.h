#ifndef CSHADERFACTORY_H
#define CSHADERFACTORY_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

#include "CShaderProgram.h"

class CShaderFactory
{
	public:
		CShaderFactory()
		{
		}

		~CShaderFactory()
		{
			if (mShader != nullptr)
			{
				delete mShader;
				mShader = nullptr;
			}
		}

		CShaderProgram *getProgram() const
		{
			return mShader;
		}

	protected:
		CShaderProgram *mShader;
};

#endif
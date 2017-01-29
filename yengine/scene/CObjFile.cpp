#include "CObjFile.h"

CObjectFile::CObjectFile(CModel *model)
{

	mModel = model;
	std::size_t found = std::string(mModel->mFilePath).find_last_of("/\\");
	dirPath = std::string(mModel->mFilePath).substr(0, found);
}

CObjectFile::CObjectFile()
{

}

CObjectFile::~CObjectFile()
{

}

std::string 
CObjectFile::getComment(std::string& buffer)
{
	return buffer.substr(buffer.find_last_of("#") + 1);
}

glm::vec3 
CObjectFile::getVector3(std::string& buffer)
{
	std::istringstream data(buffer);
	double x, y, z;
	data >> x; data >> y; data >> z;
	return glm::vec3(x, y, z);
}

glm::vec2
CObjectFile::getVector2(std::string& buffer)
{
	std::istringstream data(buffer);
	double x, y;
	data >> x; data >> y;
	return glm::vec2(x, y);
}

float
CObjectFile::getFloat(std::string& buffer)
{
	std::istringstream data(buffer);
	double x;
	data >> x; 
	return x;
}

void 
CObjectFile::getFace(std::string& buffer, std::vector<unsigned int> &v, std::vector<unsigned int> &vn, std::vector<unsigned int> &vt)
{
	const char* ch = buffer.c_str();
	
	std::string data = buffer;// .substr(buffer.find(" ") + 1);

	size_t pos = 0;
	std::vector<std::string> tokens;
	while ((pos = data.find(" ")) != std::string::npos) 
	{
		tokens.push_back(data.substr(0, pos));
		data.erase(0, pos + 1);
	}

	if (data.length())
	{
		tokens.push_back(data);
	}

	// Check if triangles or quads
	if (tokens.size() == 3)
	{
		unsigned int vv[3], vvn[3], vvt[3];

		if (mModel->mCurrentMesh->mHasNormals && mModel->mCurrentMesh->mHasTexcoords)
		{
			sscanf(ch, "%i/%i/%i %i/%i/%i %i/%i/%i", &vv[0], &vvt[0], &vvn[0], &vv[1], &vvt[1], &vvn[1], &vv[2], &vvt[2], &vvn[2]);
		}
		else if (mModel->mCurrentMesh->mHasNormals && !mModel->mCurrentMesh->mHasTexcoords)
		{
			sscanf(ch, "%i//%i %i//%i %i//%i", &vv[0], &vvn[0], &vv[1], &vvn[1], &vv[2], &vvn[2]);
		}
		else if (!mModel->mCurrentMesh->mHasNormals && mModel->mCurrentMesh->mHasTexcoords)
		{
			sscanf(ch, "%i/%i %i/%i %i/%i", &vv[0], &vvt[0], &vv[1], &vvt[1], &vv[2], &vvt[2]);
		}
		else if (!mModel->mCurrentMesh->mHasNormals && !mModel->mCurrentMesh->mHasTexcoords)
		{
			sscanf(ch, "%i %i %i", &vv[0], &vv[1], &vv[2]);
		}

		v.push_back(vv[0]);
		v.push_back(vv[1]);
		v.push_back(vv[2]);

		vn.push_back(vvn[0]);
		vn.push_back(vvn[1]);
		vn.push_back(vvn[2]);

		vt.push_back(vvt[0]);
		vt.push_back(vvt[1]);
		vt.push_back(vvt[2]);
	}
	else if (tokens.size() == 4)
	{
		/*
		http://stackoverflow.com/questions/23723993/converting-quadriladerals-in-an-obj-file-into-triangles

		3-------2
		|      /|
		|    /  |
		|  /    |
		|/      |
		0-------1
		*/

		unsigned int vv[4], vvn[4], vvt[4];

		if (mModel->mCurrentMesh->mHasNormals && mModel->mCurrentMesh->mHasTexcoords)
		{
			sscanf(ch, "%i/%i/%i %i/%i/%i %i/%i/%i %i/%i/%i", &vv[0], &vvt[0], &vvn[0], &vv[1], &vvt[1], &vvn[1], &vv[2], &vvt[2], &vvn[2], &vv[3], &vvt[3], &vvn[3]);
		}
		else if (mModel->mCurrentMesh->mHasNormals && !mModel->mCurrentMesh->mHasTexcoords)
		{
			sscanf(ch, "%i//%i %i//%i %i//%i %i//%i", &vv[0], &vvn[0], &vv[1], &vvn[1], &vv[2], &vvn[2], &vv[3], &vvn[3]);
		}
		else if (!mModel->mCurrentMesh->mHasNormals && mModel->mCurrentMesh->mHasTexcoords)
		{
			sscanf(ch, "%i/%i %i/%i %i/%i %i/%i", &vv[0], &vvt[0], &vv[1], &vvt[1], &vv[2], &vvt[2], &vv[3], &vvt[3]);
		}
		else if (!mModel->mCurrentMesh->mHasNormals && !mModel->mCurrentMesh->mHasTexcoords)
		{
			sscanf(ch, "%i %i %i %i", &vv[0], &vv[1], &vv[2], &vv[3]);
		}

		// (0,1,2)
		v.push_back(vv[0]);
		v.push_back(vv[1]);
		v.push_back(vv[2]);
		
		vn.push_back(vvn[0]);
		vn.push_back(vvn[1]);
		vn.push_back(vvn[2]);
		
		vt.push_back(vvt[0]);
		vt.push_back(vvt[1]);
		vt.push_back(vvt[2]);
		
		// (0,2,3)
		v.push_back(vv[0]);
		v.push_back(vv[2]);
		v.push_back(vv[3]);

		vn.push_back(vvn[0]);
		vn.push_back(vvn[2]);
		vn.push_back(vvn[3]);

		vt.push_back(vvt[0]);
		vt.push_back(vvt[2]);
		vt.push_back(vvt[3]);
	}

}

void
CObjectFile::finalizeModel()
{
	// Indexing

	for (auto o : mModel->mObjects)
	{
		for (auto m : o->mMeshes)
		{
			for (unsigned int i = 0; i < mModel->mMeshes[m]->vertexIndices.size(); i++)
			{
				unsigned int vertexIndex = mModel->mMeshes[m]->vertexIndices[i];
				glm::vec3 vertex = mModel->mVertices[vertexIndex - 1];
				mModel->mMeshes[m]->vertices.push_back(vertex);
			}

			if (mModel->mMeshes[m]->mHasTexcoords)
			{
				// For each uv of each triangle
				for (unsigned int i = 0; i < mModel->mMeshes[m]->uvIndices.size(); i++)
				{
					unsigned int uvIndex = mModel->mMeshes[m]->uvIndices[i];
					glm::vec2 uv = mModel->mTexcoords[uvIndex - 1];
					mModel->mMeshes[m]->uvs.push_back(uv);
				}
			}

			if (mModel->mMeshes[m]->mHasNormals)
			{
				// For each normal of each triangle
				for (unsigned int i = 0; i < mModel->mMeshes[m]->normalIndices.size(); i++)
				{
					unsigned int normalIndex = mModel->mMeshes[m]->normalIndices[i];
					glm::vec3 normal = mModel->mNormals[normalIndex - 1];
					mModel->mMeshes[m]->normals.push_back(normal);
				}
			}

		}
	}
}

void 
CObjectFile::getTexture(std::string& buffer)
{
	// Check if texture has any options
	auto found = buffer.find("-");
	if (found == std::string::npos)
	{
		if (!buffer.compare("map_Kd"))
		{
			// Diffuse texture
			mModel->mCurrentMaterial->mTextureDiffuse = buffer.substr(buffer.find(" ") + 1);
		}
		else if (!buffer.compare("map_Ka"))
		{
			// Ambient texture
			mModel->mCurrentMaterial->mTextureAmbient = buffer.substr(buffer.find(" ") + 1);
		}
		else if (!buffer.compare("map_Ks"))
		{
			// Specular texture
			mModel->mCurrentMaterial->mTextureSpecular = buffer.substr(buffer.find(" ") + 1);
		}
		else if (!buffer.compare("map_d"))
		{
			// Opacity texture
			mModel->mCurrentMaterial->mTextureOpacity = buffer.substr(buffer.find(" ") + 1);
		}
		else if (!buffer.compare("map_emissive"))
		{
			// Emissive texture
			mModel->mCurrentMaterial->mTextureEmissive = buffer.substr(buffer.find(" ") + 1);
		}
		else if (!buffer.compare("map_Ke"))
		{
			// Emissive texture
			mModel->mCurrentMaterial->mTextureEmissive = buffer.substr(buffer.find(" ") + 1);
		}
		else if ((!buffer.compare("map_bump")) || (!buffer.compare("map_Bump")) || (!buffer.compare("bump")))
		{
			// Bump texture
			mModel->mCurrentMaterial->mTextureBump = buffer.substr(buffer.find(" ") + 1);
		}
		else if (!buffer.compare("map_Kn"))
		{
			// Normal map
			mModel->mCurrentMaterial->mTextureNormal = buffer.substr(buffer.find(" ") + 1);
		}
		else if (!buffer.compare("refl"))
		{
			// Reflection texture(s)
			return;
		}
		else if (!buffer.compare("disp"))
		{
			// Displacement texture
			mModel->mCurrentMaterial->mTextureDisp = buffer.substr(buffer.find(" ") + 1);
		}
		else if (!buffer.compare("map_ns"))
		{
			// Specularity scaling (glossiness)
			mModel->mCurrentMaterial->mTextureSpecular = buffer.substr(buffer.find(" ") + 1);
		}
		else
		{
			std::cout << "Texture type not handled" << std::endl;
			return;
		}
	}
	/*
	// Textures can have options ...
	bool clamp = false;
	getTextureOption(clamp, clampIndex, out);
	m_pModel->m_pCurrentMaterial->clamp[clampIndex] = clamp;

	std::string texture;
	m_DataIt = getName<DataArrayIt>(m_DataIt, m_DataItEnd, texture);
	if (NULL != out) {
		out->Set(texture);
	}*/

}

void
CObjectFile::parseMtl(std::string mtlpath)
{
	std::ifstream mtlfile(mtlpath);
	std::string buffer;
	
	while (std::getline(mtlfile, buffer))
	{
		if (!buffer.empty())
		{
			std::istringstream iss(buffer);
			auto it = buffer.begin();

			switch (*it)
			{
				case 'N':
				case 'n':
				{
					++it;
					switch (*it)
					{
						case 's':   // Specular exponent
							{
								auto v = buffer.substr(buffer.find(" ") + 1);
								mModel->mCurrentMaterial->mShineness = getFloat(v);
							}
							break;
						case 'i':   // Index Of refraction
							{
								auto v = buffer.substr(buffer.find(" ") + 1);
								mModel->mCurrentMaterial->mRefractionIndex = getFloat(v);
							}
							break;
						case 'e':   // New material
							{
								std::string mtlname = buffer.substr(buffer.find(" ") + 1);
								// Check if the new material exists in the model's material map
								std::map<std::string, CMaterial*>::iterator it = mModel->mMaterialMap.find(mtlname);

								if(mModel->mMaterialMap.end() == it)
								{
									mModel->mCurrentMaterial = new CMaterial(mtlname);
									mModel->mMaterialMap[mtlname] = mModel->mCurrentMaterial;
								}
								else 
								{
									mModel->mCurrentMaterial = (*it).second;
								}
							}
							break;
					}
				}
				break;

				case 'i':   // Illumination model
				{
					std::string v = buffer.substr(buffer.find(" ") + 1);
					mModel->mCurrentMaterial->mIlluminationModel = std::atoi(v.c_str());
				}
				break;

				case 'm':   // Texture
				case 'b':   // quick'n'dirty - for 'bump' sections
				case 'r':   // quick'n'dirty - for 'refl' sections
				{
					getTexture(buffer);
				}
				break;

				case 'k':
				case 'K':
				{
					++it;
					switch (*it)
					{
						case 'a':   // Ambient color
							{
								auto v = getVector3(buffer.substr(buffer.find(" ") + 1));
								mModel->mCurrentMaterial->mAmbientColor = v;
							}
							break;
						case 'd':   // Diffuse color
							{
								auto v = getVector3(buffer.substr(buffer.find(" ") + 1));
								mModel->mCurrentMaterial->mDiffuseColor = v;
							}
							break;
						case 's':   // Specular color
							{
								auto v = getVector3(buffer.substr(buffer.find(" ") + 1));
								mModel->mCurrentMaterial->mSpecularColor = v;
							}
							break;
						case 'e':   // Emissive color
							{
								auto v = getVector3(buffer.substr(buffer.find(" ") + 1));
								mModel->mCurrentMaterial->mEmissiveColor = v;
							}
							break;
					}
				}
				break;

				case 'T':
				{
					// Material transmission
					auto found = buffer.find("Tf");
					if (found != std::string::npos)
					{
						auto v = getVector3(buffer.substr(buffer.find(" ") + 1));
						mModel->mCurrentMaterial->mTransparentColor = v;
					}
				}
				break;

				case 'd':
				{
					// A displacement map
					auto found = buffer.find("disp");
					if (found != std::string::npos)
					{
						getTexture(buffer);
					}
					else 
					{
						// Alpha value
						auto v = getFloat(buffer.substr(buffer.find(" ") + 1));
						mModel->mCurrentMaterial->mAlpha = v;
					}
				}
				break;
			}
		}
	}
}

bool 
CObjectFile::parse()
{

	std::ifstream objfile(mModel->mFilePath);
	std::string buffer;

	while (std::getline(objfile, buffer))
	{
		if (!buffer.empty())
		{
			std::istringstream iss(buffer);
			auto it = buffer.begin();
			switch (*it)
			{
				// Vertex datas
				case 'v':
				{
					++it;
					switch (*it)
					{
					case ' ':
					case '\t':
					{
						auto v = getVector3(buffer.substr(2));
						mModel->mVertices.push_back(v);
					}
					break;

					case 'n':
					{
						auto n = getVector3(buffer.substr(3));
						mModel->mNormals.push_back(n);
					}
					break;

					case 't':
					{
						auto t = getVector2(buffer.substr(3));
						mModel->mTexcoords.push_back(t);
					}
					break;
					}
				}
				break;

				// MTL file
				case 'm':
				{	
					std::string mtlib = "mtllib";
					std::size_t found = buffer.find(mtlib);
					if (found != std::string::npos)
					{
						auto mtlpath = buffer.substr(found + mtlib.length() + 1);
						parseMtl(dirPath + mtlpath);
					}
				}
				break;

				// Point
				case 'p': 
				{

				}	
				break;

				// Line
				case 'l':
				{

				}
				break;

				// Face
				case 'f':
				{
					if (!mModel->mCurrentMesh->mHasNormals)
					{
						if (mModel->mNormals.size()) 
						{
							mModel->mCurrentMesh->mHasNormals = true;
						}
					}

					if (!mModel->mCurrentMesh->mHasTexcoords)
					{
						if (mModel->mTexcoords.size())
						{
							mModel->mCurrentMesh->mHasTexcoords = true;
						}
					}

					std::vector<unsigned int> v, vn, vt;
					getFace(buffer.substr(2), v, vn, vt);

					for (int i = 0; i < v.size(); i++)
					{
						mModel->mCurrentMesh->vertexIndices.push_back(v[i]);

						if (mModel->mTexcoords.size())
						{
							mModel->mCurrentMesh->uvIndices.push_back(vt[i]);

							if (!mModel->mCurrentMesh->mHasTexcoords)
							{
								mModel->mCurrentMesh->mHasTexcoords = true;
							}
						}

						if (mModel->mNormals.size())
						{
							mModel->mCurrentMesh->normalIndices.push_back(vn[i]);

							if (!mModel->mCurrentMesh->mHasNormals)
							{
								mModel->mCurrentMesh->mHasNormals = true;
							}
						}
					}
				}
				break;

				// Object
				case 'o':
				{
					auto objectName = buffer.substr(buffer.find(" ") + 1);

					mModel->mCurrentObject = NULL;

					for (std::vector<CObject*>::const_iterator it = mModel->mObjects.begin();
						it != mModel->mObjects.end(); ++it)
					{
						if ((*it)->mName == objectName)
						{
							mModel->mCurrentObject = *it;
							break;
						}
					}

					if (NULL == mModel->mCurrentObject) 
					{
						mModel->mCurrentObject = new CObject(objectName);
						mModel->mObjects.push_back(mModel->mCurrentObject);
						mModel->mCurrentMesh = new CMesh(objectName);
						mModel->mMeshes.push_back(mModel->mCurrentMesh);
						unsigned int meshId = static_cast<unsigned int>(mModel->mMeshes.size() - 1);
						if (NULL != mModel->mCurrentObject)
						{
							mModel->mCurrentObject->mMeshes.push_back(meshId);
						}
					}
				}
				break;

				// Group Name
				case 'g':
				{
					if (buffer.length() > 2)
					{
						auto name = buffer.substr(buffer.find("g") + 2);
						mModel->mCurrentObject = NULL;

						for (std::vector<CObject*>::const_iterator it = mModel->mObjects.begin();
							it != mModel->mObjects.end(); ++it)
						{
							if ((*it)->mName == name)
							{
								mModel->mCurrentObject = *it;
								break;
							}
						}

						if (NULL == mModel->mCurrentObject)
						{
							mModel->mCurrentObject = new CObject(name);
							mModel->mObjects.push_back(mModel->mCurrentObject);
							mModel->mCurrentMesh = new CMesh(name);
							mModel->mMeshes.push_back(mModel->mCurrentMesh);
							unsigned int meshId = static_cast<unsigned int>(mModel->mMeshes.size() - 1);
							if (NULL != mModel->mCurrentObject)
							{
								mModel->mCurrentObject->mMeshes.push_back(meshId);
							}
						}
					}
				}
				break;

				// Get material for current mesh
				case 'u': 
				{
					auto matname = buffer.substr(buffer.find(" ") + 1);

					// Check if the current mesh has not the same material
					if (mModel->mCurrentMesh->mMaterial == NULL || mModel->mCurrentMesh->mMaterial->mMaterialName != matname)
					{
						// Search the material to use in the list of material stored on the model
						std::map<std::string, CMaterial*>::iterator it = mModel->mMaterialMap.find(matname);

						if (it == mModel->mMaterialMap.end())
						{
							// Not found, use default material
							mModel->mCurrentMaterial = mModel->mDefaultMaterial;
							matname = mModel->mDefaultMaterial->mMaterialName;
						}
						else 
						{
							// Use found material
							mModel->mCurrentMaterial = (*it).second;
						}

						mModel->mCurrentMesh->mMaterial = mModel->mCurrentMaterial;
					}
				}
				break;

				// Group number
				case 's':
				{
					// Not implemented
				}
				break;

				// Comment
				case '#':
				{
					std::string c = getComment(buffer);
				}
				break;

				case '\r':
				{
					std::string c = getComment(buffer);
				}
				break;

			}
		}
	}

	finalizeModel();

	return true;
}


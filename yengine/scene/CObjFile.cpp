#include "CObjFile.h"

CObjectFile::CObjectFile(const char * path) : m_filePath(path)
{
	cptPos = 1;
	cptNormal = 1;
	cptUv = 1; 
	offsetPos = 1;
	offsetNormal = 1;
	offsetUv = 1;
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

void 
CObjectFile::getFace(std::string& buffer, unsigned int *v, unsigned int *vn, unsigned int *vt)
{
	const char* ch = buffer.c_str();
	
	if (m_currentMesh->hasNormals && m_currentMesh->hasTexcoords)
	{
		sscanf(ch, "%i/%i/%i %i/%i/%i %i/%i/%i", &v[0], &vt[0], &vn[0], &v[1], &vt[1], &vn[1], &v[2], &vt[2], &vn[2]);
	}
	else if (m_currentMesh->hasNormals && !m_currentMesh->hasTexcoords)
	{
		sscanf(ch, "%i//%i %i//%i %i//%i", &v[0], &vn[0], &v[1], &vn[1], &v[2], &vn[2]);
	}
	else if (!m_currentMesh->hasNormals && m_currentMesh->hasTexcoords)
	{
		sscanf(ch, "%i/%i %i/%i %i/%i", &v[0], &vt[0], &v[1], &vt[1], &v[2], &vt[2]);
	}
	else if (!m_currentMesh->hasNormals && !m_currentMesh->hasTexcoords)
	{
		sscanf(ch, "%i %i %i", &v[0], &v[1], &v[2]);
	}

}

void
CObjectFile::finalizeObject()
{
	// Indexing

	// For each vertex of each triangle
	for (unsigned int i = 0; i<m_currentMesh->vertexIndices.size(); i++)
	{
		unsigned int vertexIndex = m_currentMesh->vertexIndices[i];
		glm::vec3 vertex = m_currentMesh->temp_vertices[vertexIndex - 1];
		m_currentMesh->vertices.push_back(vertex);
	}

	if (m_currentMesh->hasTexcoords)
	{
		// For each uv of each triangle
		for (unsigned int i = 0; i < m_currentMesh->uvIndices.size(); i++)
		{
			unsigned int uvIndex = m_currentMesh->uvIndices[i];
			glm::vec2 uv = m_currentMesh->temp_uvs[uvIndex - 1];
			m_currentMesh->uvs.push_back(uv);
		}
	}

	if (m_currentMesh->hasNormals)
	{
		// For each normal of each triangle
		for (unsigned int i = 0; i < m_currentMesh->normalIndices.size(); i++)
		{
			unsigned int normalIndex = m_currentMesh->normalIndices[i];
			glm::vec3 normal = m_currentMesh->temp_normals[normalIndex - 1];
			m_currentMesh->normals.push_back(normal);
		}
	}
	m_currentMesh->isFinalized = true;
}

bool
CObjectFile::createObject(std::string& buffer)
{
	if (m_currentMesh->hasDatas)
	{

		finalizeObject();
		return true;
	}
	return false;
}

bool 
CObjectFile::parse(std::vector<std::unique_ptr<CMesh>>& meshes)
{
	std::unique_ptr<CMesh> mesh(new CMesh);
	m_currentMesh = std::move(mesh);

	std::ifstream objfile(m_filePath);

	std::string buffer;

	while (std::getline(objfile, buffer))
	{
		std::istringstream iss(buffer);
		auto it = buffer.begin();
		switch (*it)
		{
			// Vertex datas
			case 'v' :

				if (!m_currentMesh->hasDatas)
				{
					m_currentMesh->hasDatas = true;
				}

				++it;
				switch (*it)
				{
					case ' ':
					case '\t':
						{
							cptPos++;
							auto v = getVector3(buffer.substr(2));
							m_currentMesh->temp_vertices.push_back(v);
						}
						break;

					case 'n' : 
						{
							cptNormal++;
							if (!m_currentMesh->hasNormals)
							{
								m_currentMesh->hasNormals = true;
							}

							auto n = getVector3(buffer.substr(3));
							m_currentMesh->temp_normals.push_back(n);
						}
						break;
					
					case 't':
						{
							cptUv++;
							if (!m_currentMesh->hasTexcoords)
							{
								m_currentMesh->hasTexcoords = true;
							}

							auto t = getVector2(buffer.substr(3));
							m_currentMesh->temp_uvs.push_back(t);
						}
						break;
				}

				break;

			case 'm':
				
				break;
			
			// Face
			case 'f':
				{
					unsigned int v[3], vn[3], vt[3];
					getFace(buffer.substr(2), v, vn, vt);

					m_currentMesh->vertexIndices.push_back(v[0]);
					m_currentMesh->vertexIndices.push_back(v[1]);
					m_currentMesh->vertexIndices.push_back(v[2]);

					if (m_currentMesh->hasTexcoords)
					{
						m_currentMesh->uvIndices.push_back(vt[0]);
						m_currentMesh->uvIndices.push_back(vt[1]);
						m_currentMesh->uvIndices.push_back(vt[2]);
					}

					if (m_currentMesh->hasNormals)
					{
						m_currentMesh->normalIndices.push_back(vn[0]);
						m_currentMesh->normalIndices.push_back(vn[1]);
						m_currentMesh->normalIndices.push_back(vn[2]);
					}
				}
				break;
			
			// Object
			case 'o':
				{
					if (createObject(buffer))
					{
						meshes.push_back(std::move(m_currentMesh)); 
						std::unique_ptr<CMesh> mesh(new CMesh);
						m_currentMesh = std::move(mesh);
					}
					m_currentMesh->m_name = buffer.substr(2);
					offsetPos = cptPos;
					offsetNormal = cptNormal;
					offsetUv = cptUv;
				}
				break;
			
			// Group
			case 'g':

				break;

			// Comment
			case '#':
				std::string c = getComment(buffer);
				break;
		}
	}

	if (!m_currentMesh->isFinalized)
	{
		/*offsetPos = cptPos;
		offsetNormal = cptNormal;
		offsetUv = cptUv;*/
		finalizeObject();
		meshes.push_back(std::move(m_currentMesh));
	}

	return true;
}


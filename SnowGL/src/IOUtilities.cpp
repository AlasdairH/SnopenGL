#include "PCH.h"
#include "IOUtilities.h"

namespace SnowGL
{
	std::string IOUtilities::loadText(std::string _filepath)
	{
		{
			std::ifstream inFile;
			//open the input file
			inFile.open(_filepath);

			if ((inFile.rdstate() & std::ifstream::failbit) != 0)
			{
				CONSOLE_ERROR("Failed to Open File: " << _filepath);
				return std::string("ERROR");
			}

			std::stringstream strStream;
			// read the file
			strStream << inFile.rdbuf();
			// store the string
			std::string str = strStream.str();

			CONSOLE_MESSAGE("Loaded File: " << _filepath);
			return str;
		}
	}

	void IOUtilities::loadMesh(Mesh &_mesh, const std::string & _filepath)
	{
		// TODO: Timer
		//Utilities::Timer timer;
		_mesh.vertices.clear();
		_mesh.indices.clear();
		_mesh.layout.reset();

		int parsedLines = 0;

		std::string objFile = loadText(_filepath);

		unsigned int cursor = 0;

		std::unordered_map<int, glm::vec3> verticesMap;
		std::unordered_map<int, glm::vec3> normals;
		std::unordered_map<int, glm::vec2> textureCoords;

		for (unsigned int i = 0; i < objFile.size(); ++i)
		{
			// find the index of the next end of line char, starting from i
			unsigned int eol = objFile.find("\n", i);
			// get a substring from i, for end of line - i chars
			std::string line = objFile.substr(i, eol - i);



			// vertex line
			if (line.find("v ") != std::string::npos)
			{
				// TODO: Split
				size_t firstSpaceIndex = line.find(" ", 0);
				size_t secondSpaceIndex = line.find(" ", firstSpaceIndex + 1);
				size_t thirdSpaceIndex = line.find(" ", secondSpaceIndex + 1);
				size_t eolIndex = line.find("\n", i);

				std::string xVal = line.substr(firstSpaceIndex + 1, secondSpaceIndex - firstSpaceIndex - 1);
				std::string yVal = line.substr(secondSpaceIndex + 1, thirdSpaceIndex - secondSpaceIndex - 1);
				std::string zVal = line.substr(thirdSpaceIndex + 1, eolIndex - thirdSpaceIndex - 1);

				float x = std::stof(xVal);
				float y = std::stof(yVal);
				float z = std::stof(zVal);

				// check bounding box values
				if (x < _mesh.minBound.x)
					_mesh.minBound.x = x;
				if (y < _mesh.minBound.y)
					_mesh.minBound.y = y;
				if (z < _mesh.minBound.z)
					_mesh.minBound.z = z;

				if (x > _mesh.maxBound.x)
					_mesh.maxBound.x = x;
				if (y > _mesh.maxBound.y)
					_mesh.maxBound.y = y;
				if (z > _mesh.maxBound.z)
					_mesh.maxBound.z = z;

				verticesMap[(int)verticesMap.size() + 1] = glm::vec3(x, y, z);
				++parsedLines;
			}

			// texture coord line
			if (line.find("vt ") != std::string::npos)
			{
				// TODO: Split
				size_t firstSpaceIndex = line.find(" ", 0);
				size_t secondSpaceIndex = line.find(" ", firstSpaceIndex + 1);
				size_t eolIndex = line.find("\n", i);

				std::string xVal = line.substr(firstSpaceIndex + 1, secondSpaceIndex - firstSpaceIndex - 1);
				std::string yVal = line.substr(secondSpaceIndex + 1, eolIndex - secondSpaceIndex - 1);

				textureCoords[(int)textureCoords.size() + 1] = glm::vec2(std::stof(xVal), std::stof(yVal));
				++parsedLines;
			}

			// normal line
			if (line.find("vn ") != std::string::npos)
			{
				// TODO: Split
				unsigned int firstSpaceIndex = line.find(" ", 0);
				unsigned int secondSpaceIndex = line.find(" ", firstSpaceIndex + 1);
				unsigned int thirdSpaceIndex = line.find(" ", secondSpaceIndex + 1);
				unsigned int eolIndex = line.find("\n", i);

				std::string xVal = line.substr(firstSpaceIndex + 1, secondSpaceIndex - firstSpaceIndex - 1);
				std::string yVal = line.substr(secondSpaceIndex + 1, thirdSpaceIndex - secondSpaceIndex - 1);
				std::string zVal = line.substr(thirdSpaceIndex + 1, eolIndex - thirdSpaceIndex - 1);

				normals[(int)normals.size() + 1] = glm::vec3(std::stof(xVal), std::stof(yVal), std::stof(zVal));
				++parsedLines;
			}


			// face line
			if (line.find("f ") != std::string::npos)
			{
				// split the face line
				std::vector<std::string> splitLine = split(line, ' ');
				// remove the "f"
				splitLine.erase(splitLine.begin());

				// create a vector for the extracted indices
				std::vector<int> vIncides;
				std::vector<int> vtIncides;
				std::vector<int> vnIncides;
				// for each split in the face components
				for (unsigned int j = 0; j < splitLine.size(); ++j)
				{
					// split the face components by the slash delimiter
					std::vector<std::string> splitBlock = split(splitLine[j], '/');
					// get the vertex index
					vIncides.push_back(std::stoi(splitBlock[0]));
					// get the vertex texture index
					vtIncides.push_back(std::stoi(splitBlock[1]));
					// get the vertex normal index
					vnIncides.push_back(std::stoi(splitBlock[2]));
				}

				// for the three vertices of a face
				for (unsigned int j = 0; j < 3; ++j)
				{
					Vertex vert;
					// grab the split data for the current vertex and store it together 
					vert.position = glm::vec3(verticesMap[vIncides[j]]);
					vert.textureCoordinate = glm::vec2(textureCoords[vtIncides[j]]);
					vert.normal = glm::vec3(normals[vnIncides[j]]);

					// see if the vertex already exists in the vector of vertices
					if (std::find(_mesh.vertices.begin(), _mesh.vertices.end(), vert) != _mesh.vertices.end())
					{
						// vertex already exists
						// get the position in the vector of vertices
						unsigned int pos = std::distance(_mesh.vertices.begin(), std::find(_mesh.vertices.begin(), _mesh.vertices.end(), vert));
						// push that position back as an index for the indices vector
						_mesh.indices.push_back(pos);
					}
					else
					{
						// if this is a new vertex
						// push the vertex back onto the vector
						_mesh.vertices.push_back(vert);
						// push back the index for that vector to the indices
						_mesh.indices.push_back(_mesh.vertices.size() - 1);
					}
				}
				++parsedLines;
			}

			// move the next start point to the end of the last line
			i = eol;
		}
		// push 3 floats for the position (x, y, z)
		_mesh.layout.push<float>(3);
		// push 2 floats for the texture coords (x, y)
		_mesh.layout.push<float>(2);
		// push 3 floats for the normals (x, y, z)
		_mesh.layout.push<float>(3);
		// return the mesh

		//CONSOLE_WARNING("Loaded " << _mesh.vertices.size() << " vertices | " << parsedLines << " lines parsed in " << timer.getDuration() << "s");
		CONSOLE_WARNING("Loaded " << _mesh.vertices.size() << " vertices | " << parsedLines << " lines parsed");
		/*
		m_isLoaded = true;

		if (!m_loadedToGPU)
		{
			m_VAO = std::make_shared<Graphics::VertexArray>();
			m_VBO = std::make_shared<Graphics::VertexBuffer>(Graphics::CNG_BUFFER_ARRAY);
			m_IBO = std::make_shared<Graphics::VertexBuffer>(Graphics::CNG_BUFFER_ELEMENT_ARRAY);

			m_VAO->addBuffer(*m_VBO, layout);

			m_VBO->loadData(vertices.data(), 0, layout.getStride() * vertices.size());
			m_IBO->loadData(indices.data(), indices.size(), sizeof(unsigned int) * indices.size());

			m_loadedToGPU = true;
		}
		*/
	}

	std::vector<std::string> IOUtilities::split(const std::string & _s, char _delimiter)
	{
		std::vector<std::string> tokens;
		std::string token;
		std::istringstream tokenStream(_s);
		while (std::getline(tokenStream, token, _delimiter))
		{
			tokens.push_back(token);
		}
		return tokens;
	}
}
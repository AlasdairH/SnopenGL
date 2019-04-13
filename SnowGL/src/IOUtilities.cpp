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
		Timer timer;
		_mesh.vertices.clear();
		_mesh.indices.clear();
		_mesh.layout.reset();

		int parsedLines = 0;

		std::string objFile = loadText(_filepath);
		if (objFile == "ERROR")
		{
			CONSOLE_ERROR("Unable to load .obj file");
			return;
		}

		CONSOLE_MESSAGE("Parsing mesh file: " << _filepath);

		unsigned int cursor = 0;

		std::unordered_map<int, glm::vec3> verticesMap;
		std::unordered_map<int, glm::vec3> normals;
		std::unordered_map<int, glm::vec2> textureCoords;

		std::vector<std::string> lines = split(objFile, '\n');

		for (std::string &line : lines)
		{
			// vertex line
			if (line.find("v ") != std::string::npos)
			{
				// split the face line
				std::vector<std::string> splitLine = split(line, ' ');
				// remove the "v"
				splitLine.erase(splitLine.begin());
				float x = std::stof(splitLine[0]);
				float y = std::stof(splitLine[1]);
				float z = std::stof(splitLine[2]);

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
				// split the face line
				std::vector<std::string> splitLine = split(line, ' ');
				// remove the "vt"
				splitLine.erase(splitLine.begin());

				float xVal = std::stof(splitLine[0]);
				float yVal = std::stof(splitLine[1]);

				textureCoords[(int)textureCoords.size() + 1] = glm::vec2(xVal, yVal);
				++parsedLines;
			}

			// normal line
			if (line.find("vn ") != std::string::npos)
			{
				// split the face line
				std::vector<std::string> splitLine = split(line, ' ');
				// remove the "vn"
				splitLine.erase(splitLine.begin());

				float xVal = std::stof(splitLine[0]);
				float yVal = std::stof(splitLine[1]);
				float zVal = std::stof(splitLine[2]);

				normals[(int)normals.size() + 1] = glm::vec3(xVal, yVal, zVal);
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
		}
		// push 3 floats for the position (x, y, z)
		_mesh.layout = Vertex::getLayout();
		// return the mesh

		CONSOLE_MESSAGE("Loaded " << _mesh.vertices.size() << " vertices | " << parsedLines << " lines parsed | Took " << timer.getDuration().count() << "s");
	}

	void IOUtilities::loadRenderable(Renderable &_renderable, const std::string &_filepath)
	{
		CONSOLE_MESSAGE("Loading renderable " << _filepath);

		int parsedLines = 0;
		bool filetypeVerified = false;

		std::string fileText = loadText(_filepath);

		std::shared_ptr<GPU_Mesh>		mesh			= std::make_shared<GPU_Mesh>();
		std::shared_ptr<GPU_Mesh>		mesh_collision	= std::make_shared<GPU_Mesh>();
		std::shared_ptr<ShaderProgram>	shader			= std::make_shared<ShaderProgram>();
		std::shared_ptr<Texture>		texture			= std::make_shared<Texture>();

		std::vector<std::string>		varyings;

		for (unsigned int i = 0; i < fileText.size(); ++i)
		{
			// find the index of the next end of line char, starting from i
			unsigned int eol = fileText.find("\n", i);
			// get a substring from i, for end of line - i chars
			std::string line = fileText.substr(i, eol - i);

			if (i == 0)
			{
				// vertex line
				if (line.find("FILETYPE RENDERABLE") != std::string::npos)
				{
					filetypeVerified = true;
				}
				else
				{
					CONSOLE_ERROR("Failed to load Renderable file, incorrect file type");
					return;
				}
			}

			// SHADER_VERT path
			if (line.find("SHADER_VERT") != std::string::npos)
			{
				std::vector<std::string> splitBlock = split(line, ' ');
				Shader vertShader(SHADER_VERTEX);
				vertShader.load(splitBlock[1]);
				shader->attachShader(vertShader);
			}

			// SHADER_FRAG path
			if (line.find("SHADER_FRAG") != std::string::npos)
			{
				std::vector<std::string> splitBlock = split(line, ' ');
				Shader fragShader(SHADER_FRAGMENT);
				fragShader.load(splitBlock[1]);
				shader->attachShader(fragShader);
			}

			// TEXTURE name path
			if (line.find("TEXTURE") != std::string::npos)
			{
				std::vector<std::string> splitBlock = split(line, ' ');
				texture->load(splitBlock[1], splitBlock[2]);
			}

			// MESH path
			if (line.find("MESH") != std::string::npos)
			{
				std::vector<std::string> splitBlock = split(line, ' ');
				Mesh objMesh;
				loadMesh(objMesh, splitBlock[1]);
				mesh->setMesh(objMesh);
			}			
			// MESH path
			if (line.find("MESH_COLLISION") != std::string::npos)
			{
				std::vector<std::string> splitBlock = split(line, ' ');
				Mesh objMesh;
				loadMesh(objMesh, splitBlock[1]);
				mesh->setMesh(objMesh);
			}

			// TFB_VARYING varying
			if (line.find("TFB_VARYING") != std::string::npos)
			{
				std::vector<std::string> splitBlock = split(line, ' ');
				varyings.push_back(splitBlock[1]);
			}

			// TRANSFORM pos_x pos_y pos_z rot_x rot_y rot_z scale_x scale_y scale_z 
			if (line.find("TRANSFORM") != std::string::npos)
			{
				std::vector<std::string> splitBlock = split(line, ' ');
				_renderable.transform.setPosition(glm::vec3(std::stof(splitBlock[1]), std::stof(splitBlock[2]), std::stof(splitBlock[3])));
				_renderable.transform.rotate(std::stof(splitBlock[4]), glm::vec3(1, 0, 0));
				_renderable.transform.rotate(std::stof(splitBlock[5]), glm::vec3(0, 1, 0));
				_renderable.transform.rotate(std::stof(splitBlock[6]), glm::vec3(0, 0, 1));
				_renderable.transform.scale(glm::vec3(std::stof(splitBlock[7]), std::stof(splitBlock[8]), std::stof(splitBlock[9])));
			}

			// move the next start point to the end of the last line
			i = eol;
		}

		if (varyings.size() > 0)
		{
			shader->setTransformFeedbackVarying(varyings);
		}

		shader->link();
		shader->setUniform1i("u_diffuseTexture", 0);
		texture->bind(0);

		_renderable.setMesh(mesh);
		_renderable.setShader(shader);
		_renderable.setTexture(texture);

		CONSOLE_MESSAGE("Finished loading renderable " << _filepath);
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
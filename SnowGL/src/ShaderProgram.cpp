#include "PCH.h"
#include "ShaderProgram.h"

namespace SnowGL
{
	ShaderProgram::ShaderProgram()
	{
		// create the program to store the shaders
		m_programID = glCreateProgram();
		CONSOLE_MESSAGE("Created Shader Program with ID " << m_programID);
	}

	ShaderProgram::ShaderProgram(const std::string & _vert, const std::string & _frag)
	{
		m_programID = glCreateProgram();
		CONSOLE_MESSAGE("Created Shader Program with ID " << m_programID);

		Shader vert(SHADER_VERTEX, _vert);
		Shader frag(SHADER_FRAGMENT, _frag);
		attachShader(vert);
		attachShader(frag);
		link();
	}

	void ShaderProgram::attachShader(Shader &_shader)
	{
		glAttachShader(m_programID, _shader.getShaderID());
		CONSOLE_MESSAGE("Attached shader " << _shader.getShaderID() << " to program " << m_programID);
	}

	bool ShaderProgram::link()
	{
		glLinkProgram(m_programID);
		if (!verify())
		{
			CONSOLE_ERROR("Failed to verify shader program " << m_programID);
			return false;
		}

		bind();

		// if the shader program contains the camera data uniform block then link it to binding point SHADER_BINDPOINT_CAMERA_VP
		if (getUniformBlockIndex("u_camera_data") != -1)
		{
			CONSOLE_MESSAGE("Found u_camera_data uniform block, starting link");
			// link the BasicShader Uniform block "u_camera_data" to the binding point
			linkUniformBlock("u_camera_data", SHADER_BINDPOINT_CAMERA_VP);
		}
		else
		{
			CONSOLE_MESSAGE("Unable to find u_camera_data uniform block, assuming not needed");
		}

		CONSOLE_MESSAGE("Shader Program " << m_programID << " sucessfully linked and verified");

		return true;
	}

	bool ShaderProgram::verify()
	{
		GLint result = GL_FALSE;
		int infoLogLength;

		glGetProgramiv(m_programID, GL_LINK_STATUS, &result);
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			GLchar* log = new GLchar[infoLogLength + 1];
			glGetProgramInfoLog(m_programID, infoLogLength, &infoLogLength, log);
			CONSOLE_ERROR("ERROR: Program Shader linking failed: " << log);
			delete[] log;

			return false;
		}

		CONSOLE_MESSAGE("Shader Program " << m_programID << " Successfully Verified");

		m_verified = true;
		return true;
	}

	void ShaderProgram::bind() const
	{
		if (m_verified)
		{
			glUseProgram(m_programID);
		}
		else
		{
			CONSOLE_ERROR("Bind Failed, Shader Program not Verified");
		}
	}
	void ShaderProgram::unBind() const
	{
		glUseProgram(0);
	}
	int ShaderProgram::getUniformLocation(const std::string & _name)
	{
		if (m_uniformLocationCache.find(_name) != m_uniformLocationCache.end())
			return m_uniformLocationCache[_name];

		int location = glGetUniformLocation(m_programID, _name.c_str());
		if (location == -1)
		{
			CONSOLE_WARNING("Uniform name " << _name << " does not exist!");
		}

		m_uniformLocationCache[_name] = location;

		return location;
	}

	int ShaderProgram::getAttributeLocation(const std::string & _name)
	{
		if (m_uniformLocationCache.find(_name) != m_uniformLocationCache.end())
			return m_uniformLocationCache[_name];

		int location = glGetAttribLocation(m_programID, _name.c_str());
		if (location == -1)
		{
			CONSOLE_WARNING("Attribute name " << _name << " does not exist!");
		}

		m_uniformLocationCache[_name] = location;

		return location;
	}

	void ShaderProgram::setTransformFeedbackVarying(std::vector<std::string> &_varying)
	{
		if (!m_verified)
		{
			// doing a bit of dodgy shit to convert a vector of strings to char**
			std::vector<char*> ptr_list;
			for (std::size_t i = 0; i != _varying.size(); ++i) 
			{
				ptr_list.push_back(&_varying.at(i)[0]);
			}
			char** arr = &ptr_list[0];

			glTransformFeedbackVaryings(m_programID, _varying.size(), arr, GL_INTERLEAVED_ATTRIBS);
		}
		else
		{
			CONSOLE_ERROR("Unable to set transform feedback varying, shader already verified / linked");
		}	
		return;
	}

	void ShaderProgram::setUniform1f(const std::string & _name, float _value)
	{
		bind();
		glUniform1f(getUniformLocation(_name), _value);
	}

	void ShaderProgram::setUniform3f(const std::string & _name, float _value1, float _value2, float _value3)
	{
		bind();
		glUniform3f(getUniformLocation(_name), _value1, _value2, _value3);
	}

	void ShaderProgram::setUniform3f(const std::string &_name, glm::vec3 _vector)
	{
		setUniform3f(_name, _vector.x, _vector.y, _vector.z);
	}

	void ShaderProgram::setUniform4f(const std::string &_name, float _value1, float _value2, float _value3, float _value4)
	{
		bind();
		glUniform4f(getUniformLocation(_name), _value1, _value2, _value3, _value4);
	}

	void ShaderProgram::setUniform4f(const std::string &_name, glm::vec4 _vector)
	{
		setUniform4f(_name, _vector.x, _vector.y, _vector.z, _vector.w);
	}

	void ShaderProgram::setUniform1i(const std::string & _name, int _value)
	{
		bind();
		glUniform1i(getUniformLocation(_name), _value);
	}

	void ShaderProgram::setUniformMat4f(const std::string & _name, const glm::mat4 &_value)
	{
		bind();
		glUniformMatrix4fv(getUniformLocation(_name), 1, GL_FALSE, &_value[0][0]);
	}

	void ShaderProgram::setUniformBool(const std::string & _name, const bool _value)
	{
		bind();
		setUniform1i(_name, _value);
	}

	GLuint ShaderProgram::getUniformBlockIndex(const std::string & _name)
	{
		bind();
		GLuint blockIndex = glGetUniformBlockIndex(m_programID, _name.c_str());
		if (blockIndex == -1)
		{
			CONSOLE_WARNING("Uniform block " << _name << " does not exist (" << blockIndex << ")");
		}
		return blockIndex;
	}

	void ShaderProgram::linkUniformBlock(const std::string &_blockIndex, GLuint _bindingPoint)
	{
		bind();
		glUniformBlockBinding(m_programID, getUniformBlockIndex(_blockIndex), _bindingPoint);
		CONSOLE_MESSAGE("Linked shader " << m_programID << " uniform block " << _blockIndex << " to binding point " << _bindingPoint);
	}
}

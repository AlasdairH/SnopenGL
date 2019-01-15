#include "PCH.h"
#include "Texture.h"

namespace SnowGL
{
	Texture::Texture()
		: m_textureID(0), m_localBuffer(nullptr), m_width(0), m_height(0), m_BPP(0)
	{
		m_name = "Default";
	}

	Texture::Texture(const std::string &_name, const std::string &_filepath)
		: m_textureID(0), m_filepath(_filepath), m_localBuffer(nullptr), m_width(0), m_height(0), m_BPP(0)
	{
		load(_name, _filepath);
		m_name = _name;
	}

	Texture::~Texture()
	{
		CONSOLE_MESSAGE("Destroyed Texture with ID: " << m_textureID);
		glDeleteTextures(1, &m_textureID);
	}

	void Texture::load(const std::string &_name, const std::string &_filepath)
	{
		// load the image
		stbi_set_flip_vertically_on_load(true);
		m_localBuffer = stbi_load(_filepath.c_str(), &m_width, &m_height, &m_BPP, 4);
		CONSOLE_MESSAGE("Loaded File: " << _filepath);

		// TODO: Move to Ctor to stop double gen
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		// minification filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// magnification filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// give OpenGL the data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer);
		unBind();

		// free the image now it is stored on the GPU
		if (m_localBuffer)
		{
			stbi_image_free(m_localBuffer);
		}

		CONSOLE_MESSAGE("Created Texture with ID: " << m_textureID);
	}

	void Texture::bind(GLuint _slot) const
	{
		ApplicationState &state = ApplicationState::getInstance();
		glActiveTexture(GL_TEXTURE0 + _slot);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		state.curBoundTexture = m_name;
	}
}
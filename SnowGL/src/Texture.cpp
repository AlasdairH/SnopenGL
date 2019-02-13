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

	Texture::Texture(const std::string & _name, const int _width, const int _height, TextureType _type, TexturePixelDataType _pixelDataFormat)
	{
		create(_name, _width, _height, _type, _pixelDataFormat);
	}

	Texture::~Texture()
	{
		CONSOLE_MESSAGE("Destroyed Texture with ID: " << m_textureID);
		glDeleteTextures(1, &m_textureID);
	}

	void Texture::create(const std::string & _name, const int _width, const int _height, TextureType _type, TexturePixelDataType _pixelDataFormat)
	{
		m_width = _width;
		m_height = _height;

		m_type = _type;
		m_pixelDataType = _pixelDataFormat;
		m_BPP = -1;
		m_filepath = "MANUAL";

		// delete incase it's a duplicate gen
		glDeleteTextures(1, &m_textureID);
		// generate
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		// minification filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// magnification filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// give OpenGL the data
		glTexImage2D(GL_TEXTURE_2D, 0, _type, m_width, m_height, 0, _type, _pixelDataFormat, NULL);
		unBind();
		CONSOLE_MESSAGE("Created blank Texture with ID: " << m_textureID);
	}

	void Texture::load(const std::string &_name, const std::string &_filepath)
	{
		// load the image
		stbi_set_flip_vertically_on_load(true);
		m_localBuffer = stbi_load(_filepath.c_str(), &m_width, &m_height, &m_BPP, 4);
		CONSOLE_MESSAGE("Loaded File: " << _filepath);

		m_type = TEXTURE_RGBA;
		m_pixelDataType = TEXTURE_PIXEL_UBYTE;

		glDeleteTextures(1, &m_textureID);
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		// minification filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// magnification filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glm::vec4 borderColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &borderColor[0]);

		// give OpenGL the data
		glTexImage2D(GL_TEXTURE_2D, 0, m_type, m_width, m_height, 0, m_type, m_pixelDataType, m_localBuffer);
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
		glActiveTexture(GL_TEXTURE0 + _slot);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
	}
}
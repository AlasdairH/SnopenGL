#pragma once

// cstdlib
#include <string>

// external libs
#include "GL/glew.h"
#include "external/stb_image.h"

// program
#include "PCH.h"
#include "ApplicationState.h"

namespace SnowGL
{
	enum TextureType { TEXTURE_RGB = GL_RGB, TEXTURE_RGBA = GL_RGBA, TEXTURE_DEPTH = GL_DEPTH_COMPONENT };
	enum TextureFormat { TEXTURE_2D = GL_TEXTURE_2D, TEXTURE_3D = GL_TEXTURE_3D };
	enum TexturePixelDataType { TEXTURE_PIXEL_FLOAT = GL_FLOAT, TEXTURE_PIXEL_UBYTE = GL_UNSIGNED_BYTE, TEXTURE_PIXEL_INT = GL_INT };

	/*! @class Texture
	*	@brief An abstraction of an OpenGL Texture
	*
	*	This class handles the loading and assignment of an OpenGL texture.
	*/
	class Texture
	{
	public:
		/** @brief Texture Ctor
		*
		*	Loads an image file using stb_image and loads it into OpenGL
		*/
		Texture();
		/** @brief Texture Ctor
		*	@param _name A name to assign the texture
		*	@param _filepath The path to the image to be loaded
		*
		*	Loads an image file using stb_image and loads it into OpenGL
		*/
		Texture(const std::string &_name, const std::string &_filepath);
		/** @brief Creates a manual texture through the constructor
		*	@param _name The name of the texure
		*	@param _width Width
		*	@param _height Height
		*	@param _type The texture type, eg RGB, RGBA
		*	@param _pixelDataFormat The format of the pixel data, eg Float, Unsigned Byte
		*
		*	Creates an OpenGL texture
		*/
		Texture(const std::string &_name, const int _width, const int _height, TextureType _type, TexturePixelDataType _pixelDataFormat);
		/** @brief Texture Dtor
		*
		*	Frees the OpenGL Texture
		*/
		~Texture();

		/** @brief Creates a manual texture
		*	@param _name The name of the texure
		*	@param _width Width
		*	@param _height Height
		*	@param _type The texture type, eg RGB, RGBA
		*	@param _pixelDataFormat The format of the pixel data, eg Float, Unsigned Byte
		*
		*	Creates an OpenGL texture
		*/
		void create(const std::string &_name, const int _width, const int _height, TextureType _type, TexturePixelDataType _pixelDataFormat);

		/** @brief Load Texture
		*	@param _name name to assign the texture
		*	@param _filepath The path to the image to be loaded
		*
		*	Loads an image file using stb_image and loads it into OpenGL
		*/
		void load(const std::string &_name, const std::string &_filepath);

		/** @brief Binds the Texture
		*	@param _slot The texture slot to bind the texture to
		*
		*	Binds the texture to a specific slot in the OpenGL texture slot array
		*/
		void bind(GLuint _slot = 0) const;
		/** @brief Unbinds the Texture
		*
		*	Unbinds the texture.
		*/
		void unBind() { glBindTexture(GL_TEXTURE_2D, 0); }

		/** @brief Gets the with of the texture
		*	@return The texture width
		*
		*	Gets the with of the texture
		*/
		inline int getWidth() const { return m_width; }
		/** @brief Gets the with of the texture
		*	@return The texture height
		*
		*	Gets the height of the texture
		*/
		inline int getHeight() const { return m_height; }

		/** @brief Gets OpenGL ID of the texture
		*	@return The OpenGL ID
		*
		*	Gets OpenGL ID of the texture
		*/
		inline GLuint getGLID() { return m_textureID; }

	protected:
		GLuint					m_textureID = 0;			/**< The OpenGL Texture ID */
		std::string				m_filepath;			/**< The filepath to the texture */
		unsigned char			*m_localBuffer;			/**< The local buffer for the loaded image */

		int						m_width;				/**< The width */
		int						m_height;				/**< The height */
		int						m_BPP;					/**< The BPP */
		TextureType				m_type;					/**< The texture type */
		TexturePixelDataType	m_pixelDataType;		/**< The data type of the pixels */

		std::string				m_name;					/**< The name of the texture */
	};
}
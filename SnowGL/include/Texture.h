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
		*	@param _filepath The path to the image to be loaded
		*
		*	Loads an image file using stb_image and loads it into OpenGL
		*/
		Texture(const std::string &_name, const std::string &_filepath);
		/** @brief Texture Dtor
		*
		*	Frees the OpenGL Texture
		*/
		~Texture();

		/** @brief Load Texture
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

	protected:
		GLuint			 m_textureID;			/**< The OpenGL Texture ID */
		std::string		 m_filepath;			/**< The filepath to the texture */
		unsigned char	*m_localBuffer;			/**< The local buffer for the loaded image */

		int				m_width;				/**< The width */
		int				m_height;				/**< The height */
		int				m_BPP;					/**< The BPP */

		std::string		m_name;					/**< The name of the texture */
	};
}
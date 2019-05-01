#pragma once

// cstdlib
#include <iostream>

// external libs
#include "GL/glew.h"

// program
#include "PCH.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ShaderProgram.h"

namespace SnowGL
{
	enum FrameBufferTextureType { FBO_TEXTURE_COLOUR = GL_COLOR_ATTACHMENT0, FBO_TEXTURE_DEPTH = GL_DEPTH_ATTACHMENT };

	/*! @class FrameBuffer
	*	@brief Abstraction of an OpenGL FrameBuffer
	*
	*	This class abstracts an OpenGL FrameBuffer for drawing to and then displaying on the screen if required.
	*/
	class FrameBuffer
	{
	public:
		/** @brief FrameBuffer Ctor
		*	@param _width The width of the FrameBuffer
		*	@param _height The height of the FrameBuffer
		*	@param _shader The shader program to use when rendering the frame buffer quad
		*
		*	Generates a FrameBuffer with the specified width and height.
		*/
		FrameBuffer(const int _width, const int _height, std::shared_ptr<ShaderProgram> _shader);
		/** @brief FrameBuffer Dtor
		*
		*	Deletes the FrameBuffer from memory along with all accompanying objects.
		*/
		~FrameBuffer();

		/** @brief Attaches a texture to the framebuffer
		*	@param _texture The texture to attach to the framebuffer
		*	@param _type The texture type
		*
		*	Attaches a texture to the framebuffer. This could for example be colour or depth.
		*/
		void attach(std::shared_ptr<Texture> _texture, FrameBufferTextureType _type);

		/** @brief Sets the colour buffer to draw into and read from
		*	@param _buffer The colour buffer to draw and read
		*
		*	When colors are written to the frame buffer, they are written into the color buffers specified by this function.
		*/
		void setColourBuffer(GLenum _buffer);

		/** @brief Binds the FrameBuffer
		*
		*	Binds the framebuffer for drawing to
		*/
		void bind() const;
		/** @brief Unbinds the FrameBuffer
		*
		*	Unbinds the FrameBuffer
		*/
		void unBind() const;

		/** @brief Unbinds the FrameBuffer
		*	@return The colour texure on the frame buffer
		*
		*	The colour texture of the frame buffer
		*/
		std::shared_ptr<Texture> getTexture();

		/** @brief Verifies the frame buffer is correctly constructed
		*
		*	Verifies the frame buffer is correctly constructed
		*/
		bool verify();

		/** @brief Draws the FrameBuffer to the fullscreen vertices
		*
		*	Unbinds the FrameBuffer
		*/
		void drawToScreen();

		/** @brief Create a Depth RenderBuffer Attachment
		*
		*	This creates a depth render buffer for sampling the depth of the drawn scene.
		*/
		void createDepthRenderBufferAttachment();

	protected:
		VertexArray						*m_arrayBuffer;				/**< The VertexArray for the full screen mesh */
		VertexBuffer					*m_vertexBuffer;			/**< The VertexBuffer for the full screen mesh */
		std::shared_ptr<ShaderProgram>	m_shaderProgram;			/**< The Shader that holds the Post Processing effects */
			
		int								m_width;					/**< The width of the FrameBuffer and Accompanying Textures */
		int								m_height;					/**< The height of the FrameBuffer and Accompanying Textures */

		GLuint							m_frameBufferID;			/**< The OpenGL ID of the FrameBuffer */
			
		std::shared_ptr<Texture>		m_texture;					/**< The OpenGL ID of the FrameBuffer Texture Attachment */
		GLuint							m_depthRenderBufferID;		/**< The OpenGL ID of the FrameBuffer Depth RenderBuffer Attachment */
	};
}

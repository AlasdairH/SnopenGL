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

// TODO: Seperate the FrameBuffer and the object it draws to

namespace SnowGL
{
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
		*
		*	Generates a FrameBuffer with the specified width and height.
		*/
		FrameBuffer(const int _width, const int _height);
		/** @brief FrameBuffer Dtor
		*
		*	Deletes the FrameBuffer from memory along with all accompanying objects.
		*/
		~FrameBuffer();

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

		/** @brief Draws the FrameBuffer to the fullscreen vertices
		*
		*	Unbinds the FrameBuffer
		*/
		void drawToScreen();

	protected:
		/** @brief Create a Texture Attachment
		*
		*	Creates an OpenGL Texture for the FrameBuffer to draw the colours to. This can be used anywhere in the program.
		*/
		void createTextureAttachment();
		/** @brief Create a Depth RenderBuffer Attachment
		*
		*	This creates a depth render buffer for sampling the depth of the drawn scene.
		*/
		void createDepthRenderBufferAttachment();

		VertexArray					*m_arrayBuffer;				/**< The VertexArray for the full screen mesh */
		VertexBuffer				*m_vertexBuffer;			/**< The VertexBuffer for the full screen mesh */
		ShaderProgram				*m_shaderProgram;			/**< The Shader that holds the Post Processing effects */
			
		int							m_width;					/**< The width of the FrameBuffer and Accompanying Textures */
		int							m_height;					/**< The height of the FrameBuffer and Accompanying Textures */

		GLuint						m_frameBufferID;			/**< The OpenGL ID of the FrameBuffer */
			
		GLuint						m_textureID;				/**< The OpenGL ID of the FrameBuffer Texture Attachment */
		GLuint						m_depthRenderBufferID;		/**< The OpenGL ID of the FrameBuffer Depth RenderBuffer Attachment */
	};
}

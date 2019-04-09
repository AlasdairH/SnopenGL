#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"
#include "GPU_Mesh.h"
#include "Transform.h"
#include "ShaderProgram.h"
#include "Renderable.h"
#include "FrameBuffer.h"

namespace SnowGL
{
	/*! @class Renderer
	*	@brief Renders GPU objects to the screen
	*
	*	Abstracts and provides simple utilities for an OpenGL Shader to be used by a shader program.
	*/
	class Renderer
	{
	public:
		/** @brief Renderer Ctor
		*
		*	Sets up the OpenGL functions for culling etc
		*/
		Renderer();

		/** @brief Renders an object to the screen
		*	@param _mesh The mesh to render
		*	@param _shaderProgram The shader to render the mesh with
		*	@param _transform The transform to apply to the mesh
		*
		*	Renders the GPU_Mesh with the shader program and transform. GPU_Mesh protected variables
		*	are accessible through the "friend" class.
		*/
		void render(const GPU_Mesh &_mesh, ShaderProgram &_shaderProgram, const Transform &_transform);
		/** @brief Renders an object to the screen
		*	@param _renderable The renderable to render
		*
		*	Renders the GPU_Mesh with the shader program and transform. GPU_Mesh protected variables
		*	are accessible through the "friend" class.
		*/
		void render(const Renderable &_renderable);
		/** @brief Renders an object to the screen with the shader overridden with a specified parameter
		*	@param _renderable The renderable to render
		*
		*	Renders the GPU_Mesh with the shader program and transform. GPU_Mesh protected variables
		*	are accessible through the "friend" class. The object is drawn with the provided shader instead 
		*	of the shader on the renderable.
		*/
		void renderToDepthBuffer(const Renderable &_renderable);

		/** @brief Sets whether the stencil buffer is active
		*	@param _active Flag stating whether to write to the stencil buffer or not
		*
		*	Sets whether the stencil buffer should be written to or not
		*/
		void setStencilBufferActive(bool _active);

		/** @brief Bind the renderer default framebuffer
		*
		*	Bind the renderer's default framebuffer for drawing to the screen with.
		*/
		inline void bindFrameBuffer() { m_frameBuffer->bind(); }
		/** @brief unbind the renderer default framebuffer
		*
		*	unbind the renderer's default framebuffer for drawing to the screen with.
		*/
		inline void unBindFrameBuffer() { m_frameBuffer->unBind(); }

		/** @brief Bind the renderer shadow mapping framebuffer
		*
		*	Bind the renderer's shadow mapping framebuffer for calculating shadows.
		*/
		inline void bindDepthFrameBuffer() { m_depthFrameBuffer->bind(); }
		/** @brief Unbind the renderer shadow mapping framebuffer
		*
		*	Unbind the renderer's shadow mapping framebuffer for calculating shadows.
		*/
		inline void unBindDepthFrameBuffer() { m_depthFrameBuffer->unBind(); }

		/** @brief Draws the default framebuffer to the screen quad
		*
		*	Draws the default framebuffer to the screen quad.
		*/
		void drawFrameBuffer();

		/** @brief Draws the shadow mapping framebuffer to the screen quad
		*
		*	Draws the shadow mapping framebuffer to the screen quad. This is primarily used for debugging the shadow map.
		*/
		void drawDepthFrameBuffer();

		/** @brief Sets whether depth should be tested when rendering
		*	@param _active Flag stating whether depth should be tested
		*
		*	Sets whether depth should be tested when rendering with OpenGL
		*/
		void setDepthTest(bool _active);

	protected:
		GLenum m_sencilFunc = GL_NOTEQUAL;						/**< The current stencil buffer function */
		int m_stencilBufferInt = 0x00;							/**< The stencil buffer integar */

		std::shared_ptr<FrameBuffer>	m_frameBuffer;			/**< The framebuffer that will be rendered to */
		std::shared_ptr<FrameBuffer>	m_depthFrameBuffer;		/**< The framebuffer that will be rendered to */
		std::shared_ptr<ShaderProgram>	m_shaderDepthTest;		/**< The framebuffer that will be rendered to */

		std::shared_ptr<Texture>		m_snowTexture;			/**< The texture used to render the snow */
	};
}
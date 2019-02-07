#pragma once

// TODO: Doxygen

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
		*	@param _mesh The mesh to render
		*	@param _shaderProgram The shader to render the mesh with
		*	@param _transform The transform to apply to the mesh
		*
		*	Renders the GPU_Mesh with the shader program and transform. GPU_Mesh protected variables
		*	are accessible through the "friend" class.
		*/
		void render(const Renderable &_renderable);

		/** @brief Sets whether the stencil buffer is active
		*	@param _active Flag stating whether to write to the stencil buffer or not
		*
		*	Sets whether the stencil buffer should be written to or not
		*/
		void setStencilBufferActive(bool _active);

		inline void bindFrameBuffer() { m_frameBuffer->bind(); }
		inline void unBindFrameBuffer() { m_frameBuffer->unBind(); }

		void drawFrameBuffer();

		/** @brief Sets whether depth should be tested when rendering
		*	@param _active Flag stating whether depth should be tested
		*
		*	Sets whether depth should be tested when rendering with OpenGL
		*/
		void setDepthTest(bool _active);

	protected:
		GLenum m_sencilFunc = GL_NOTEQUAL;
		int m_stencilBufferInt = 0x00;

		std::shared_ptr<FrameBuffer>	m_frameBuffer;	/**< The framebuffer that will be rendered to */
	};
}
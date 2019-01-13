#pragma once

// TODO: Doxygen

// cstdlib

// external libs

// program
#include "PCH.h"
#include "GPU_Mesh.h"
#include "Transform.h"
#include "ShaderProgram.h"

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

		/** @brief Loads a text file into the Shader
		*	@param _mesh The mesh to render
		*	@param _shaderProgram The shader to render the mesh with
		*	@param _transform The transform to apply to the mesh
		*
		*	Renders the GPU_Mesh with the shader program and transform. GPU_Mesh protected variables
		*	are accessible through the "friend" class.
		*/
		void render(const GPU_Mesh &_mesh, ShaderProgram &_shaderProgram, const Transform &_transform);

	protected:

	};
}
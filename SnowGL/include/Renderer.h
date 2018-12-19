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
	class Renderer
	{
	public:
		Renderer();

		void render(const GPU_Mesh &_mesh, ShaderProgram &_shaderProgram, const Transform &_transform);

	protected:

	};
}
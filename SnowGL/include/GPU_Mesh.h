#pragma once

// TODO: Doxygen

// cstdlib

// external libs

// program
#include "PCH.h"
#include "Vertex.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace SnowGL
{
	class GPU_Mesh
	{
		friend class Renderer;

	public:
		GPU_Mesh();

		void setMesh(const Mesh &_mesh);

	protected:
		std::shared_ptr<VertexArray>	m_VAO;
		std::shared_ptr<VertexBuffer>	m_VBO;
		std::shared_ptr<VertexBuffer>	m_IBO;
	};
}
#include "PCH.h"
#include "GPU_Mesh.h"

namespace SnowGL
{
	GPU_Mesh::GPU_Mesh()
	{
		// create the necessary buffers and arrays
		m_VAO = std::make_shared<VertexArray>();
		m_VBO = std::make_shared<VertexBuffer>(BUFFER_ARRAY_TEXTURE);
		m_IBO = std::make_shared<VertexBuffer>(BUFFER_ELEMENT_ARRAY);
	}

	void GPU_Mesh::setMesh(const Mesh &_mesh)
	{
		// add the model data VBO to the VAO
		m_VAO->addBuffer(*m_VBO, _mesh.layout);

		// load data to VBOs
		m_VBO->loadData(_mesh.vertices.data(), _mesh.layout.getStride() * _mesh.vertices.size());
		m_IBO->loadData(_mesh.indices.data(), (GLuint)_mesh.indices.size(), sizeof(unsigned int) * _mesh.indices.size());

		m_vertexCount = _mesh.vertices.size();
	}
}
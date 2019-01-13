#include "PCH.h"
#include "Renderer.h"

namespace SnowGL
{
	Renderer::Renderer()
	{
		glCullFace(GL_BACK);
	}

	void Renderer::render(const GPU_Mesh &_mesh, ShaderProgram &_shaderProgram, const Transform &_transform)
	{
		_shaderProgram.bind();
		_shaderProgram.setUniformMat4f("u_modelMatrix", _transform.getModelMatrix());
		// access member through friend
		_mesh.m_VAO->bind();
		_mesh.m_IBO->bind();

		glDrawElements(GL_TRIANGLES, _mesh.m_IBO->getCount(), GL_UNSIGNED_INT, 0);
	}

	
}
#include "PCH.h"
#include "Renderer.h"

namespace SnowGL
{
	Renderer::Renderer()
	{
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_MULTISAMPLE);

		// stencil buffer setup
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		glStencilFunc(m_sencilFunc, 1, 0xFF);
		glStencilMask(m_stencilBufferInt);
	}

	void Renderer::render(const GPU_Mesh &_mesh, ShaderProgram &_shaderProgram, const Transform &_transform)
	{
		// set stencil buffer
		glStencilFunc(m_sencilFunc, 1, 0xFF);
		glStencilMask(m_stencilBufferInt);

		_shaderProgram.bind();
		_shaderProgram.setUniformMat4f("u_modelMatrix", _transform.getModelMatrix());
		// access member through friend
		_mesh.m_VAO->bind();
		_mesh.m_IBO->bind();

		glDrawElements(GL_TRIANGLES, _mesh.m_IBO->getCount(), GL_UNSIGNED_INT, 0);
	}

	void Renderer::setStencilBufferActive(bool _active)
	{
		if (_active)
		{
			m_sencilFunc = GL_ALWAYS;
			m_stencilBufferInt = 0xFF;
		}
		else
		{
			m_sencilFunc = GL_NOTEQUAL;
			m_stencilBufferInt = 0x00;
		}
	}

	void Renderer::setDepthTest(bool _active)
	{
		if (_active)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}

}
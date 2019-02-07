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

		glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

		ApplicationState &state = ApplicationState::getInstance();
		m_frameBuffer = std::make_shared<FrameBuffer>(state.windowSize.x, state.windowSize.y);
	}

	void Renderer::render(const GPU_Mesh &_mesh, ShaderProgram &_shaderProgram, const Transform &_transform)
	{
		m_frameBuffer->bind();

		// set stencil buffer
		glStencilFunc(m_sencilFunc, 1, 0xFF);
		glStencilMask(m_stencilBufferInt);

		_shaderProgram.bind();
		_shaderProgram.setUniformMat4f("u_modelMatrix", _transform.getModelMatrix());
		// access member through friend
		_mesh.m_VAO->bind();
		_mesh.m_IBO->bind();

		glDrawElements(GL_TRIANGLES, _mesh.m_IBO->getCount(), GL_UNSIGNED_INT, 0);

		m_frameBuffer->unBind();
	}

	void Renderer::render(const Renderable &_renderable)
	{
		m_frameBuffer->bind();

		// set stencil buffer
		glStencilFunc(m_sencilFunc, 1, 0xFF);
		glStencilMask(m_stencilBufferInt);

		_renderable.m_shader->bind();
		_renderable.m_shader->setUniformMat4f("u_modelMatrix", _renderable.transform.getModelMatrix());
		_renderable.m_texture->bind();
		// access member through friend
		_renderable.m_mesh->m_VAO->bind();
		_renderable.m_mesh->m_IBO->bind();

		glDrawElements(GL_TRIANGLES, _renderable.m_mesh->m_IBO->getCount(), GL_UNSIGNED_INT, 0);

		m_frameBuffer->unBind();
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

	void Renderer::drawFrameBuffer()
	{
		m_frameBuffer->drawToScreen();
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
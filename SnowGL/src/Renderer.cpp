﻿#include "PCH.h"
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
		//glEnable(GL_STENCIL_TEST);
		//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		glStencilFunc(m_sencilFunc, 1, 0xFF);
		glStencilMask(m_stencilBufferInt);

		glClearColor(0.19f, 0.54f, 0.93f, 1.0f);

		SDL_GL_SetSwapInterval(1);

		ApplicationState &state = ApplicationState::getInstance();

		glViewport(0, 0, (GLsizei)state.windowSize.x, (GLsizei)state.windowSize.y);

		// colour frame buffer ----------------------------------------------------------------------------------------------------
		// create a frame buffer 
		m_frameBuffer = std::make_shared<FrameBuffer>(state.windowSize.x, state.windowSize.y, std::make_shared<ShaderProgram>("resources/shaders/post_processing/PostProcVert.vert", "resources/shaders/post_processing/PostProcFrag.frag"));
		// attach a depth render buffer
		m_frameBuffer->createDepthRenderBufferAttachment();
		// attach a colour buffer texture
		m_frameBuffer->attach(std::make_shared<Texture>("null", state.windowSize.x, state.windowSize.y, TEXTURE_RGB, TEXTURE_PIXEL_UBYTE), FBO_TEXTURE_COLOUR);
#ifdef COMPILE_DEBUG
		// check it all worked
		m_frameBuffer->verify();
#endif

		// depth frame buffer ----------------------------------------------------------------------------------------------------
		// create a FBO for the depth buffer with a depth rendering shader
		m_depthFrameBuffer = std::make_shared<FrameBuffer>(1024, 1024, std::make_shared<ShaderProgram>("resources/shaders/depth_test/DepthMapFBO.vert", "resources/shaders/depth_test/DepthMapFBO.frag"));
		// attach a texture for the depth buffer
		m_depthFrameBuffer->attach(std::make_shared<Texture>("null", 1024, 1024, TEXTURE_DEPTH, TEXTURE_PIXEL_FLOAT), FBO_TEXTURE_DEPTH);
		m_depthFrameBuffer->setColourBuffer(GL_NONE);
#ifdef COMPILE_DEBUG
		// verify the FBO
		m_depthFrameBuffer->verify();
#endif

		m_shaderDepthTest = std::make_shared<ShaderProgram>("resources/shaders/depth_test/DepthMap.vert", "resources/shaders/depth_test/DepthMap.frag");

		m_snowTexture = std::make_shared<Texture>("Snow", "resources/textures/snow.png");

		LOG(LOG_DEBUG) << "Finished creating renderer";
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

	void Renderer::render(const Renderable &_renderable)
	{
		m_snowTexture->bind(10);
		m_depthFrameBuffer->getTexture()->bind(20);

		// set stencil buffer
		glStencilFunc(m_sencilFunc, 1, 0xFF);
		glStencilMask(m_stencilBufferInt);

		// access member through friend
		_renderable.m_mesh->m_VAO->bind();
		_renderable.m_mesh->m_IBO->bind();

		ApplicationState &state = ApplicationState::getInstance();

		if (!state.renderWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		glDrawElements(GL_TRIANGLES, _renderable.m_mesh->m_IBO->getCount(), GL_UNSIGNED_INT, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	void Renderer::renderToDepthBuffer(const Renderable & _renderable, const Transform &_transform)
	{
		// set stencil buffer
		glStencilFunc(m_sencilFunc, 1, 0xFF);
		glStencilMask(m_stencilBufferInt);

		m_shaderDepthTest->bind();
		m_shaderDepthTest->setUniformMat4f("u_modelMatrix", _transform.getModelMatrix());
		_renderable.m_texture->bind(0);

		_renderable.m_mesh->m_VAO->bind();
		_renderable.m_mesh->m_IBO->bind();

		glDrawElements(GL_TRIANGLES, _renderable.m_mesh->m_IBO->getCount(), GL_UNSIGNED_INT, 0);
	}
	void Renderer::renderToDepthBuffer(const Renderable & _renderable)
	{
		// set stencil buffer
		glStencilFunc(m_sencilFunc, 1, 0xFF);
		glStencilMask(m_stencilBufferInt);

		m_shaderDepthTest->bind();
		m_shaderDepthTest->setUniformMat4f("u_modelMatrix", _renderable.transform.getModelMatrix());
		_renderable.m_texture->bind(0);

		_renderable.m_mesh->m_VAO->bind();
		_renderable.m_mesh->m_IBO->bind();

		glDrawElements(GL_TRIANGLES, _renderable.m_mesh->m_IBO->getCount(), GL_UNSIGNED_INT, 0);
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

	void Renderer::drawDepthFrameBuffer()
	{
		m_depthFrameBuffer->drawToScreen();
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
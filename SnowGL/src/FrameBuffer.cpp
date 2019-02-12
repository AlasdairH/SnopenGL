#include "PCH.h"
#include "FrameBuffer.h"

namespace SnowGL
{
	FrameBuffer::FrameBuffer(const int _width, const int _height, std::shared_ptr<ShaderProgram> _shader)
	{
		m_width = _width;
		m_height = _height;
		CONSOLE_MESSAGE("Creating Frame Buffer with resolution: " << m_width << ", " << m_height);

		// create VAO
		m_arrayBuffer = new VertexArray;
		// create VBO
		m_vertexBuffer = new VertexBuffer(BUFFER_ARRAY);
		GLfloat fbo_vertices[] = 
		{
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};
		// create layout
		VertexBufferLayout layout1;
		// push 2 floats to the layout (x and y pos)
		layout1.push<float>(2);
		// push 2 floats to the layout (x and y tex coord)
		layout1.push<float>(2);
		m_arrayBuffer->addBuffer(*m_vertexBuffer, layout1);
		m_vertexBuffer->loadData(&fbo_vertices, 0, sizeof(fbo_vertices));

		m_shaderProgram = _shader;
		m_shaderProgram->setUniform1i("screenTexture", 0);

		glGenFramebuffers(1, &m_frameBufferID);

		unBind();
	}

	FrameBuffer::~FrameBuffer()
	{
		GLuint id = m_texture->getGLID();
		glDeleteFramebuffers(1, &m_frameBufferID);
		glDeleteTextures(1, &id);
		glDeleteRenderbuffers(1, &m_depthRenderBufferID);
		delete m_vertexBuffer;
		delete m_arrayBuffer;
	}

	void FrameBuffer::attach(std::shared_ptr<Texture> _texture, FrameBufferTextureType _type)
	{
		CONSOLE_MESSAGE("Attaching texture " << _texture->getGLID() << " to frame buffer " << m_frameBufferID);
		m_texture = _texture;
		bind();
		_texture->bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, _type, GL_TEXTURE_2D, _texture->getGLID(), 0);
		_texture->unBind();
		unBind();
	}

	void FrameBuffer::setColourBuffer(GLenum _buffer)
	{
		bind();
		glDrawBuffer(_buffer);
		glReadBuffer(_buffer);
		unBind();
	}

	void FrameBuffer::bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferID);
		glViewport(0, 0, m_width, m_height);
	}

	void FrameBuffer::unBind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default franebuffer
	}

	std::shared_ptr<Texture> FrameBuffer::getTexture()
	{
		return m_texture;
	}

	bool FrameBuffer::verify()
	{
		bind();
		GLenum status;
		if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
		{
			CONSOLE_ERROR("Unable to verify frame buffer " << m_frameBufferID << ": " << status);
			return false;
		}
		else
		{
			CONSOLE_MESSAGE("Verified Frame Buffer with ID: " << m_frameBufferID);
			return true;
		}
		unBind();
		return false;
	}

	void FrameBuffer::drawToScreen()
	{
		unBind();

		glClear(GL_COLOR_BUFFER_BIT);

		m_shaderProgram->bind();
		m_arrayBuffer->bind();

		m_texture->bind();

		glDisable(GL_DEPTH_TEST);

		// draw
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glEnable(GL_DEPTH_TEST);

#ifdef DEBUG
		m_shaderProgram->unBind();
		m_arrayBuffer->unBind();
#endif
	}

	void FrameBuffer::createDepthRenderBufferAttachment()
	{
		bind();
		glGenRenderbuffers(1, &m_depthRenderBufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderBufferID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);

		// attach the renderbuffer to the framebuffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBufferID);
	}
}
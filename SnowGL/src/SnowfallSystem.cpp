#include "PCH.h"
#include "..\include\SnowfallSystem.h"

namespace SnowGL
{
	SnowfallSystem::SnowfallSystem()
	{

	}

	SnowfallSystem::~SnowfallSystem()
	{

	}
	bool SnowfallSystem::initialise()
	{
		m_tfShader = std::make_shared<ShaderProgram>();

		Shader tfVert(SHADER_VERTEX);
		tfVert.load("resources/shaders/particle/particle.vert");
		m_tfShader->attachShader(tfVert);
		m_tfShader->setTransformFeedbackVarying("out_Value", 1);
		m_tfShader->link();

		GLfloat data[] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };

		// Create VAO
		m_tfVAO = std::make_shared<VertexArray>();

		// Create transform feedback buffer 1
		m_tfBuffer[0] = std::make_shared<VertexBuffer>(BUFFER_ARRAY);
		m_tfBuffer[1] = std::make_shared<VertexBuffer>(BUFFER_ARRAY);
		// load initial data
		m_tfBuffer[0]->loadData(data, sizeof(data));

		// set attrib array in VAO
		GLint inputAttrib = m_tfShader->getAttributeLocation("in_Value");
		std::cout << inputAttrib << std::endl;
		m_tfVAO->setAttribArray(inputAttrib, 1, GL_FLOAT, GL_FALSE, 0, 0);

		// Create transform feedback buffer 2
		m_tfBuffer[1]->loadData(nullptr, sizeof(data));

		// set current vertex buffer and current transform feedback buffer to be alternate of eachother (0, 1);
		//m_currVB = m_currTFB;
		//m_currTFB = (m_currTFB + 1) & 0x1;

		return true;
	}

	void SnowfallSystem::render(int _deltaTimeMS, const glm::mat4 &_VP, const glm::vec3 &_cameraPos)
	{

		updateParticles(_deltaTimeMS);

		renderParticles(_VP, _cameraPos);

		m_currVB = m_currTFB;
		m_currTFB = (m_currTFB + 1) & 0x1;
	}

	void SnowfallSystem::renderParticles(const glm::mat4 & _VP, const glm::vec3 & _cameraPos)
	{

	}

	void SnowfallSystem::updateParticles(int _deltaTimeMS)
	{
		// Perform feedback transform
		glEnable(GL_RASTERIZER_DISCARD);

		m_tfBuffer[1]->bindBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0);

		glBeginTransformFeedback(GL_POINTS);
		glDrawArrays(GL_POINTS, 0, 5);
		glEndTransformFeedback();

		glDisable(GL_RASTERIZER_DISCARD);

		glFlush();

		// Fetch and print results
		GLfloat feedback[5];
		glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);

		CONSOLE_MESSAGE(feedback[0] << ", " << feedback[1] << ", " << feedback[2] << ", " << feedback[3] << ", " << feedback[4]);
	}
}
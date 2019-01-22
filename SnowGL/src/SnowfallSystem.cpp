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
		CONSOLE_MESSAGE("Initialising Snowfall Particle System")

		m_tfShader = std::make_shared<ShaderProgram>();

		float spread = 10.0f;

		Shader tfVert(SHADER_VERTEX);
		tfVert.load("resources/shaders/particle/particle.vert");
		m_tfShader->attachShader(tfVert);
		Shader tfFrag(SHADER_FRAGMENT);
		tfFrag.load("resources/shaders/particle/particle.frag");
		m_tfShader->attachShader(tfFrag);
		std::vector<std::string> tfVaryings{ "out_position", "out_velocity", "out_delay" };
		m_tfShader->setTransformFeedbackVarying(tfVaryings);
		m_tfShader->link();

		m_numParticles = 100000;

		VertexBufferLayout layout;
		layout.push<glm::vec4>(1);	// position (w = is active)
		layout.push<glm::vec3>(1);	// velocity
		layout.push<float>(1);		// delay

		for (int i = 0; i < 2; ++i)
		{
			m_tfVAO[i] = std::make_shared<VertexArray>();
			m_tfVBO[i] = std::make_shared<VertexBuffer>(BUFFER_TRANSFORM_FEEDBACK);

			m_tfVBO[i]->bind();
			m_tfVBO[i]->loadData(nullptr, sizeof(Particle) * m_numParticles);

			if (i == 0)
			{
				struct buffer_t
				{
					glm::vec4 position;		/**< The particle position */
					glm::vec3 velocity;		/**< The particle velocity */
					float delay;			/**< The particles maximum delay */
				} *buffer = (buffer_t *) glMapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, GL_WRITE_ONLY);

				for (int j = 0; j < m_numParticles; ++j)
				{
					buffer[j].position = glm::vec4(Utils::randFloat(-spread, spread), 10, Utils::randFloat(-spread, spread), 1);
					buffer[j].velocity = glm::vec3(0, 0, 0);
					buffer[j].delay = Utils::randFloat(0.0f, 5.0f);
				}

				glUnmapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER);
			}

			m_tfVAO[i]->bind();
			m_tfVBO[i]->bind(BUFFER_ARRAY);
			m_tfVAO[i]->addBuffer(*m_tfVBO[i], layout);
		}

		// particle system setup
		m_tfShader->setUniform3f("u_baseColour", glm::vec3(1.0f));

		// set current vertex buffer and current transform feedback buffer to be alternate of eachother (0, 1);
		m_currVAO = m_currVBO;
		m_currVBO = (m_currVBO + 1) & 0x1;

		return true;
	}

	void SnowfallSystem::render(int _deltaTime, const glm::mat4 &_VP, const glm::vec3 &_cameraPos)
	{

		updateParticles(_deltaTime);

		renderParticles(_VP, _cameraPos);


	}

	void SnowfallSystem::renderParticles(const glm::mat4 & _VP, const glm::vec3 & _cameraPos)
	{

	}

	void SnowfallSystem::updateParticles(float _deltaTime)
	{
		m_simTime += _deltaTime;
		CONSOLE_MESSAGE(m_simTime);
		m_tfShader->setUniform1f("u_deltaTime", _deltaTime);
		m_tfShader->setUniform1f("u_simTime", m_simTime);


		m_tfShader->bind();
		m_tfShader->setUniformMat4f("u_modelMatrix", m_transform.getModelMatrix());

		m_tfVAO[m_currVAO]->bind();
		m_tfVBO[m_currVBO]->bindBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0);

		glBeginTransformFeedback(GL_POINTS);
		glDrawArrays(GL_POINTS, 0, m_numParticles);
		glEndTransformFeedback();

		// ping pong
		m_currVAO = m_currVBO;
		m_currVBO = (m_currVBO + 1) & 0x1;
			
		++m_frameCount;
	}
}
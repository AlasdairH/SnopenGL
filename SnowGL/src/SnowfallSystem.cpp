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

		Shader tfVert(SHADER_VERTEX);
		tfVert.load("resources/shaders/particle/particle.vert");
		m_tfShader->attachShader(tfVert);
		Shader tfFrag(SHADER_FRAGMENT);
		tfFrag.load("resources/shaders/BlockColour.frag");
		m_tfShader->attachShader(tfFrag);
		std::vector<std::string> tfVaryings{ "out_position", "out_velocity", "out_lifetime" };
		m_tfShader->setTransformFeedbackVarying(tfVaryings);
		m_tfShader->link();

		m_numParticles = 1000000;

		VertexBufferLayout layout;
		layout.push<glm::vec4>(1);	// position (w = is active)
		layout.push<glm::vec3>(1);	// velocity
		layout.push<float>(1);		// lifetime

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
					float lifetime;			/**< The particles maximum lifetime */
				} *buffer = (buffer_t *) glMapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, GL_WRITE_ONLY);

				for (int j = 0; j < m_numParticles; ++j)
				{
					buffer[j].position = glm::vec4(Utils::randFloat(-5.0f, 5.0f), 5, Utils::randFloat(-5.0f, 5.0f), 1);
					buffer[j].velocity = glm::vec3(0, 0, 0);
					buffer[j].lifetime = Utils::randFloat(0.0f, 5000.0f);
				}

				glUnmapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER);
			}

			m_tfVAO[i]->bind();
			m_tfVBO[i]->bind(BUFFER_ARRAY);
			m_tfVAO[i]->addBuffer(*m_tfVBO[i], layout);
		}

		// set current vertex buffer and current transform feedback buffer to be alternate of eachother (0, 1);
		//m_currVB = m_currTFB;
		//m_currTFB = (m_currTFB + 1) & 0x1;

		return true;
	}

	void SnowfallSystem::render(int _deltaTime, const glm::mat4 &_VP, const glm::vec3 &_cameraPos)
	{

		updateParticles(_deltaTime);

		renderParticles(_VP, _cameraPos);

		m_currVB = m_currTFB;
		m_currTFB = (m_currTFB + 1) & 0x1;
	}

	void SnowfallSystem::renderParticles(const glm::mat4 & _VP, const glm::vec3 & _cameraPos)
	{

	}

	void SnowfallSystem::updateParticles(float _deltaTime)
	{
		//CONSOLE_MESSAGE("Updating Particles");

		glm::vec3 randPos = glm::vec3(Utils::randFloat(-5.0f, 5.0f), 5, Utils::randFloat(-5.0f, 5.0f));

		m_tfShader->setUniform1f("u_timeStep", _deltaTime);
		m_tfShader->setUniform3f("u_randomStartingPos", randPos.x, randPos.y, randPos.z);

		// Perform feedback transform
		//glEnable(GL_RASTERIZER_DISCARD);

		m_tfShader->bind();
		m_tfShader->setUniformMat4f("u_modelMatrix", m_transform.getModelMatrix());
		m_tfShader->setUniform4f("diffuseColour", 1.0f, 1.0f, 1.0f, 1.0f);

		if ((m_frameCount & 1) != 0)
		{
			m_tfVAO[1]->bind();
			m_tfVBO[0]->bindBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0);
		}
		else
		{
			m_tfVAO[0]->bind();
			m_tfVBO[1]->bindBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0);
		}

		glBeginTransformFeedback(GL_POINTS);
		glDrawArrays(GL_POINTS, 0, m_numParticles);
		glEndTransformFeedback();

		//glDisable(GL_RASTERIZER_DISCARD);

		/*
		// Fetch and print results
		std::vector<Particle> particles;
		particles.resize(m_numParticles);

		glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_numParticles * sizeof(Particle), particles.data());

		for (int i = 0; i < particles.size(); ++i)
		{
			CONSOLE_MESSAGE(particles[i].position.x << ", " << particles[i].position.y << ", " << particles[i].position.z << " | "
				<< particles[i].velocity.x << ", " << particles[i].velocity.y << ", " << particles[i].velocity.z << " | " << particles[i].lifetime);
		}
		*/
			
		++m_frameCount;
		//CONSOLE_MESSAGE("Update Complete")
	}
}
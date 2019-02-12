#include "PCH.h"
#include "ParticleSystem.h"

namespace SnowGL
{
	ParticleSystem::ParticleSystem(const ParticleSettings &_settings)
	{
		m_settings = std::make_shared<ParticleSettings>(_settings);
	}

	ParticleSystem::~ParticleSystem()
	{

	}

	bool ParticleSystem::initialise()
	{
		CONSOLE_MESSAGE("Initialising Snowfall Particle System")

		m_tfShader = std::make_shared<ShaderProgram>();

		float spread = 1.0f;

		Shader tfVert(SHADER_VERTEX);
		tfVert.load("resources/shaders/particle/particle.vert");
		m_tfShader->attachShader(tfVert);
		Shader tfFrag(SHADER_FRAGMENT);
		tfFrag.load("resources/shaders/particle/particle.frag");
		m_tfShader->attachShader(tfFrag);
		std::vector<std::string> tfVaryings{ "out_position", "out_startPosition", "out_velocity", "out_startTime", "out_lifetime" };
		m_tfShader->setTransformFeedbackVarying(tfVaryings);
		m_tfShader->link();

		m_numParticles = m_settings->getMaxParticles();

		VertexBufferLayout layout;
		layout.push<glm::vec4>(1);	// position (w = is active)
		layout.push<glm::vec4>(1);	// start position
		layout.push<glm::vec3>(1);	// velocity
		layout.push<float>(1);		// delay
		layout.push<float>(1);		// lifetime

		for (int i = 0; i < 2; ++i)
		{
			// create 2 VAOs and VBOs for ping ponging
			m_tfVAO[i] = std::make_shared<VertexArray>();
			m_tfVBO[i] = std::make_shared<VertexBuffer>(BUFFER_ARRAY);

			m_tfVBO[i]->bind();
			// load correctly sized but empty data to the VBO
			m_tfVBO[i]->loadData(nullptr, sizeof(Particle) * m_numParticles);

			// if the buffer is the first to be created, send it the actual particle data
			if (i == 0)
			{
 
				Particle *buffer = (Particle *)glMapBuffer(BUFFER_ARRAY, GL_WRITE_ONLY);

				for (int j = 0; j < m_numParticles; ++j)
				{
					buffer[j].currentPosition = glm::vec4(Utils::randFloat(-spread, spread), 10, Utils::randFloat(-spread, spread), 1);
					buffer[j].startPosition = buffer[j].currentPosition;
					buffer[j].velocity = glm::vec3(0, 0, 0);
					buffer[j].delay = (j / (float)m_numParticles) * m_settings->lifetimeMax;
					buffer[j].lifetime = Utils::randFloat(m_settings->lifetimeMin, m_settings->lifetimeMax);
				}

				glUnmapBuffer(BUFFER_ARRAY);
			}

			m_tfVAO[i]->bind();
			m_tfVBO[i]->bind(BUFFER_ARRAY);
			m_tfVAO[i]->addBuffer(*m_tfVBO[i], layout);
		}

		// particle system setup
		applySettingsToShader();

		// set current vertex buffer and current transform feedback buffer to be alternate of eachother (0, 1);
		m_currVAO = m_currVBO;
		m_currVBO = (m_currVBO + 1) & 0x1;

		CONSOLE_MESSAGE("Created " << m_numParticles << " particles on the GPU");

		return true;
	}

	void ParticleSystem::applySettingsToShader()
	{
		m_tfShader->setUniform4f("u_startColour", m_settings->colourStart);
		m_tfShader->setUniform4f("u_endColour", m_settings->colourEnd);
		m_tfShader->setUniform3f("u_globalWind", m_settings->globalWind);
		CONSOLE_MESSAGE("Particle settings applied to shader")
	}

	void ParticleSystem::updateParticles(float _deltaTime, int _vertexCount)
	{
		m_simTime += _deltaTime;
		//CONSOLE_MESSAGE(m_simTime);
		m_tfShader->bind();
		m_tfShader->setUniform1f("u_deltaTime", _deltaTime);
		m_tfShader->setUniform1f("u_simTime", m_simTime);
		m_tfShader->setUniform1i("u_triangleCount", _vertexCount);

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
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
		Particle particles[MAX_PARTICLES];

		particles[0].type = PARTICLE_ROOT;
		particles[0].position = glm::vec3(0, 0, 0);
		particles[0].velocity = glm::vec3(0.0f, 0.0001f, 0.0f);
		particles[0].lifetime = 0.0f;

		glGenTransformFeedbacks(2, m_transformFeedback);

		for (unsigned int i = 0; i < 2; i++) 
		{
			m_particleBuffer[i] = std::make_shared<VertexBuffer>(BUFFER_ARRAY);
			glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedback[i]);
			m_particleBuffer[i]->bind();
			m_particleBuffer[i]->loadData(particles, -1, sizeof(particles));
			m_particleBuffer[i]->bindBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0);
		}
	}

	void SnowfallSystem::render(int _deltaTimeMS, const glm::mat4 &_VP, const glm::vec3 &_cameraPos)
	{
		m_time += _deltaTimeMS;

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

	}
}
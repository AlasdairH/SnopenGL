#pragma once

// cstdlib

// external libs


// program
#include "PCH.h"
#include "VertexBuffer.h"

#define MAX_PARTICLES 10000

namespace SnowGL
{
	enum ParticleType { PARTICLE_STANDARD, PARTICLE_ROOT };

	struct Particle
	{
		ParticleType type;
		glm::vec3 position;
		glm::vec3 velocity;
		float lifetime;
	};

	class SnowfallSystem
	{
	public:
		SnowfallSystem();
		~SnowfallSystem();

		bool initialise();

		void render(int _deltaTimeMS, const glm::mat4 & _VP, const glm::vec3 & _cameraPos);

	protected:
		void renderParticles(const glm::mat4 & _VP, const glm::vec3 & _cameraPos);
		void updateParticles(int _deltaTimeMS);

		bool m_isFirstRender;
		unsigned int m_currVB;
		unsigned int m_currTFB;
		std::shared_ptr<VertexBuffer> m_particleBuffer[2];
		GLuint m_transformFeedback[2];
		//PSUpdateTechnique m_updateTechnique;
		//BillboardTechnique m_billboardTechnique;
		int m_time;

	};
}
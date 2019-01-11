#pragma once

// cstdlib

// external libs


// program
#include "PCH.h"
#include "ShaderProgram.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

#define MAX_PARTICLES 10000

namespace SnowGL
{
	struct Particle
	{
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

		void updateParticles(int _deltaTimeMS);
		void render(int _deltaTimeMS, const glm::mat4 & _VP, const glm::vec3 & _cameraPos);

	protected:
		void renderParticles(const glm::mat4 & _VP, const glm::vec3 & _cameraPos);

		bool m_isFirstRender;
		unsigned int m_currVB;
		unsigned int m_currTFB;
		std::shared_ptr<VertexArray> m_tfVAO;
		std::shared_ptr<VertexBuffer> m_tfBuffer[2];

		std::shared_ptr<ShaderProgram> m_tfShader;
	};
}
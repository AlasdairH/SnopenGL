#pragma once

// cstdlib

// external libs


// program
#include "PCH.h"
#include "Transform.h"
#include "ShaderProgram.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Utils.h"

#define MAX_PARTICLES 10000

namespace SnowGL
{
	/*! @class Particle
	*	@brief The data for a particle in the Snowfall System
	*
	*	A particle data class which contains only the data for managing particles, not rendering.
	*/
	struct Particle
	{
		glm::vec4 position;		/**< The particle position */
		glm::vec3 velocity;		/**< The particle velocity */
		float delay = -1;	/**< The particles maximum delay */
	};

	/*! @class SnowfallSystem
	*	@brief The managment system for simulating snowfall
	*
	*	The snowfall system simulates particles through Transform Feedback with OpenGL.
	// TODO: Expand
	*/
	class SnowfallSystem
	{
	public:
		/** @brief Snowfall System Ctor
		*
		*	Nothing
		*/
		SnowfallSystem();
		/** @brief Snowfall System Dtor
		*
		*	Nothing
		*/
		~SnowfallSystem();

		/** @brief Initialises the Snowfall System
		*
		*	Initialises the buffers and shaders for Transform Feedback
		*/
		bool initialise();

		/** @brief Renderer Ctor
		*	@param _deltaTime The update deltaTime
		*
		*	Performs the transform feedback operation for simulating particles
		*/
		void updateParticles(float _deltaTime);
		/** @brief Renderer Ctor
		*	@param _deltaTime The update deltaTime
		// TODO: Particle Renderer
		*
		*	Performs the transform feedback operation for simulating particles
		*/
		void render(int _deltaTime, const glm::mat4 & _VP, const glm::vec3 & _cameraPos);

	protected:
		// TODO: This
		void renderParticles(const glm::mat4 & _VP, const glm::vec3 & _cameraPos);

		bool								m_isFirstRender;		/**< Flag for if this is the first render */			
		unsigned int						m_currVAO;				/**< The current Transform Feedback Buffer */
		unsigned int						m_currVBO;				/**< The current Vertex Buffer */
		std::shared_ptr<VertexArray>		m_tfVAO[2];				/**< The Vertex Arrays for particle data */
		std::shared_ptr<VertexBuffer>		m_tfVBO[2];				/**< The Vertex Buffers for particle data */

		std::shared_ptr<ShaderProgram>		m_tfShader;				/**< The transform Feedback shader */

		int									m_numParticles;			/**< THe number of particles */

		int									m_frameCount = 0;
		float								m_simTime = 0;

		Transform							m_transform;
	};
}
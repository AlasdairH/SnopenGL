#pragma once

// cstdlib

// external libs


// program
#include "PCH.h"
#include "ParticleSettings.h"
#include "Transform.h"
#include "ShaderProgram.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Utils.h"
#include "GPU_SSBO_AccumulationPartition.h"

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
		glm::vec4	currentPosition;	/**< The particle position */
		glm::vec4	startPosition;		/**< The particle position */
		glm::vec3	velocity;			/**< The particle velocity */
		float		delay		= -1;	/**< The particles delay from the start of the simulation to when it is created */
		float		lifetime	= -1;	/**< The particles lifetime */
	};

	/*! @class SnowfallSystem
	*	@brief The management system for simulating snowfall
	*
	*	The snowfall system simulates particles through Transform Feedback with OpenGL.
	// TODO: Expand
	*/
	class ParticleSystem
	{
	public:
		/** @brief Snowfall System Ctor
		*
		*	Nothing
		*/
		ParticleSystem(const ParticleSettings &_settings);
		/** @brief Snowfall System Dtor
		*
		*	Nothing
		*/
		~ParticleSystem();

		/** @brief Initialises the Snowfall System
		*
		*	Initialises the buffers and shaders for Transform Feedback
		*/
		bool initialise();

		/** @brief Applys settings to the shader
		*
		*	Takes the stored settings CPU side and applies them to the GPU based shader for the particles.
		*/
		void applySettingsToShader();

		/** @brief Renderer Ctor
		*	@param _deltaTime The update deltaTime
		*	@param _triangleCount The number of triangles in the scene
		*
		*	Performs the transform feedback operation for simulating particles
		*/
		void updateParticles(float _deltaTime, int _triangleCount);

		/** @brief Gets the partcle settings
		*	@return A pointer to the particles settings
		*
		*	Returns a pointer to the particles settings which can be modified and then pushed to the GPU using the applySettingsToShader
		*	method, also on this class.
		*/
		inline std::shared_ptr<ParticleSettings> getSettingsPtr() { return m_settings; }

		/** @brief Sets the point size to use to render the particles
		*	@param _size The size to use
		*
		*	Sets the OpenGL point size
		*/
		inline void setPointSize(float _size) { m_pointSize = _size; glPointSize(m_pointSize); }	
		/** @brief Gets the point size to be used to render the snow
		*	@return Gets the point size
		*
		*	Gets the OpenGL point size
		*/
		inline float getPointSize() { return m_pointSize; }

		/** @brief Sets the buffer to get the world space geometry from
		*	@param _tex The texture buffer to get the data from
		*	@param _ary The array buffer that contains the texture buffer
		*
		*	Sets the buffer to get the world space geometry from. This is used in the collision detection in the particle simulation.
		*/
		inline void setWsGeometryBuffer(GLuint _tex, GLuint _ary) { m_wsGeomTextureBuffer = _tex; m_wsGeomArrayBuffer = _ary; }

	protected:
		std::shared_ptr<ParticleSettings>	m_settings;					/**< The settings used by the particle system */

		bool								m_isFirstRender;			/**< Flag for if this is the first render */			
		unsigned int						m_currVAO;					/**< The current Transform Feedback Buffer */
		unsigned int						m_currVBO;					/**< The current Vertex Buffer */
		std::shared_ptr<VertexArray>		m_tfVAO[2];					/**< The Vertex Arrays for particle data */
		std::shared_ptr<VertexBuffer>		m_tfVBO[2];					/**< The Vertex Buffers for particle data */

		std::shared_ptr<VertexBuffer>		m_accumulationSSBO;			/**< SSBO version of the accumulation data */
		GPU_SSBO_accumulationPartition		m_SSBO_AccumulationData;	/**< SSBO version of the accumulation data */

		std::shared_ptr<ShaderProgram>		m_tfShader;					/**< The transform Feedback shader */
		GLuint								m_wsGeomArrayBuffer;
		GLuint								m_wsGeomTextureBuffer;

		int									m_numParticles;				/**< The number of particles */

		int									m_frameCount = 0;			/**< The number of frames since the start of the simulation */
		float								m_simTime = 0;				/**< How long the simulation has been running */

		float								m_pointSize = 1.0f;			/**< The point size to use when rendering the particles */

		Transform							m_transform;				/**< The transform used for the overall position of the simulation */

		std::shared_ptr<Renderable>			m_drawableDomain;			/**< A drawable representation of the particle domain */
		Transform							m_domainTransform;			/**< The transform for the drawable domain */
	};
}
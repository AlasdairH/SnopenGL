#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"

namespace SnowGL
{
	struct ParticleSettings
	{
		float		particlesPerSecond;		// The number of particles emitted per second
		float		lifetimeMin;			// min lifetime in seconds
		float		lifetimeMax;			// max lifetime in seconds
		float		collisionMultiplier;	// max lifetime in seconds

		glm::vec3	globalWind;

		glm::vec4	colourStart;			// the start colour in RGBA
		glm::vec4	colourEnd;				// the end colour in RGBA
		glm::vec4	collisionDebugColour;	// colour of any particles colliding

		glm::vec3	initialVelocity = glm::vec3(0.0f);	// the end colour in RGBA

		glm::vec3	domainPosition = glm::vec3(0.0f);			// domain position
		glm::vec3	domainSize = glm::vec3(5.0f);

		bool		drawDomain;

		/** @brief Required particle count getter
		*	@return The maxiumum number of particles that would be required for the system
		*
		*	Calculagtes the maxiumum number of particles that would be required for the system
		*/
		inline int getMaxParticles() const { return ceil(lifetimeMax) * (int)particlesPerSecond; }
	};
}
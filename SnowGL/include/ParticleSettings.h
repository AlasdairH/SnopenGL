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

		glm::vec4	colourStart;			// the start colour in RGBA
		glm::vec4	colourEnd;				// the end colour in RGBA

		/** @brief Required particle count getter
		*	@return The maxiumum number of particles that would be required for the system
		*
		*	Calculagtes the maxiumum number of particles that would be required for the system
		*/
		inline int getMaxParticles() const { return ceil(lifetimeMax) * particlesPerSecond; }
	};
}
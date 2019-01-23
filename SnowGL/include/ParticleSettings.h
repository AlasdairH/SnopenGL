#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"

namespace SnowGL
{
	struct ParticleSettings
	{
		float particlesPerSecond;	// The number of particles emitted per second
		float lifetimeMin;			// min lifetime in seconds
		float lifetimeMax;			// max lifetime in seconds

		inline int getMaxParticles() const { return ceil(lifetimeMax) * particlesPerSecond; }
	};
}
#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"
#include "Renderable.h"
#include "ParticleSystem.h"

namespace SnowGL
{
	class Scene
	{
	public:
		Scene();
		~Scene();

	protected:
		std::vector<Renderable> m_objects;
		std::vector<ParticleSystem> m_particleSystems;

	};
}
#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"
#include "Renderable.h"
#include "ParticleSystem.h"

namespace SnowGL
{
	/*! @class Scene
	*	@brief A 3D scene containing objects
	*
	*	This class holds the objects and particles in a 3D scene
	*/
	class Scene
	{
	public:
		/** @brief Scene Ctor
		*
		*	Does nothing
		*/
		Scene();
		/** @brief Scene Dtor
		*
		*	Does nothing
		*/
		~Scene();

	protected:
		std::vector<Renderable>			m_objects;				/**< A vector of renderable objects in the scene */	
		std::vector<ParticleSystem>		m_particleSystems;		/**< A vector of all particle systems in the scene */

	};
}
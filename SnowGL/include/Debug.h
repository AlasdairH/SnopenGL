#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"

// TODO: Doxygen

namespace SnowGL
{
	class Debug
	{
	public:
		/** @brief Gets the static instance of the Debug
		*
		*	Gets the static instance of the Debug.
		*/
		static Debug& getInstance()
		{
			static Debug instance;		// guaranteed to be destroyed.
											// instantiated on first use.
			return instance;
		}

		~Debug();

		void drawCube(Transform &_transform, glm::vec4 _colour);
		void drawPlane(Transform &_transform, glm::vec4 _colour);

		// ------------------------------------------------
		Debug(Debug const&) = delete;
		void operator=(Debug const&) = delete;

	protected:
		Debug();

		std::shared_ptr<Renderable>	 m_debugPlane;
		std::shared_ptr<Renderable>	 m_debugCube;
	};
}
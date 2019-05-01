#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"

namespace SnowGL
{
	/*! @class InitManager
	*	@brief Initialisation for external libraries.
	*
	*	This class handles the initialisation and uninitialisation of external libraries such as SDL and OpenGL (GLEW).
	*/
	class InitManager
	{
	public:
		/** @brief SDL Initialisation
		*
		*	Initialises SDL.
		*/
		static void initSDL();

		/** @brief SDL Initialisation
		*
		*	Initialises OpenGL (GLEW). Before this is done, an OpenGL Context much exist!! (see Window class)
		*/
		static void initOpenGL();
	};
}
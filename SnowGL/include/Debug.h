#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"

namespace SnowGL
{
	/*! @class Debug
	*	@brief A (singleton) class containing debug tools for OpenGL 
	*
	*	This class contains the methods to draw debug objects 
	*/
	class Debug
	{
	public:
		/** @brief Gets the static instance of the Debug class
		*
		*	Gets the static instance of the Debug.
		*/
		static Debug& getInstance()
		{
			static Debug instance;		// guaranteed to be destroyed.	
			return instance;			// instantiated on first use.
		}

		~Debug();

		/** @brief Draws a debug cube
		*	@param _transform The transform at which to draw the object
		*	@param _colour The colour to draw the object in
		*
		*	Draws a debug cube at a given transform and in a specified colour
		*/
		void drawCube(Transform &_transform, glm::vec4 _colour);
		/** @brief Draws a debug plane
		*	@param _transform The transform at which to draw the object
		*	@param _colour The colour to draw the object in
		*
		*	Draws a debug plane at a given transform and in a specified colour
		*/
		void drawPlane(Transform &_transform, glm::vec4 _colour);

		// ------------------------------------------------
		Debug(Debug const&) = delete;
		void operator=(Debug const&) = delete;

	protected:
		/** @brief Debug ctor
		*
		*	Initialised on first instance call
		*/
		Debug();

		std::shared_ptr<Renderable>	 m_debugPlane;	/**< A shared pointer to the plane renderable */
		std::shared_ptr<Renderable>	 m_debugCube;	/**< A shared pointer to the cube renderable */
	};
}
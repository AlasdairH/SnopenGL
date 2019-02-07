#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"

// TODO: Doxygen

namespace SnowGL
{
	enum SceneMode { MODE_VIEW, MODE_EDIT };

	/*! @class ApplicationState
	*	@brief Holds the state of the application
	*
	*	Holds various elements of the applications state
	*/
	class ApplicationState
	{
	public:
		/** @brief Gets the static instance of the ResourceManager
		*
		*	Gets the static instance of the ResourceManager.
		*/
		static ApplicationState& getInstance()
		{
			static ApplicationState instance;	// guaranteed to be destroyed
												// instantiated on first use
			return instance;
		}

		bool isRunning			= true;
		bool isMenuBarHidden	= false;

		// window
		glm::vec2 windowSize;

		// performance
		float deltaTime;
		float framesPerSecond;

		// input
		glm::vec2 mousePosition;
		glm::vec2 lastMousePosition = glm::vec2(640, 360);
		glm::vec2 mouseOffset;

		// camera
		float cameraPitch;
		float cameraYaw = -90.0f;

		// gl
		std::string curBoundTexture = "None";

		inline SceneMode getSceneMode() { return m_sceneMode; }
		
		void switchSceneMode();

		// ------------------------------------------------
		ApplicationState(ApplicationState const&) = delete;
		void operator=(ApplicationState const&) = delete;

	protected:

		SceneMode m_sceneMode = MODE_VIEW;


		/** @brief ApplicationState Ctor
		*
		*	This is called on the first time the ApplicationState instance is grabbed.
		*/
		ApplicationState() { };
		/** @brief ApplicationState Dtor
		*
		*	Called on program close
		*/
		~ApplicationState() { };

	};
}

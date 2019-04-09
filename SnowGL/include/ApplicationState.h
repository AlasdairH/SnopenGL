#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"

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

		bool isRunning			= true;		/**< The running state of the program */
		bool isUIHidden			= false;	/**< Whether the UI is hidden or not */

		// window
		glm::vec2 windowSize;	/**< The current window size */
		/** @brief Gets the aspect ratio
		*	@return The aspect ratio of the window
		*
		*	Gets the aspect ratio
		*/
		float getAspectRatio() { return windowSize.x / windowSize.y; }

		// performance
		float deltaTime;			/**< The delta time between frames */
		float framesPerSecond;		/**< The frames per second */

		// input
		glm::vec2 mousePosition;									/**< The current mouse position */
		glm::vec2 lastMousePosition = glm::vec2(640, 360);			/**< The last mouse position ID */
		glm::vec2 mouseOffset;										/**< The difference between the current and last mouse position */

		// gl
		std::string curBoundTexture = "None";						/**< The currently bound texture (not used) */

		/** @brief Gets the current scene mode
		*	@return The current scene mode
		*
		*	Returns the scene mode
		*/
		inline SceneMode getSceneMode() { return m_sceneMode; }	
		
		/** @brief Toggle the scene mode
		*
		*	Toggles the scene mode between view and edit
		*/
		void switchSceneMode();									

		// ------------------------------------------------
		ApplicationState(ApplicationState const&) = delete;
		void operator=(ApplicationState const&) = delete;

	protected:

		SceneMode m_sceneMode = MODE_VIEW;			/**< The current scene mode */			


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

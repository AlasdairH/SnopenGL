#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"
#include "ParticleSystem.h"
#include "ApplicationState.h"
#include "Camera.h"

namespace SnowGL
{
	/*! @class GUI
	*	@brief The ImGUI wrapper for the SnowGL settings
	*
	*	This class wraps Dear ImGui and provides a GUI for the SnowGL settings
	*/
	class GUI
	{
	public:
		/** @brief GUI Ctor
		*
		*	Creates and initialises ImGUI
		*/
		GUI(SDL_Window *_window);
		/** @brief GUI Dtor
		*
		*	closes the ImGUI context
		*/
		~GUI();

		/** @brief Updates the GUI
		*
		*	Runs the immediate mode GUI code for setting the GUI
		*/
		void onUpdate();
		/** @brief Renders the GUI
		*
		*	Renders the GUI with modern OpenGL
		*/
		void onRender();

		/** @brief Renders the GUI
		*	@param _system The settings for the selected particle system
		*
		*	The selected particle system settings
		*/
		inline void setSelectedParticleSystem(std::shared_ptr<ParticleSystem> _system) { m_selectedParticleSystem = _system; }

	protected:
		SDL_Window				*m_window;		/**< The SDL window to render to */
		ApplicationState		*m_state;		/**< The state of the application */

		float f1, f2, f3 = 0.0f;

		std::array<float, 50>	m_fpsValues;

		std::shared_ptr<ParticleSystem> m_selectedParticleSystem;
	};
}
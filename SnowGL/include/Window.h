#pragma once

// cstdlib


// external libs

// program
#include "PCH.h"

namespace SnowGL
{
	/*! @class Window
	*	@brief An abstraction of the SDL Window class which can be used to create a window for OpenGL rendering
	*
	*	This class uses the SDL Window class to create a window in the constructor. By use of default parameters,
	*	a default window can be created by passing no arguments to the constructor or various settings can be applied.
	*	An OpenGL Context is created with the window for rendering.
	*/
	class Window
	{
	public:
		/** @brief The Window Constructor
		*	@param _title The Window Title that will be displayed in the OS
		*	@param _width The default width of the window
		*	@param _height The default height of the window
		*
		*	The constructor takes default arguments for a standard window but title, width and height can be set
		*	by overwriting these provided values.
		*	The constructor creates an SDL Window, SDL Renderer and an accompanying GL Context.
		*/
		Window(const std::string _title = "Default", const int _width = 1920, const int _height = 1080);

		/** @brief The Window Destructor
		*
		*	This Destructor simply destorys the SDL Window and Renderer.
		*/
		~Window();

		/** @brief Window Buffer Swap
		*
		*	This method simply swaps the window buffer. Should be called per frame.
		*/
		void swapBuffer();

		/** @brief Sets a new window size
		*	@param _width The new width of the window
		*	@param _height The new height of the window
		*
		*	Resizes the window and applies the info to OpenGL
		*/
		void resize(int _width, int _height);

		/** @brief Get the window width
		*	@return An integar containing the window width
		*
		*	Returns the window width.
		*/
		inline int getWidth() { return m_width; }
		/** @brief Get the window Height
		*	@return An integar containing the window height
		*
		*	Returns the window height.
		*/
		inline int getHeight() { return m_height; }		
		
		/** @brief Get a pointer to the SDL Window
		*	@return A pointer to the SDL window
		*
		*	Get a pointer to the SDL Window. Used by IMGUI.
		*/
		inline SDL_Window * getWindowPtr() { return m_window; }

	protected:
		SDL_Window		*m_window;				/**< A pointer to the SDL_Window. */
		SDL_Renderer	*m_renderer;			/**< A pointer to the SDL_Renderer. */
		SDL_GLContext	 m_openGLContext;		/**< The GL Context for OpenGL Rendering. */

		int				 m_width;				/**< The Window Width */
		int				 m_height;				/**< The Window Width */
	};
}
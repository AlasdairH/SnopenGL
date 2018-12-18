#include "PCH.h"
#include "Window.h"

namespace SnowGL
{
	Window::Window(const std::string _title, const int _width, const int _height)
	{
		m_window = SDL_CreateWindow(_title.c_str(),
			50, 50,
			_width, _height,
			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

		m_renderer = SDL_CreateRenderer(m_window, -1, 0);
		m_openGLContext = SDL_GL_CreateContext(m_window);

		resize(_width, _height);
	}

	Window::~Window()
	{
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
	}

	void Window::swapBuffer()
	{
		SDL_GL_SwapWindow(m_window);
	}

	void Window::resize(int _width, int _height)
	{
		m_width = _width;
		m_height = _height;

		// TODO: RESIZE

		CONSOLE_MESSAGE("Window resised to: " << m_width << ", " << m_height);

	}
}
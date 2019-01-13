#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"

namespace SnowGL
{
	class GUI
	{
	public:
		GUI(SDL_Window *_window);
		~GUI();

		void onUpdate();
		void onRender();

	protected:
		SDL_Window *m_window;
	};
}
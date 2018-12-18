// cstdlib

// external libs

// program
#include "PCH.h"

#undef main

int main()
{
	SnowGL::InitManager::initSDL();

	SnowGL::Window window("SnowGL");

	SnowGL::InitManager::initOpenGL();

	system("PAUSE");

	return 0;
}
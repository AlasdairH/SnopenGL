#pragma once

// cstdlib
#include <iostream>
#include <string>
#include <memory>

// external libs
#include "SDL/SDL.h"
#include "GL/glew.h"

// program
#include "Window.h"
#include "InitManager.h"

#define CONSOLE_MESSAGE(_text) std::cout << "MESSAGE: " << _text << std::endl;
#define CONSOLE_WARNING(_text) std::cout << "WARNING: " << _text << std::endl;
#define CONSOLE_ERROR(_text) std::cout << "ERROR: " << _text << std::endl;
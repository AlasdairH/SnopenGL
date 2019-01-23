#pragma once

// cstdlib
#include <algorithm>
#include <array>
#include <chrono>
#include <functional>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <math.h>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

// external libs
#include "SDL/SDL.h"
#include "GL/glew.h"
#include "GLM/glm.hpp"
#include "GLM/ext.hpp"

#include "external/imgui.h"
#include "external/imgui_impl_sdl_gl3.h"

// program
#include "IOUtilities.h"
#include "CompileSettings.h"

#define CONSOLE_MESSAGE(_text) std::cout << "MESSAGE: " << _text << std::endl;
#define CONSOLE_WARNING(_text) std::cout << "WARNING: " << _text << std::endl;
#define CONSOLE_ERROR(_text) std::cout << "ERROR: " << _text << std::endl;

#define TICK_INTERVAL 30

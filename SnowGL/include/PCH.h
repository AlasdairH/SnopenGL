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
#include "Debug.h"
#include "Log.h"
// benchmarking
#include "LogFile.h"
#include "SceneDump.h"
#include "Timer.h"
#include "GPU_Timer.h"

#ifdef ENABLE_BENCHMARK
#define COLLISION_BENCHMARK_ITERATIONS 1
#endif

#define TICK_INTERVAL 30

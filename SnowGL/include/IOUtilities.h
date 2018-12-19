#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"
#include "Mesh.h"

namespace SnowGL
{
	class IOUtilities
	{
	public:
		// TODO: Doxygen
		static std::string loadText(std::string _filepath);
		static void loadMesh(Mesh &_mesh, const std::string &_filepath);

		/** @brief Splits a string based on a delimiter
		*	@param _s The string to split
		*	@param _delimiter The character delimiter to split the string around
		*	@return An vector of individual strings
		*
		*	Loads a text file into an std::string.
		*/
		static std::vector<std::string> split(const std::string &_s, char _delimiter);
	};
}

#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"
#include "Mesh.h"
#include "Renderable.h"

namespace SnowGL
{
	/*! @class IOUtilities
	*	@brief Contains utilities for the input and output of files
	*
	*	This class contains static methods for input and output of different types of files
	*/
	class IOUtilities
	{
	public:
		/** @brief Loads text from a file to a string
		*	@param _filepath The path to the text file
		*	@return A string containing the text
		*
		*	Loads a text file from memory and into a string
		*/
		static std::string loadText(std::string _filepath);

		/** @brief Loads a triangulated .obj from a file to a mesh
		*	@param _mesh The mesh to load to
		*	@param _filepath The path to the .obj file
		*
		*	Loads from a .obj file to a mesh provided by reference.
		*	Obj files must be triangulated.
		*/
		static void loadMesh(Mesh &_mesh, const std::string &_filepath);

		/** @brief Loads a triangulated .obj from a file to a mesh
		*	@param _renderable The renderable to load to
		*	@param _filepath The path to the renderable file
		*
		*	Loads from a rednerable file to a renderable data structure provided by reference.
		*	Obj files must be triangulated.
		*/
		static void loadRenderable(Renderable &_renderable, const std::string &_filepath);

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

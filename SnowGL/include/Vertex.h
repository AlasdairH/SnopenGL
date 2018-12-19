#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"

namespace SnowGL
{
	/*! @class Vertex
	*	@brief A vertex in 3D space
	*
	*	Contains the data required to represent a single vertex in 3D space.
	*/
	struct Vertex
	{
		glm::vec3 position;				/**< The position of the Vertex */
		glm::vec2 textureCoordinate;	/**< The texture coordinates */
		glm::vec3 normal;				/**< The normal coordinates */

		/** @brief Operator Overload of ==
		*	@param _other The Vertex to compare against
		*	@return Bool indicating wether the two vertices are the same
		*
		*	Takes a vertex and compares it to the this vertex. If the two are the exact same, return true, else return false.
		*/
		bool operator==(const Vertex &_other)
		{
			if (position == _other.position && textureCoordinate == _other.textureCoordinate && normal == _other.normal)
			{
				return true;
			}
			return false;
		}
	};
}
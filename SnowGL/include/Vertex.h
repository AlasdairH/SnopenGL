#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"
#include "VertexBufferLayout.h"

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

		/** @brief Returns the layout for use in a VAO/VBO
		*	@return The layout of the struct
		*
		*	Returns the layout of the struct for use in the data layout sent to a VAO/VBO
		*/
		static VertexBufferLayout getLayout()
		{
			VertexBufferLayout layout;
			layout.push<glm::vec3>(1);	// position
			layout.push<glm::vec2>(1);	// texture coordinates
			layout.push<glm::vec3>(1);	// normal
			return layout;
		}

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
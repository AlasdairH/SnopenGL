#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"
#include "Vertex.h"
#include "VertexBufferLayout.h"

namespace SnowGL
{
	/*! @class Mesh
	*	@brief Data required for a mesh
	*
	*	Contains the data required for a mesh that can be loaded into a VertexBuffer.
	*/
	struct Mesh
	{
		std::vector<Vertex>				vertices;							/**< A vector of vertices that stores all the positions, UV data, etc of each vertex */
		std::vector<unsigned int>		indices;							/**< The indices for the index buffer that ties all the vertex data together */

		VertexBufferLayout				layout;								/**< The layout of the vertices member */

		glm::vec3						minBound = glm::vec3(INFINITY);		/**< The minimum bounding point of the mesh */
		glm::vec3						maxBound = glm::vec3(-INFINITY);	/**< The maximum bounding point of the mesh */
	};
}
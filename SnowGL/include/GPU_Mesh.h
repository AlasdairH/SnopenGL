#pragma once

// TODO: Doxygen

// cstdlib

// external libs

// program
#include "PCH.h"
#include "Vertex.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace SnowGL
{
	/*! @class GPU_Mesh
	*	@brief A mesh stored on the GPU
	*
	*	This class represents a mesh on the GPU, only storing the nessesary buffers
	*	for rendering. Mesh data on the CPU is stored in the Mesh class.
	*/
	class GPU_Mesh
	{
		friend class Renderer;

	public:
		/** @brief GPU_Mesh Ctor
		*
		*	Instantiates the various buffers on the GPU
		*/
		GPU_Mesh();

		/** @brief set the mesh data on the GPU from a mesh stored in regular memory
		*	@param _mesh The mesh to load to the GPU
		*
		*	Adds the mesh VBO to the VAO with the mesh vertex layout attributes.
		*/
		void setMesh(const Mesh &_mesh);

		// TODO: Doxygen
		inline std::shared_ptr<VertexBuffer> getVBO() { return m_VBO; }

		/** @brief Gets the vertex count of the object
		*	@return The vertex count
		*
		*	Returns the number of vertices on the object
		*/
		inline int getVertexCount() { return m_vertexCount; }

	protected:
		std::shared_ptr<VertexArray>	m_VAO;					/**< The Vertex Array Object */
		std::shared_ptr<VertexBuffer>	m_VBO;					/**< The Vertex Buffer Object which contains interleaved vertex data */
		std::shared_ptr<VertexBuffer>	m_IBO;					/**< The index buffer data for the model */

		int								m_vertexCount = -1;		/**< The number of vertices of the model */
	};
}
#pragma once

// cstdlib

// external libs


// program
#include "PCH.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace SnowGL
{
	/*! @class VertexArray
	*	@brief Absraction of an OpenGL Vertex Array Object (VAO)
	*
	*	This class handles the creation and managment of VAOs in OpenGL. It works by assigning Vertex Buffers with a corresponding layout to an array attribute.
	*/
	class VertexArray
	{
	public:
		/** @brief VertexArray Ctor
		*
		*	Generates a Vertex Array and stores the ID in m_vertexArrayID.
		*/
		VertexArray();
		/** @brief VertexArray Dtor
		*
		*	Deletes the Vertex Array
		*/
		~VertexArray();

		/** @brief Adds a buffer to the VAO
		*	@param _vertexBuffer The vertex buffer to be added to the array.
		*	@param _layout The layout of the corresponding buffer.
		*
		*	Takes a VBO, binds it to the VAO and sets up the glVertexAttribPointer based on the layout provided. 
		*/
		void addBuffer(const VertexBuffer &_vertexBuffer, const VertexBufferLayout &_layout);

		/** @brief VertexArray Dtor
		*
		*	Binds the Vertex Array Object (VAO)
		*/
		inline void bind() const { glBindVertexArray(m_vertexArrayID); }
		/** @brief VertexArray Dtor
		*
		*	Unbinds the Vertex Array Object (VAO)
		*/
		inline void unBind() const { glBindVertexArray(0); }

	protected:
		GLuint	m_vertexArrayID;		/**< The OpenGL ID of the VAO */
	};
}
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

		/** @brief Deletes and reinstates the VAO as new
		*
		*	Deletes the VAO and then creates a new one and reassigns the ID
		*/
		void reset();

		/** @brief Adds a buffer to the VAO
		*	@param _vertexBuffer The vertex buffer to be added to the array.
		*	@param _layout The layout of the corresponding buffer.
		*
		*	Takes a VBO, binds it to the VAO and sets up the glVertexAttribPointer based on the layout provided. 
		*/
		void addBuffer(const VertexBuffer &_vertexBuffer, const VertexBufferLayout &_layout);

		/** @brief Manually set the attribute array
		*	@param _index Specifies the index of the generic vertex attribute to be modified.
		*	@param _size Specifies the number of components per generic vertex attribute.
		*	@param _type Specifies the data type of each component in the array.
		*	@param _normalised Specifies whether fixed-point data values should be normalized.
		*	@param _stride Specifies the byte offset between consecutive generic vertex attributes.
		*	@param _pointer Specifies a offset of the first component of the first generic vertex attribute.
		*
		*	Manually set the attribute array.
		*/
		void setAttribArray(GLuint _index, GLint _size, GLenum _type, GLboolean _normalised, GLsizei _stride, const GLvoid *_pointer);

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

		/** @brief Returns OpenGL ID of the array buffer
		*	@return The OpenGL ID
		*
		*	Gets the buffer ID
		*/
		inline GLuint getGLID() { return m_vertexArrayID; }

	protected:
		GLuint	m_vertexArrayID;		/**< The OpenGL ID of the VAO */
	};
}
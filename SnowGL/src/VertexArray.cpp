#include "PCH.h"
#include "VertexArray.h"

namespace SnowGL
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_vertexArrayID);
		CONSOLE_MESSAGE("Vertex Array Object with ID: " << m_vertexArrayID << " Created");
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_vertexArrayID);
		CONSOLE_MESSAGE("Vertex Array Object with ID: " << m_vertexArrayID << " Deleted");
	}

	void VertexArray::reset()
	{
		glDeleteVertexArrays(1, &m_vertexArrayID);
		glGenVertexArrays(1, &m_vertexArrayID);
	}

	void VertexArray::addBuffer(const VertexBuffer & _vertexBuffer, const VertexBufferLayout & _layout)
	{
		// bind the VAO
		bind();
		// bind the VBO to be added
		_vertexBuffer.bind();
		// get the elements of the layout (vertex data, normal data, UV data, etc)
		const auto &elements = _layout.getElements();

		// set the initial offset from the start of the buffer
		unsigned int offset = 0;

		// loop for each element
		for (unsigned int i = 0; i < elements.size(); ++i)
		{
			// enable a attrib array for the element
			glEnableVertexAttribArray(i);
			// set up the layout (index, size, type, normalised, stride, offset ptr)
			// it is here that the buffer is linked to the VAO (Whatever buffer is bound to GL_ARRAY_BUFFER is added)
			glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalised, _layout.getStride(), (const void*)offset);
			// increase the offset for the next element to begin
			offset += elements[i].count * VertexBufferElement::getSizeOfType(elements[i].type);
		}
	}
}
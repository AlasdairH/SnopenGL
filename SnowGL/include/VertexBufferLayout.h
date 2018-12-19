#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"

namespace SnowGL
{
	/*! @class VertexBufferElement
	*	@brief Represents an element of a layout of data stored in a VAO.
	*
	*	This class is used in the VertexBufferLayout class to store an element of a layout
	*/
	struct VertexBufferElement
	{
		GLuint type;				/**< The data type of the element */
		GLuint count;				/**< The number of the specified type stored in the element (eg, 3 for a vec3 as it is 3 floats) */
		unsigned char normalised;	/**< Wether the data is normalised (between -1.0 and 1.0) */

		/** @brief Gets the size of a supported element type
		*	@param _type The type to return the size of
		*	@return the byte size of the type specified by the parameter
		*
		*	Takes in a data type and returns the size of that type. This method will only return the size of supported data types that can be added to a VBO (layout or actual VBO).
		*/
		static unsigned int getSizeOfType(unsigned int _type)
		{
			switch (_type)
			{
				case GL_FLOAT: return sizeof(float);
				case GL_UNSIGNED_INT: return sizeof(GLuint);
				case GL_UNSIGNED_BYTE: return sizeof(unsigned char);
			}
			return 0;
		}
	};


	
	/*! @class VertexBufferLayout
	*	@brief Abstraction of the layout of a VBO.
	*
	*	This class stores a vector of elements that make up the layout of data stored in a VBO.
	*	The push function adds a new element of the templated type to the VBO layout.
	*/
	class VertexBufferLayout
	{
	public:
		VertexBufferLayout()
			: m_stride(0) { }

		/** @brief Pushes a new element to the layout of the specified type
		*	@param _count The number of values in the element. Eg, 3 for a vec3 (3 floats), 2 for a vec2 (2 floats).
		*
		*	Pushes a new element into the layout of the templated type and with the specified count (how many items of the specified type make up the element).
		*/
		template<typename T>
		void push(unsigned int _count)
		{
			static_assert(false);
		}
		/** @brief Pushes a new float element to the layout
		*	@param _count The number of values in the element. Eg, 3 for a vec3 (3 floats), 2 for a vec2 (2 floats).
		*
		*	Pushes a new float element into the layout with the specified count (how many items of the specified type make up the element).
		*/
		template<>
		void push<float>(unsigned int _count)
		{
			m_elements.push_back({ GL_FLOAT, _count, GL_FALSE });
			m_stride += VertexBufferElement::getSizeOfType(GL_FLOAT) * _count;
		}
		/** @brief Pushes a new GLuint element to the layout
		*	@param _count The number of values in the element. Eg, 3 for a vec3 (3 floats), 2 for a vec2 (2 floats).
		*
		*	Pushes a new GLuint element into the layout with the specified count (how many items of the specified type make up the element).
		*/
		template<>
		void push<GLuint>(unsigned int _count)
		{
			m_elements.push_back({ GL_UNSIGNED_INT, _count, GL_FALSE });
			m_stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT) * _count;
		}
		/** @brief Pushes a new unsigned char element to the layout
		*	@param _count The number of values in the element. Eg, 3 for a vec3 (3 floats), 2 for a vec2 (2 floats).
		*
		*	Pushes a new unsigned char element into the layout with the specified count (how many items of the specified type make up the element).
		*/
		template<>
		void push<unsigned char>(unsigned int _count)
		{
			m_elements.push_back({ GL_UNSIGNED_BYTE, _count, GL_TRUE });
			m_stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE) * _count;
		}

		/** @brief Returns the vector of elements that make up the layout
		*
		*	Returns the vector that contains the elements of the layout. These can be read by the VAO class and applied to a VBO.
		*/
		inline const std::vector<VertexBufferElement> getElements() const { return m_elements; }

		/** @brief Returns the stride of the layout
		*
		*	Returns the stride of the layout (the distance in bytes between one item in the data and the next corresponding item)
		*/
		inline GLuint getStride() const { return m_stride; }

		/** @brief Resets the layout
		*
		*	Resets the layout back to default
		*/
		inline void reset() { m_stride = 0; m_elements.clear(); }



	protected:
		std::vector<VertexBufferElement>	m_elements;			/**< The vector containing the elements that make up the layout */
		GLuint								m_stride;			/**< The stride between data in the layout */
	};
}
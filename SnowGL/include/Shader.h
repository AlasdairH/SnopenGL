#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"

namespace SnowGL
{
	enum ShaderType { SHADER_VERTEX = GL_VERTEX_SHADER, SHADER_FRAGMENT = GL_FRAGMENT_SHADER };

	/*! @class Shader
	*	@brief Abstracted OpenGL Shader
	*
	*	Abstracts and provides simple utilities for an OpenGL Shader to be used by a shader program.
	*/
	class Shader
	{
	public:
		/** @brief Shader Ctor
		*	@param _shaderType The Type of Shader to Create (vertex, fragment, etc)
		*
		*	Creates an OpenGL shader of the specified type and stores the OpenGL ID in m_shaderID.
		*/
		Shader(ShaderType _shaderType);		
		/** @brief Shader Ctor
		*	@param _shaderType The Type of Shader to Create (vertex, fragment, etc)
		*	@param _filepath The filepath to the shader text file to load
		*
		*	Creates an OpenGL shader of the specified type and stores the OpenGL ID in m_shaderID.
		*/
		Shader(ShaderType _shaderType, const std::string &_filepath);

		/** @brief Shader Dtor
		*
		*	Destorys the shader
		*/
		~Shader() { glDeleteShader(m_shaderID); CONSOLE_MESSAGE("Deleted Shader: " << m_shaderID); }

		/** @brief Loads a text file into the Shader
		*	@param _filepath The path to the Text File to be loaded
		*	@return the result of the load. True for sucessful, false for error.
		*
		*	Uses the file manager service to load a text file and places it onto the GPU. The shader is then verified to ensure the shader source is valid.
		*/
		bool load(std::string _filepath);

		/** @brief Loads a text file into the Shader
		*	@return The result of the verification. True for sucessful, false for error.
		*
		*	Verifies the shader complination and insures it will run.
		*/
		bool verify();

		/** @brief Gets the shader ID
		*	@return The shader ID
		*
		*	Returns the Shader ID
		*/
		GLuint getShaderID() { return m_shaderID; }

		/** @brief Gets the shader type
		*	@return The shader type
		*
		*	Returns the Shader Type
		*/
		ShaderType getShaderType() { return m_shaderType; }

	protected:
		/** @brief Initialises the Shader
		*
		*	Creates the shader on the GPU. This is seperated from the Constructor as initialising there was causing issues.
		*/
		void init();

		bool		m_verified = false;	/**< Flag to indiciate the verification state of the shader */

		ShaderType	m_shaderType;		/**< The Shader Type */
		GLuint		m_shaderID;			/**< The OpenGL ID */
	};
}
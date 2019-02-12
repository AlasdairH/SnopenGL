#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"
#include "GPU_Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Transform.h"

namespace SnowGL
{
	/*! @class Renderable
	*	@brief Renderable class containing all the data needed to render a model
	*
	*	This class contains all the data that is required to render a model using the Renderer class.
	*/
	class Renderable
	{
	friend class Renderer;

	public:
		/** @brief Renderable Ctor
		*
		*	Does nothing
		*/
		Renderable();
		/** @brief Renderable Ctor
		*	@param _mesh The GPU based mesh to render
		*	@param _shader The shader to use when rendering
		*	@param _texture The diffuse texture
		*
		*	Creates a renderable with the parameters supplied
		*/
		Renderable(const GPU_Mesh &_mesh, const ShaderProgram &_shader, const Texture &_texture);

		Transform transform;	/**< The renderable objects transform */

		/** @brief Mesh setter
		*	@param _mesh The mesh to set
		*
		*	Sets the object's mesh
		*/
		inline void setMesh(std::shared_ptr<GPU_Mesh> _mesh) { m_mesh = _mesh; }
		/** @brief Mesh setter
		*	@param _shader The shader to set
		*
		*	Sets the object's shader
		*/
		inline void setShader(std::shared_ptr<ShaderProgram> _shader) { m_shader = _shader; }
		/** @brief Texture setter
		*	@param _texture The texture to set
		*
		*	Sets the object's texture
		*/
		inline void setTexture(std::shared_ptr<Texture> _texture) { m_texture = _texture; }

		/** @brief Gets the objects mesh
		*	@return The GPU mesh of the object
		*
		*	Gets a shared pointer to the objects mesh
		*/
		inline std::shared_ptr<GPU_Mesh> getGPUMesh() { return m_mesh; }

		/** @brief Gets the objects shader
		*	@return The Shader of the object
		*
		*	Gets a shared pointer to the object's Sahder
		*/
		inline std::shared_ptr<ShaderProgram> getShader() { return m_shader; }

	protected:
		std::shared_ptr<GPU_Mesh>			m_mesh;		/**< The mesh */
		std::shared_ptr<ShaderProgram>		m_shader;	/**< The shader */
		std::shared_ptr<Texture>			m_texture;	/**< The texture */

	};
}
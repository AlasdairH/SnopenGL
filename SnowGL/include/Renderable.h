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
	class Renderable
	{
	friend class Renderer;

	public:
		Renderable();
		Renderable(const GPU_Mesh &_mesh, const ShaderProgram &_shader, const Texture &_texture);

		Transform transform;

		inline void setMesh(std::shared_ptr<GPU_Mesh> _mesh) { m_mesh = _mesh; }
		inline void setShader(std::shared_ptr<ShaderProgram> _shader) { m_shader = _shader; }
		inline void setTexture(std::shared_ptr<Texture> _texture) { m_texture = _texture; }

		inline std::shared_ptr<GPU_Mesh> getGPUMesh() { return m_mesh; }

	protected:
		std::shared_ptr<GPU_Mesh>			m_mesh;
		std::shared_ptr<ShaderProgram>		m_shader;
		std::shared_ptr<Texture>			m_texture;

	};
}
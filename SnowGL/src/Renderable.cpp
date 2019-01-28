#include "PCH.h"
#include "Renderable.h"

namespace SnowGL
{
	Renderable::Renderable()
	{

	}

	Renderable::Renderable(const GPU_Mesh &_mesh, const ShaderProgram &_shader, const Texture &_texture)
	{
		m_mesh = std::make_shared<GPU_Mesh>(_mesh);
		m_shader = std::make_shared<ShaderProgram>(_shader);
		m_texture = std::make_shared<Texture>(_texture);
	}

}
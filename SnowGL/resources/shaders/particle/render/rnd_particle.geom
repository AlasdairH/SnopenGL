#version 330

layout (std140) uniform u_camera_data
{ 
	mat4 viewMatrix;
	mat4 projectionMatrix;
	mat4 viewProjectionMatrix;
};

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform float particle_size = 0.02f;
uniform float particle_size2 = 0.05f;
uniform mat4 u_modelMatrix;

out vec2 txcoord;

void main() 
{
	vec4 position = gl_in[0].gl_Position;
	// TODO: ?Randomise? particle size
	float activeParticleSize = particle_size2;

	// a: left-bottom 
	vec2 va = position.xy + vec2(-0.5, -0.5) * activeParticleSize;
	gl_Position = projectionMatrix * vec4(va, position.zw);
	txcoord = vec2(0.0, 0.0);
	EmitVertex();  
  
	// b: left-top
	vec2 vb = position.xy + vec2(-0.5, 0.5) * activeParticleSize;
	gl_Position = projectionMatrix * vec4(vb, position.zw);
	txcoord = vec2(0.0, 1.0);
	EmitVertex();  
  
	// d: right-bottom
	vec2 vd = position.xy + vec2(0.5, -0.5) * activeParticleSize;
	gl_Position = projectionMatrix * vec4(vd, position.zw);
	txcoord = vec2(1.0, 0.0);
	EmitVertex();  

	// c: right-top
	vec2 vc = position.xy + vec2(0.5, 0.5) * activeParticleSize;
	gl_Position = projectionMatrix * vec4(vc, position.zw);
	txcoord = vec2(1.0, 1.0);
	EmitVertex();  

	EndPrimitive();  
};  
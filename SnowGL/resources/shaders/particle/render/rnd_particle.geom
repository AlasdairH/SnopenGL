#version 330

layout (std140) uniform u_camera_data
{ 
	mat4 viewMatrix;
	mat4 projectionMatrix;
	mat4 viewProjectionMatrix;
};

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform float particle_size = 1.0f;

out vec2 txcoord;

void main() 
{
	vec4 P = gl_in[0].gl_Position;

	// a: left-bottom 
	vec2 va = P.xy + vec2(-0.5, -0.5) * particle_size;
	gl_Position = projectionMatrix * vec4(va, P.zw);
	txcoord = vec2(0.0, 0.0);
	EmitVertex();  
	
	// b: left-top
	vec2 vb = P.xy + vec2(-0.5, 0.5) * particle_size;
	gl_Position = projectionMatrix * vec4(vb, P.zw);
	txcoord = vec2(0.0, 1.0);
	EmitVertex();  
	
	// d: right-bottom
	vec2 vd = P.xy + vec2(0.5, -0.5) * particle_size;
	gl_Position = projectionMatrix * vec4(vd, P.zw);
	txcoord = vec2(1.0, 0.0);
	EmitVertex();  

	// c: right-top
	vec2 vc = P.xy + vec2(0.5, 0.5) * particle_size;
	gl_Position = projectionMatrix * vec4(vc, P.zw);
	txcoord = vec2(1.0, 1.0);
	EmitVertex();  

	EndPrimitive();  
};  
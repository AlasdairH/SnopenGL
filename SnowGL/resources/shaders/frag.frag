#version 430 core

uniform sampler2D u_texture;

uniform vec4 u_diffuse = { 1.0f, 0.78f, 0.91f, 1.0f };
uniform bool u_useTexture = false;
//uniform vec3 u_cameraPosition;
uniform mat4 u_m;

uniform float u_materialShininess = 0.1f;
uniform vec3 u_materialSpecularCol = { 1.0f, 1.0f, 1.0f };

uniform vec3 u_lightPosition = { 0, 0, 0 };

uniform vec3 u_viewPos = { 0, 2, 0 };

layout(location = 0) out vec4 color;

in vec2 frag_texCoord;
in vec3 frag_vert;
in vec3 frag_normal;

void main()
{
	vec4 surfaceColor;
	surfaceColor = u_diffuse;

    vec3 fragPosition = vec3(u_m * vec4(frag_vert, 1));

    color = texture(u_texture, frag_texCoord);
    color =	vec4(color.x, color.y, color.z, 1.0f);
} 
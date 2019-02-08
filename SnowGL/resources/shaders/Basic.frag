#version 430 core

uniform sampler2D u_diffuseTexture;
uniform sampler2D u_depthMap;

layout(location = 0) out vec4 color;

in vec2 frag_texCoord;
in vec3 frag_normal;
in vec3 frag_pos;
in vec4 frag_posLightSpace;

void main()
{
    color = texture(u_diffuseTexture, frag_texCoord);
} 
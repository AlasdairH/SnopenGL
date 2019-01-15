#version 430 core

uniform vec4 diffuseColour = vec4(0.00, 0.9, 0.0, 1.0);

layout(location = 0) out vec4 color;


void main()
{
    color = diffuseColour;
}

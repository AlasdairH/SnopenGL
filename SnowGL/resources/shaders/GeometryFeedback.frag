#version 430 core

in vec4 frag_colour;

out vec4 outputColour;

void main()
{
	//outputColour = frag_colour;
	outputColour = vec4(1.0, 1.0, 1.0, 0.0);
} 
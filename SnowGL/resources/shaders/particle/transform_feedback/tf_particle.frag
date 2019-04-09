#version 430

in vec4 particleColour;

out vec4 out_Colour;

void main(void)
{
	out_Colour = particleColour;
}
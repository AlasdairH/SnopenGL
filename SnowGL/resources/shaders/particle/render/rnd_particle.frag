#version 430

uniform sampler2D snowflakeTexture;

in vec2 txcoord;

out vec4 out_Colour;

void main(void)
{
	vec2 uv = txcoord;
	//uv.y *= -1.0;
	vec4 t = texture(snowflakeTexture, uv);
	//vec4 t = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	out_Colour = vec4(t);
}
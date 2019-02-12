#version 430 core

out vec4 fragColor;
  
in vec2 texCoords;

uniform sampler2D screenTexture;

void main()
{ 
    float depthValue = texture(screenTexture, texCoords).r;
    fragColor = vec4(vec3(depthValue), 1.0);
}
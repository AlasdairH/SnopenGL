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

	// ambient
    float ambientStrength = 0.1;
	vec3 lightColour = { 1.0, 1.0, 1.0 };

    vec3 ambient = ambientStrength * lightColour;
  	
    // diffuse
    vec3 norm = normalize(frag_normal);
    vec3 lightDir = normalize(u_lightPosition - fragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColour;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_viewPos - fragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColour;  
        
    vec3 result = (ambient + diffuse + specular) * surfaceColor.xyz;
    color = vec4(result, 1.0);
} 
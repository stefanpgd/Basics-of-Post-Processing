#version 460 core
in vec3 Normal;
in vec2 UV;
in vec3 FragPos;

layout (location = 0) out vec4 FragColor;

uniform sampler2D texture_albedo;
uniform float emission;

void main()
{
	vec3 diffuseTex = texture(texture_albedo, UV).rgb;

    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
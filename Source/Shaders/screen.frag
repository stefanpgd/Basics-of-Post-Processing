#version 460 core
out vec4 FragColor;
  
in vec2 uv;

uniform sampler2D sceneTexture;

uniform float exposure;
uniform vec2 rOffset;
uniform vec2 gOffset;
uniform vec2 bOffset;

uniform bool centeredCA;
uniform float centeredCAStrength;

void main()
{ 
    vec3 color = texture(sceneTexture, uv).rgb;
    FragColor = vec4(color, 1.0);
}
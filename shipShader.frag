#version 330 core

in vec3 normalColor;
in vec2 texCoords;
in vec3 pos;

out vec4 color;

uniform sampler2D shipTex;

void main()
{
	vec3 newNormal = normalize(normalColor);
	color = vec4(vec3(0.157f, 0.192f, 0.212f), 1.0f);
}
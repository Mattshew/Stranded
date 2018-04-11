#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

// Uniform variables can be updated by fetching their location and passing values to that location
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform mat4 modelview;
uniform vec4 clipPlane;

out vec3 normalColor;
out vec2 texCoords;
out vec3 pos;

void main()
{
	// OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
	vec4 loc = model * vec4(position, 1.0f);
	texCoords = texCoord;
	pos = position;
	gl_Position = projection * modelview * vec4(position, 1.0);
	gl_ClipDistance[0] = dot(loc, clipPlane);
	normalColor = mat3(transpose(inverse(modelview))) * normal;
}

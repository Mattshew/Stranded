#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec3 camPos;
uniform vec4 clipPlane;

out vec3 pos;
out float y;
out vec3 norm;
out vec3 viewV;

void main()
{   
	y = position.y;
    
    gl_Position = projection * model * view * vec4(position, 1.0);
    gl_ClipDistance[0] = dot(model * vec4(position, 1.0f), clipPlane);
    pos = position;
    norm = normal;
    viewV = camPos;
}
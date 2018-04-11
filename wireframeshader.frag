#version 330 core

out vec4 color;

uniform vec3 colorCollide;

void main()
{
    color = vec4(colorCollide,1.0f);
}
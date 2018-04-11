#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform mat4 modelview;
uniform sampler2D dmap;
uniform sampler2D nmap;

uniform vec3 camPos;
uniform float time;
uniform bool isDmap;
uniform bool isWave;

out vec4 clipPlane;

out vec2 texCoords;
out vec3 pos;
out vec2 distortion1;
out vec2 distortion2;
out float tempTime;

out vec3 reflection;
out vec3 refraction;
out float fresnel;

const float temp = 0.15; // Water constant

void main()
{
	float dispY, dudvY, move;
	texCoords = texCoord;
	gl_Position = projection * modelview * vec4(position, 1.0f);

	pos = position;
	vec3 normal;
	tempTime = time;

	if (isDmap == true)
	{
		dispY = position.y * 2.0f + texture(dmap, texCoords).r * 5.0f * sin(time);
		gl_Position = projection * modelview * vec4(position.x, dispY, position.z, 1.0f);
	}

	normal = vec3(0.0f, 1.0f, 0.0f);
    vec3 cameraDir = normalize(camPos - position);

	refraction = normalize(refract(normal, cameraDir, temp));
	cameraDir.y = -cameraDir.y;
    reflection = normalize(reflect(-normal, cameraDir));

	clipPlane = projection * modelview * vec4(position.x, position.y, position.z, 1.0f);

	fresnel = temp + (1.0f - temp) * pow(max(0.0f, 1.0f - dot(-cameraDir, normal)), 2.0);
}
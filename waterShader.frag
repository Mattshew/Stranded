#version 330 core

in vec4 clipPlane;
in vec2 texCoords;
in vec3 pos;

in vec3 reflection;
in vec3 refraction;
in float fresnel;
in float tempTime;

out vec4 color;

uniform sampler2D dudvMap;
uniform sampler2D reflectTex;
uniform sampler2D refractTex;
uniform bool isWave;

void main()
{
	vec2 calc = (clipPlane.xy / clipPlane.w) / 2.0f + 0.5f;
	vec2 reflectTexCoords = vec2(calc.x, 1.0f - calc.y);
	vec2 refractTexCoords = vec2(calc.x, calc.y);

	if (isWave == true)
	{
		vec2 distortion1 = (texture(dudvMap, texCoords).rg * 2.0f - 1.0f) * 0.02f * sin(tempTime);
		vec2 distortion2 = (texture(dudvMap, vec2(-texCoords.x, texCoords.y)).rg * 2.0f - 1.0f) * 0.02f * sin(tempTime);
		vec2 totalDistortion = distortion1 + distortion2;

		reflectTexCoords += totalDistortion;
		refractTexCoords += totalDistortion;
	}

	vec4 reflectionColor = texture(reflectTex, reflectTexCoords);
	vec4 refractionColor = texture(refractTex, refractTexCoords);

	color = mix(refractionColor, reflectionColor, fresnel);
	color = mix(color, vec4(0.0f, 0.3f, 0.5f, 1.0f), 0.2f);
}
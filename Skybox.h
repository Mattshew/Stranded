#ifndef SKYBOX_H
#define SKYBOX_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h> // Remove this line in future projects
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Window.h"

class Skybox
{
private:
	glm::mat4 toWorld;

public:
	Skybox();
	~Skybox();

	void draw(GLuint skyboxShader);

	unsigned int getCubeMap();
	unsigned int loadCubemap(std::vector<std::string> faces);

	std::vector<std::string> faces = {
		"C:/Users/Matthew/Documents/_UCSD/CSE 167/CSE167Final/ss_rt.ppm",
		"C:/Users/Matthew/Documents/_UCSD/CSE 167/CSE167Final/ss_lf.ppm",
		"C:/Users/Matthew/Documents/_UCSD/CSE 167/CSE167Final/ss_up.ppm",
		"C:/Users/Matthew/Documents/_UCSD/CSE 167/CSE167Final/ss_dn.ppm",
		"C:/Users/Matthew/Documents/_UCSD/CSE 167/CSE167Final/ss_bk.ppm",
		"C:/Users/Matthew/Documents/_UCSD/CSE 167/CSE167Final/ss_ft.ppm"
	};

	unsigned int cubemapTexture;
	GLuint skyboxVBO, skyboxVAO;
	GLuint uProjection, uModelview;
};

const float skyboxVertices[] = {
// positions
-500.0f,  500.0f, -500.0f,
-500.0f, -500.0f, -500.0f,
500.0f, -500.0f, -500.0f,
500.0f, -500.0f, -500.0f,
500.0f,  500.0f, -500.0f,
-500.0f,  500.0f, -500.0f,

-500.0f, -500.0f,  500.0f,
-500.0f, -500.0f, -500.0f,
-500.0f,  500.0f, -500.0f,
-500.0f,  500.0f, -500.0f,
-500.0f,  500.0f,  500.0f,
-500.0f, -500.0f,  500.0f,

500.0f, -500.0f, -500.0f,
500.0f, -500.0f,  500.0f,
500.0f,  500.0f,  500.0f,
500.0f,  500.0f,  500.0f,
500.0f,  500.0f, -500.0f,
500.0f, -500.0f, -500.0f,

-500.0f, -500.0f,  500.0f,
-500.0f,  500.0f,  500.0f,
500.0f,  500.0f,  500.0f,
500.0f,  500.0f,  500.0f,
500.0f, -500.0f,  500.0f,
-500.0f, -500.0f,  500.0f,

-500.0f,  500.0f, -500.0f,
500.0f,  500.0f, -500.0f,
500.0f,  500.0f,  500.0f,
500.0f,  500.0f,  500.0f,
-500.0f,  500.0f,  500.0f,
-500.0f,  500.0f, -500.0f,

-500.0f, -500.0f, -500.0f,
-500.0f, -500.0f,  500.0f,
500.0f, -500.0f, -500.0f,
500.0f, -500.0f, -500.0f,
-500.0f, -500.0f,  500.0f,
500.0f, -500.0f,  500.0f
};

#endif
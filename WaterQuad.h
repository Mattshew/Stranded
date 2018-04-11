#ifndef WATERQUAD_H
#define WATERQUAD_H

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

class WaterQuad
{
public:
	WaterQuad();
	~WaterQuad();

	void setIsDmap(bool);
	bool getIsDmap();
	void setIsWave(bool);
	bool getIsWave();
	unsigned int loadMap(std::vector<std::string> faces);
	void draw(GLuint texReflect, GLuint texRefract);

	std::vector<std::string> dispFace = {
		"C:/Users/Matthew/Documents/_UCSD/CSE 167/CSE167Final/Water_001_DISP.ppm"
	};

	std::vector<std::string> normFace = {
		"C:/Users/Matthew/Documents/_UCSD/CSE 167/CSE167Final/Water_001_NORM.ppm"
	};

	std::vector<std::string> dudvFace = {
		"C:/Users/Matthew/Documents/_UCSD/CSE 167/CSE167Final/waterDUDV.ppm"
	};

	unsigned int dmapTexture, nmapTexture, dudvmapTexture;

	glm::mat4 toWorld;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	float watertime = 0.0f;
	float waterHeight = 0.0f;
	bool isDmap = false;
	bool isWave = false;
	GLuint quadVAO, quadVBO, quadEBO, quadTBO;
	GLuint uProjection, uModelview, uCamPos, time, uDmap, uNmap, uIsDmap, uDUDVmap;
};

#endif
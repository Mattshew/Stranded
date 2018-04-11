#ifndef OBJOBJECT_H
#define OBJOBJECT_H

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
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class OBJObject
{
private:
	std::vector<unsigned int> indices, uvIndices, normalIndices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	glm::mat4 toWorld;

public:
	OBJObject(const char* filepath);
	~OBJObject();

	glm::mat4 getToWorld();
	void parse(const char* filepath);
	void draw(GLuint shaderProgram);
	unsigned int loadTexture(std::vector<std::string> faces);

	void translate(int key);
	void zoom(double);
	void rotate(int rotAng);

	std::vector<std::string> texFace = {
		"C:/Users/Matthew/Documents/_UCSD/CSE 167/CSE167Final/dark_wood_planks.ppm"
	};

	unsigned int texmap;

	int angle;
	float scale = 0.0f;
	glm::vec3 dir;

	float maxX;
	float minX;
	float maxY;
	float minY;
	float maxZ;
	float minZ;

	GLuint VBO, VAO, EBO, NBO, TBO;
	GLuint uProjection, uModelview;
};

#endif
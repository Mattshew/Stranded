#include "WaterQuad.h"
#include "Window.h"
#include "stb_image.h"

WaterQuad::WaterQuad()
{
	toWorld = glm::mat4(1.0f);
	float sizeW = 1000.0f;
	waterHeight = 0.0f;

	for (float i = -500.0f; i < 500.0f; i += 1.0f) {
		for (float j = -500.0f; j < 500.0f; j += 1.0f) {
			vertices.push_back(glm::vec3(i, 0.0f, j));
			texCoords.push_back(glm::vec2((i + 500) * 0.001, (j + 500) * 0.001));
		}
	}

	for (float i = 0; i < sizeW - 1; i += 1.0f) {
		for (float j = 0; j < sizeW - 1; j += 1.0f) {
			indices.push_back(i + j * sizeW);
			indices.push_back(i + 1 + j * sizeW);
			indices.push_back(i + (j + 1) * sizeW);

			indices.push_back(i + (j + 1) * sizeW);
			indices.push_back(i + 1 + j * sizeW);
			indices.push_back(i + 1 + (j + 1) * sizeW);
		}
	}

	dmapTexture = loadMap(dispFace);
	nmapTexture = loadMap(normFace);
	dudvmapTexture = loadMap(dudvFace);

	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glGenBuffers(1, &quadEBO);
	glGenBuffers(1, &quadTBO);

	glBindVertexArray(quadVAO);

	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, quadTBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), &texCoords[0], GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

WaterQuad::~WaterQuad()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
	glDeleteBuffers(1, &quadEBO);
	glDeleteBuffers(1, &quadTBO);
}

void WaterQuad::setIsDmap(bool dispmap)
{
	isDmap = dispmap;
}

bool WaterQuad::getIsDmap()
{
	return isDmap;
}

void WaterQuad::setIsWave(bool wavemap)
{
	isWave = wavemap;
}

bool WaterQuad::getIsWave()
{
	return isWave;
}

unsigned int WaterQuad::loadMap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	int width, height, nrChannels;
	unsigned char *data = stbi_load(faces[0].c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D,
			0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
		);
		stbi_image_free(data);
	}
	else
	{
		printf("Dmap texture failed to load at path: %s\n", faces[0].c_str());
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

	glDisable(GL_CULL_FACE);

	return textureID;
}

void WaterQuad::draw(GLuint texReflect, GLuint texRefract)
{
	glUseProgram(Window::waterQuadShader);

	glm::mat4 modelview = Window::V * toWorld;

	// Get the location of the uniform variables
	uProjection = glGetUniformLocation(Window::waterQuadShader, "projection");
	uModelview = glGetUniformLocation(Window::waterQuadShader, "modelview");
	uCamPos = glGetUniformLocation(Window::waterQuadShader, "camPos");
	time = glGetUniformLocation(Window::waterQuadShader, "time");
	uIsDmap = glGetUniformLocation(Window::waterQuadShader, "isDmap");
	uDmap = glGetUniformLocation(Window::waterQuadShader, "dmap");
	glUniform1i(uDmap, 1);
	uNmap = glGetUniformLocation(Window::waterQuadShader, "nmap");
	glUniform1i(uNmap, 2);
	uDUDVmap = glGetUniformLocation(Window::waterQuadShader, "dudvMap");
	glUniform1i(uDUDVmap, 3);

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(Window::waterQuadShader, "model"), 1, GL_FALSE, &toWorld[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(Window::waterQuadShader, "view"), 1, GL_FALSE, &Window::V[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	glUniform3fv(uCamPos, 1, &Window::cam_pos[0]);
	glUniform1f(time, watertime);
	watertime += 0.02f;
	if (isDmap == true) glUniform1i(uIsDmap, GL_TRUE);
	if (isDmap == false) glUniform1i(uIsDmap, GL_FALSE);

	if (isWave == true) glUniform1i(glGetUniformLocation(Window::waterQuadShader, "isWave"), GL_TRUE);
	if (isWave == false) glUniform1i(glGetUniformLocation(Window::waterQuadShader, "isWave"), GL_FALSE);

	// Fragment uniform variables
	glUniform1i(glGetUniformLocation(Window::waterQuadShader, "reflectTex"), 4);
	glUniform1i(glGetUniformLocation(Window::waterQuadShader, "refractTex"), 5);

	// ... set view and projection matrix
	glBindVertexArray(quadVAO);

	// Binding Textures
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, dmapTexture);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, nmapTexture);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, dudvmapTexture);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, texReflect);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, texRefract);

	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}
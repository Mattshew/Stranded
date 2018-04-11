#include "OBJObject.h"
#include "Window.h"
#include "stb_image.h"


static int window_width = 1920, window_height = 1080;
static float* pixels = new float[window_width * window_height * 3];

OBJObject::OBJObject(const char *filepath)
{
	toWorld = glm::mat4(1.0f);
	parse(filepath);
	texmap = loadTexture(texFace);

	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &NBO);
	glGenBuffers(1, &TBO);

	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); 

	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);
}

OBJObject::~OBJObject()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &NBO);
}

glm::mat4 OBJObject::getToWorld()
{
	return toWorld;
}

void OBJObject::parse(const char *filepath)
{
	//TODO parse the OBJ file
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
	FILE* fp;
	int c1, c2;    // characters read from file
	maxX, maxY, maxZ = -std::numeric_limits<float>::max();
	minX, minY, minZ = std::numeric_limits<float>::max();

	fp = fopen(filepath, "rb");
	if (fp == NULL) perror("Error opening file");
	else {
		while (!feof(fp)) {
			c1 = fgetc(fp);
			c2 = fgetc(fp);
			if (c1 == '#' || c1 == '\n') {
				int c3 = fgetc(fp);
				if (c3 == EOF) break;
			}
			else if (c1 == 'v' && c2 == ' ') {
				glm::vec3 vertex;
				fscanf(fp, "%f %f %f\r\n", &vertex.x, &vertex.y, &vertex.z);
				vertices.push_back(vertex);
			}
			else if (c1 == 'v' && c2 == 't') {
				glm::vec2 texCoord;
				fscanf(fp, "%f %f\r\n", &texCoord.x, &texCoord.y);
				uvs.push_back(glm::normalize(texCoord));
			}
			else if (c1 == 'v' && c2 == 'n') {
				glm::vec3 normal;
				fscanf(fp, "%f %f %f\r\n", &normal.x, &normal.y, &normal.z);
				normals.push_back(glm::normalize(normal));
			}
			else if (c1 == 'f' && c2 == ' ')
			{
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(fp, "%d/%d/%d %d/%d/%d %d/%d/%d\r\n", 
					&vertexIndex[0], &uvIndex[0], &normalIndex[0], 
					&vertexIndex[1], &uvIndex[1], &normalIndex[1], 
					&vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9) {
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
					return;
				}
				indices.push_back(vertexIndex[0] - 1);
				indices.push_back(vertexIndex[1] - 1);
				indices.push_back(vertexIndex[2] - 1);

				uvIndices.push_back(uvIndex[0] - 1);
				uvIndices.push_back(uvIndex[1] - 1);
				uvIndices.push_back(uvIndex[2] - 1);

				normalIndices.push_back(normalIndex[0] - 1);
				normalIndices.push_back(normalIndex[1] - 1);
				normalIndices.push_back(normalIndex[2] - 1);

				for (int i = 0; i < 3; ++i) {
					if (vertices[vertexIndex[i]].x > maxX) maxX = vertices[vertexIndex[i]].x;
					if (vertices[vertexIndex[i]].x < minX) minX = vertices[vertexIndex[i]].x;
					if (vertices[vertexIndex[i]].y > maxY) maxY = vertices[vertexIndex[i]].y;
					if (vertices[vertexIndex[i]].y < minY) minY = vertices[vertexIndex[i]].y;
					if (vertices[vertexIndex[i]].z > maxZ) maxZ = vertices[vertexIndex[i]].z;
					if (vertices[vertexIndex[i]].z < minZ) minZ = vertices[vertexIndex[i]].z;
				}
			}
			else {
				continue;
			}
		}
		fclose(fp);
	}
}

unsigned int OBJObject::loadTexture(std::vector<std::string> faces)
{
	unsigned int textureID;
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			printf("Cubemap texture failed to load at path: %s\n", faces[i].c_str());
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glDisable(GL_CULL_FACE);

	return textureID;
}


void OBJObject::draw(GLuint shaderProgram) 
{
	glUseProgram(shaderProgram);

	// Calculate the combination of the model and view (camera inverse) matrices
	glm::mat4 modelview = Window::V * toWorld; // change this to mat4
	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &toWorld[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &Window::V[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	glUniform1i(glGetUniformLocation(shaderProgram, "shipTex"), 0);

	glEnable(GL_CLIP_DISTANCE0);
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texmap);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glDisable(GL_CLIP_DISTANCE0);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}

void OBJObject::zoom(double zoffset)
{
	toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, zoffset)) * toWorld;
}

void OBJObject::translate(int key)
{
	if (key == GLFW_KEY_UP)
	{
		if (angle == 0)
			toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f)) * toWorld;
		if (angle == 90 || angle == -270)
			toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, 0.0f)) * toWorld;
		if (angle == 180 || angle == -180)
			toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.5f)) * toWorld;
		if (angle == 270 || angle == -90)
			toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f)) * toWorld;
	}
	if (key == GLFW_KEY_DOWN)
	{
		if (angle == 0)
			toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.5f)) * toWorld;
		if (angle == 90 || angle == -270)
			toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f)) * toWorld;
		if (angle == 180 || angle == -180)
			toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f)) * toWorld;
		if (angle == 270 || angle == -90)
			toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, 0.0f)) * toWorld;
	}
}

void OBJObject::rotate(int rotAng)
{
	int ang = 90 * rotAng;
	angle += ang;
	angle = angle % 360;
	toWorld = glm::rotate(toWorld, glm::radians((float)ang), glm::vec3(0.0f, 1.0f, 0.0f));
}
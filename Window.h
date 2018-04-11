#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <math.h>
#include <glm/gtx/string_cast.hpp> // glm::to_string
#include "Skybox.h"
#include "shader.h"
#include "WaterQuad.h"
#include "OBJObject.h"
#include "Terrain.h"
#include "BoundingBox.h"

class Window
{
public:
	static int width;
	static int height;
	static glm::vec3 cam_pos;
	static glm::vec3 cam_look_at;
	static glm::vec3 cam_up;
	static GLuint skyboxShader;
	static GLuint waterQuadShader;
	static GLuint shipShader;
	static GLuint terrainShader;

	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view

	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void drawOrder();
	static void setFBO(GLuint & FBO, GLuint & TBO);
	static void loadFBO();
	static void frameScene(bool isReflect, GLuint & tex, GLuint & TBO);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static glm::vec3 trackBallMapping(float xpos, float ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};

#endif

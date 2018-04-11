#include "Window.h"

const char* window_title = "GLFW Starter Project";

GLuint Window::skyboxShader;
GLuint Window::waterQuadShader; 
GLuint Window::shipShader;
GLuint Window::terrainShader;

Skybox * cube;
WaterQuad * quad;
OBJObject * ship;
Terrain * terrain;
BoundingBox * box;

float rotCount = 0.0f;
int tcount = 0;
int tinc = 0;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "C:/Users/Matthew/Documents/_UCSD/CSE 167/CSE167Final/skyboxShader.vert"
#define FRAGMENT_SHADER_PATH "C:/Users/Matthew/Documents/_UCSD/CSE 167/CSE167Final/skyboxShader.frag"
#define VERTEX_SHADER_PATH1 "C:/Users/Matthew/Documents/_UCSD/CSE 167/CSE167Final/waterShader.vert"
#define FRAGMENT_SHADER_PATH1 "C:/Users/Matthew/Documents/_UCSD/CSE 167/CSE167Final/waterShader.frag"
#define VERTEX_SHADER_PATH2 "C:/Users/Matthew/Documents/_UCSD/CSE 167/CSE167Final/shipShader.vert"
#define FRAGMENT_SHADER_PATH2 "C:/Users/Matthew/Documents/_UCSD/CSE 167/CSE167Final/shipShader.frag"
#define VERTEX_SHADER_PATH3 "C:/Users/Matthew/Documents/_UCSD/CSE 167/CSE167Final/terrainshader.vert"
#define FRAGMENT_SHADER_PATH3 "C:/Users/Matthew/Documents/_UCSD/CSE 167/CSE167Final/terrainshader.frag"

// Default camera parameters
glm::vec3 Window::cam_pos = glm::vec3(0.0f, 10.0f, 20.0f);		// e  | Position of camera
glm::vec3 Window::cam_look_at = glm::vec3(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 Window::cam_up = glm::vec3(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;

bool isPos = true;
bool isBox = false;
bool isTerrain = true;
bool isShip = true;
float count1 = 2.0f;
float inc = 2.0f;

GLuint reflect, refract, TBOreflect, TBOrefract;

double x_pos, y_pos;
glm::vec3 lastPoint;
int counter = 0;
float angle = 0.0f;

int movement = 0;
glm::vec3 lastpt;

glm::mat4 Window::P;
glm::mat4 Window::V;

void Window::initialize_objects()
{
	setFBO(reflect, TBOreflect);
	setFBO(refract, TBOrefract);

	cube = new Skybox();
	quad = new WaterQuad();
	ship = new OBJObject("C:/Users/Matthew/Documents/_UCSD/CSE 167/CSE167Final/ship.obj");
	terrain = new Terrain();
	box = new BoundingBox(ship);
	box->setHeightMap(terrain->getHeightMap(), terrain->SIZEMAP);

	skyboxShader = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	waterQuadShader = LoadShaders(VERTEX_SHADER_PATH1, FRAGMENT_SHADER_PATH1);
	shipShader = LoadShaders(VERTEX_SHADER_PATH2, FRAGMENT_SHADER_PATH2);
	terrainShader = LoadShaders(VERTEX_SHADER_PATH3, FRAGMENT_SHADER_PATH3);
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(cube);
	delete(quad);
	delete(ship);
	delete(terrain);
	delete(box);

	glDeleteProgram(skyboxShader);
	glDeleteProgram(waterQuadShader);
	glDeleteProgram(shipShader);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	Window::width = width;
	Window::height = height;

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{

}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	loadFBO();
	drawOrder();
	quad->draw(reflect, refract);
	if (isBox == true) box->draw();

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::drawOrder()
{
	cube->draw(skyboxShader);
	
	if (isTerrain == true) terrain->draw();
	if (isShip == true) ship->draw(shipShader);
}

void Window::setFBO(GLuint & FBO, GLuint & TBO) {
	glGenTextures(1, &TBO);
	glBindTexture(GL_TEXTURE_2D, TBO);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Window::width, Window::height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TBO, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Window::loadFBO() {
	frameScene(true, reflect, TBOreflect);
	frameScene(false, refract, TBOrefract);
}

void Window::frameScene(bool isReflect, GLuint & tex, GLuint & TBO) {
	glm::vec4 clipPlane;
	glm::vec3 camPos = Window::cam_pos;
	glm::vec3 camLookAt = Window::cam_look_at;
	glm::vec3 camUp = Window::cam_up;
	float waterHeight = quad->waterHeight;

	if (isReflect) {
		clipPlane = glm::vec4(0.0f, 1.0f, 0.0f, waterHeight);
		camPos.y = waterHeight - (camPos.y - waterHeight);
		camLookAt.y = waterHeight - (camLookAt.y - waterHeight);

	}
	else {
		clipPlane = glm::vec4(0.0f, -1.0f, 0.0f, waterHeight);
	}

	// store window parameter
	glm::mat4 tempW = Window::V;
	Window::V = glm::lookAt(camPos, camLookAt, camUp);

	glEnable(GL_CLIP_DISTANCE0);

	glUseProgram(Window::terrainShader);
	glUniform4f(glGetUniformLocation(Window::terrainShader, "clipPlane"), clipPlane.x, clipPlane.y, clipPlane.z, clipPlane.w);
	glUseProgram(0);

	glBindFramebuffer(GL_FRAMEBUFFER, tex);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TBO, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawOrder();

	glClear(GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glUseProgram(Window::terrainShader);
	glUniform4f(glGetUniformLocation(Window::terrainShader, "clipPlane"), 0.0f, 0.0f, 0.0f, 0.0f);
	glUseProgram(0);

	glDisable(GL_CLIP_DISTANCE0);

	Window::V = tempW;
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check if escape was pressed
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		// Displacement map toggle
		if (key == GLFW_KEY_F1 && (quad->getIsDmap() == true))
		{
			quad->setIsDmap(false);
		}
		else if (key == GLFW_KEY_F1 && (quad->getIsDmap() == false))
		{
			quad->setIsDmap(true);
		}
		// Wave map toggle
		if (key == GLFW_KEY_F2 && (quad->getIsWave() == true))
		{
			quad->setIsWave(false);
		}
		else if (key == GLFW_KEY_F2 && (quad->getIsWave() == false))
		{
			quad->setIsWave(true);
		}
		// Wireframe toggle
		if (key == GLFW_KEY_F3 && isBox == true)
		{
			isBox = false;
		}
		else if (key == GLFW_KEY_F3 && isBox == false)
		{
			isBox = true;
		}
		// terrain draw toggle
		if (key == GLFW_KEY_F4 && isTerrain == true)
		{
			isTerrain = false;
		}
		else if (key == GLFW_KEY_F4 && isTerrain == false)
		{
			isTerrain = true;
		}
		// ship draw toggle
		if (key == GLFW_KEY_F5 && isShip == true)
		{
			isShip = false;
		}
		else if (key == GLFW_KEY_F5 && isShip == false)
		{
			isShip = true;
		}
		// terrain reset draw toggle
		if (key == GLFW_KEY_F6)
		{
			terrain->reset();
		}
		if (key == GLFW_KEY_P) {
			terrain->changeTerrain();
			box->setHeightMap(terrain->getHeightMap(), terrain->SIZEMAP);
		}

		if (key == GLFW_KEY_LEFT) {
			ship->rotate(1);
			box->changeWireframe();
		}
		if (key == GLFW_KEY_RIGHT) {
			ship->rotate(-1);
			box->changeWireframe();
		}
	}
	if (key == GLFW_KEY_UP) {
		ship->translate(key);
		box->changeWireframe();
	}
	if (key == GLFW_KEY_DOWN) {
		ship->translate(key);
		box->changeWireframe();
	}
	if (key == GLFW_KEY_Z && mods == 0)
	{
		cam_pos = glm::normalize(cam_pos) * inc + cam_pos;
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
	if (key == GLFW_KEY_Z && mods == 1)
	{
		cam_pos = glm::normalize(cam_pos) * -inc + cam_pos;
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

glm::vec3 Window::trackBallMapping(float xpos, float ypos) {
	glm::vec3 v;

	float d;

	v.x = (2.0*xpos - width) / width;
	v.y = (height - 2.0*ypos) / height;
	v.z = 0.0;
	d = glm::length(v);
	d = (d<1.0) ? d : 1.0;
	v.z = sqrtf(1.001 - d * d);
	v = glm::normalize(v); // Still need to normalize, since we only capped d, not v.
	return v;

}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		if (mods == GLFW_MOD_SHIFT) {
			movement = 2;
			lastpt = trackBallMapping(xpos, ypos);
		}
		else {
			movement = 1;
			lastpt = trackBallMapping(xpos, ypos);
		}
	}

	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		movement = 0;
	}
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	float pixel_diff;
	float rot_ang, zoom_factor;

	switch (movement)
	{
	case 1:
	{
		glm::vec3 currpt;
		glm::vec3 dir;

		const float m_ROTSCALE = 90.0;
		currpt = trackBallMapping(xpos, ypos);

		dir = currpt - lastpt;

		float velocity = glm::length(dir); ///use a length fcn
		if (velocity > .000001) {
			glm::vec3 rotAx;

			rotAx = glm::cross(lastpt, currpt);
			rot_ang = glm::radians(velocity*m_ROTSCALE);

			glm::vec4 move = glm::rotate(glm::mat4(1.0f), -rot_ang, rotAx) * glm::vec4(cam_pos, 1.0f);
			cam_pos = glm::vec3(move.x, move.y, move.z);
			//cam_look_at = glm::vec3(cam_pos.x, cam_pos.y, cam_pos.z - 20.0f);

			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
			lastpt = currpt;
		}
		break;
	}

	case 2:
	{

	}

	default:
	{
		break;
	}

	}
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	cam_pos += glm::normalize(cam_pos)*(float)(yoffset) * 2.0f;
	V = glm::lookAt(cam_pos, cam_look_at, cam_up);
}
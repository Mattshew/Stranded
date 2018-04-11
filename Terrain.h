//
//  Terrain.h
//  HW4
//
//  Created by Peter  Phan on 3/15/18.
//  Copyright © 2018 Peter  Phan. All rights reserved.
//

#ifndef Terrain_h
#define Terrain_h

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
#include <math.h>

#define GLFW_INCLUDE_GLEXT
#define GLFW_INCLUDE_GLCOREARB

// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS


#endif

class Terrain
{
private:   
    std::vector<float> height;
    std::vector<unsigned int> indicesV,indicesN;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    glm::mat4 toWorld;
    float seed;
	float originalSeed;
 
public:
    Terrain();
    ~Terrain();
    
    static GLint terrainShader;
    float maxY;
    float minY;
    GLuint VBO, VAO, EBO;
    GLuint uProjection, uView, uModel;
    GLuint time;
    GLuint normalbuffer;
    GLuint uCamPos;
    GLuint uMinY;
    GLuint uMaxY;
    unsigned int textureID;
    
    unsigned int cubemapTexture;
    glm::vec3 camPos;   
    
    float SIZEMAP;
    float randH;
    float randH2;

	void reset();
    void createPoints();
    void createHeights();
    std::vector<float> getHeightMap();
    void createNormals();
    void updatePoints();
    void draw();
    void updateCamPos(glm::vec3 cam_pos);
    void changeTerrain();    
};

#include <stdio.h>

#endif /* Terrain_hpp */
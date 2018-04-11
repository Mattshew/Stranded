//
//  BoundingBox.hpp
//  HW4
//
//  Created by Peter  Phan on 3/19/18.
//  Copyright © 2018 Peter  Phan. All rights reserved.
//

#ifndef BoundingBox_h
#define BoundingBox_h

#include <stdio.h>
#include "OBJObject.h"

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


#endif /* BoundingBox_hpp */

class BoundingBox{
private:

    
public:
    BoundingBox(OBJObject * o);
    ~BoundingBox();
    
    float origMaxX,origMaxY,origMaxZ,origMinX,origMinY,origMinZ;
    float maxX,maxY,maxZ,minX,minY,minZ;
    bool collide;
    OBJObject * o;
    std::vector<float> v;
    std::vector<glm::vec3> wireframe;
    glm::vec3 colorCollision;
    
    GLuint VBO, VAO, EBO;
    GLuint uProjection, uModelview;
    GLuint normalbuffer;
    GLuint uCollide;
    
    float sizemap;
    void createWireFrame(float maxX, float maxY, float maxZ,float minX, float minY, float minZ);
    void setOBJ(OBJObject * o);
    void setHeightMap(std::vector<float> v, float sizemap);
    
    void changeWireframe();
    void isCollision();
    
    void draw();
    void update();
};

#endif

//
//  BoundingBox.cpp
//  HW4
//
//  Created by Peter  Phan on 3/19/18.
//  Copyright © 2018 Peter  Phan. All rights reserved.
//

#include "BoundingBox.h"
#include "Window.h"

GLint wireframeShader;
#define VERTEX_SHADER_PATH "C:/Users/Matthew/Documents/_UCSD/CSE 167/CSE167Final/wireframeshader.vert"
#define FRAGMENT_SHADER_PATH "C:/Users/Matthew/Documents/_UCSD/CSE 167/CSE167Final/wireframeshader.frag"


BoundingBox::BoundingBox(OBJObject * o){
    collide = false;
    wireframeShader = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
    
    colorCollision = glm::vec3(0.0f,0.0f,1.0f);
    sizemap = 0.0f;
    
    this->o = o;
    origMaxX = o->maxX;
    origMaxY = o->maxY;
    origMaxZ = o->maxZ;
    origMinX = o->minX;
    origMinY = o->minY;
    origMinZ = o->minZ;
    
    maxX = o->maxX;
    maxY = o->maxY;
    maxZ = o->maxZ;
    minX = o->minX;
    minY = o->minY;
    minZ = o->minZ;
    
    createWireFrame(maxX, maxY, maxZ, minX, minY, minZ);
     
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, wireframe.size() * sizeof(glm::vec3), &wireframe[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); 

    // Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind the VAO now so we don't accidentally tamper with it.
    glBindVertexArray(0);
}

BoundingBox::~BoundingBox(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(wireframeShader);
}

void BoundingBox::createWireFrame(float maxX, float maxY, float maxZ, float minX, float minY, float minZ) {
    wireframe.clear();

    wireframe.push_back(glm::vec3(minX,maxY,minZ));
    wireframe.push_back(glm::vec3(minX,minY,minZ));
    wireframe.push_back(glm::vec3(minX,minY,maxZ));

    wireframe.push_back(glm::vec3(minX,maxY,maxZ));
    wireframe.push_back(glm::vec3(maxX,maxY,maxZ));
    wireframe.push_back(glm::vec3(maxX,minY,maxZ));

    wireframe.push_back(glm::vec3(maxX,minY,minZ));
    wireframe.push_back(glm::vec3(maxX,maxY,minZ));
    wireframe.push_back(glm::vec3(minX,maxY,minZ));

    wireframe.push_back(glm::vec3(minX,maxY,maxZ));
    wireframe.push_back(glm::vec3(maxX,maxY,maxZ));
    wireframe.push_back(glm::vec3(maxX,maxY,minZ));

    wireframe.push_back(glm::vec3(maxX,minY,minZ));

    wireframe.push_back(glm::vec3(minX,minY,minZ));
    wireframe.push_back(glm::vec3(minX,minY,maxZ));
    wireframe.push_back(glm::vec3(maxX,minY,maxZ));

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, wireframe.size() * sizeof(glm::vec3), &wireframe[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    
    // Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind the VAO now so we don't accidentally tamper with it.
    glBindVertexArray(0);    
}


void BoundingBox::setOBJ(OBJObject * o){
    this->o = o;
    maxX = o->maxX;
    maxY = o->maxY;
    maxZ = o->maxZ;
    minX = o->minX;
    minY = o->minY;
    minZ = o->minZ;
    
    createWireFrame(maxX, maxY, maxZ, minX, minY, minZ);   
}
void BoundingBox::setHeightMap(std::vector<float> v, float sizemap){
    this->v = v;
    this->sizemap = sizemap;    
}

//use in Window when you change object
void BoundingBox::changeWireframe(){
    glm::mat4 M = o->getToWorld();
    glm::vec4 pt1 = M * glm::vec4(o->maxX,o->maxY,o->maxZ, 1.0f);
    glm::vec4 pt2 = M * glm::vec4(o->maxX,o->maxY,o->minZ, 1.0f);
    glm::vec4 pt3 = M * glm::vec4(o->maxX,o->minY,o->maxZ, 1.0f);
    glm::vec4 pt4 = M * glm::vec4(o->maxX,o->minY,o->minZ, 1.0f);
    glm::vec4 pt5 = M * glm::vec4(o->minX,o->maxY,o->maxZ, 1.0f);
    glm::vec4 pt6 = M * glm::vec4(o->minX,o->maxY,o->minZ, 1.0f);
    glm::vec4 pt7 = M * glm::vec4(o->minX,o->minY,o->maxZ, 1.0f);
    glm::vec4 pt8 = M * glm::vec4(o->minX,o->minY,o->minZ, 1.0f);
    
    maxX = std::max({pt1.x, pt2.x, pt3.x, pt4.x, pt5.x, pt6.x, pt7.x, pt8.x});
    maxY = std::max({pt1.y, pt2.y, pt3.y, pt4.y, pt5.y, pt6.y, pt7.y, pt8.y});
    maxZ = std::max({pt1.z, pt2.z, pt3.z, pt4.z, pt5.z, pt6.z, pt7.z, pt8.z});
    
    minX = std::min({pt1.x, pt2.x, pt3.x, pt4.x, pt5.x, pt6.x, pt7.x, pt8.x});
    minY = std::min({pt1.y, pt2.y, pt3.y, pt4.y, pt5.y, pt6.y, pt7.y, pt8.y});
    minZ = std::min({pt1.z, pt2.z, pt3.z, pt4.z, pt5.z, pt6.z, pt7.z, pt8.z});
    
    createWireFrame(maxX, maxY, maxZ, minX, minY, minZ);
    isCollision();
}

//use in idle_callback in Window
void BoundingBox::isCollision(){
    float correctmaxX = maxX+sizemap/2.0f;
    float correctminX = minX+sizemap/2.0f;

    float correctmaxZ = maxZ+sizemap/2.0f;
    float correctminZ = minZ+sizemap/2.0f;
    
    //only check if inside the terrain
    if(correctmaxX < sizemap && correctminX > 0 && correctmaxZ < sizemap && correctminZ > 0){
        //get heights at each 4 points       
        //only at the height
        float terrainMax = -std::numeric_limits<float>::max();
        float terrainMin = std::numeric_limits<float>::max();

        for(float i = correctminX ; i < correctmaxX; i++ ){
            for(float j= correctminZ ; j < correctmaxZ; j++ ){
                float curr = v[(int)i*sizemap+(int)j];

                if(curr>terrainMax){
                    terrainMax = curr;          
                }
                if(curr<terrainMin){
                    terrainMin = curr;
                }               
            }
        }        
        
        if(maxY >= terrainMin && minY <= terrainMax){
            collide = true;
                colorCollision = glm::vec3(1.0f,0.0f,0.0f);
        }
        
        else{
            collide = false;
                colorCollision = glm::vec3(0.0f,0.0f,1.0f);
        }    
    }
}


void BoundingBox::draw(){
    glUseProgram(wireframeShader);
    
    glm::mat4 modelview = Window::V * glm::mat4(1.0f);
    
    GLuint uProjection = glGetUniformLocation(wireframeShader, "projection");
    GLuint uModelview = glGetUniformLocation(wireframeShader, "modelview");
    GLuint uCollide = glGetUniformLocation(wireframeShader, "colorCollide");
    
    
    // Now send these values to the shader program
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
    glUniform3f(uCollide, colorCollision.x,colorCollision.y,colorCollision.z);
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_STRIP, 0, wireframe.size());
    // Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
    glBindVertexArray(0);
}

//also use in idle callback  in Window
void BoundingBox::update(){
    
}

//
//  Terrain.cpp
//  HW4
//
//  Created by Peter  Phan on 3/15/18.
//  Copyright © 2018 Peter  Phan. All rights reserved.
//

#include "Terrain.h"
#include "Window.h"

#include <iostream>
#include <algorithm>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>

const float TOPR = 0.0f;
const float TOPL = 0.0f;
const float BOTR = 0.0f;
const float BOTL= 0.0f;


Terrain::Terrain(){
    toWorld = glm::mat4(1.0f);
    minY = std::numeric_limits<float>::max();
    maxY = -std::numeric_limits<float>::max();
        
    // SIZEMAP must = 2^n+1
    SIZEMAP = 257.0f;
    seed = 135;
	originalSeed = seed;
    srand(seed);
    randH = 300;
    randH2 = 300;
    
    for(float i = 0; i<SIZEMAP ;i++){
        for(float j = 0; j<SIZEMAP; j++){
            height.push_back(0.0f);
        }
    }
    
    createHeights();
    createPoints();
    updatePoints();
    createNormals();
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &normalbuffer);
    
       
    // Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    // 3rd attribute buffer : normals
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);    
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesV.size() * sizeof(unsigned int) , &indicesV[0], GL_STATIC_DRAW);
    
    // Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind the VAO now so we don't accidentally tamper with it.
    glBindVertexArray(0);   
}

void Terrain::reset()
{
	seed = originalSeed;
	changeTerrain();
}

void Terrain::createPoints(){
    for(float i = -SIZEMAP/2; i<SIZEMAP/2; i++){
        for(float j = -SIZEMAP/2; j<SIZEMAP/2; j++){
            vertices.push_back(glm::vec3(i, 0.0f, j));            
        }
    }
    
    for(float i = 0; i<SIZEMAP-1; i++){        
        for(float j = 0; j<SIZEMAP-1; j++){ 
            indicesV.push_back(j+(i+1)*SIZEMAP);
            indicesV.push_back(j+1+i*SIZEMAP);
            indicesV.push_back(j+1+(i+1)*SIZEMAP);
            
            indicesV.push_back(j+i*SIZEMAP);
            indicesV.push_back(j+1+i*SIZEMAP);
            indicesV.push_back(j+(i+1)*SIZEMAP);
        }        
    }    
}

void Terrain::createHeights(){
    float n = SIZEMAP-1.0f;

    n/=2.0f;

    while(n >= 1.0f){


        for(float i=n; i<SIZEMAP; i+=(n*2)){
            for(float j=n; j<SIZEMAP; j+=(n*2)){
				float curr = (height[j - n + (i - n)*SIZEMAP]
					+ height[j + n + (i - n)*SIZEMAP]
					+ height[j + n + (i + n)*SIZEMAP]
					+ height[j - n + (i + n)*SIZEMAP]) / 4.0f
					+ ((float)rand() / (double)RAND_MAX)*(float)randH - randH / 3.0f;

                height[j+i*SIZEMAP] = curr; 
            }
        }

        for (float i = 0.0f; i < SIZEMAP; i += (n * 2)) {
            for (float j = n; j < SIZEMAP; j += (n * 2)) {
                float curr = height[j+i*SIZEMAP];

                if (i-n < 0.0f){
					curr = (height[j + n + (i)*SIZEMAP]
						+ height[j - n + (i)*SIZEMAP]
						+ height[j + (i + n)*SIZEMAP]) / 3.0f
						+ ((float)rand() / (double)RAND_MAX)*randH - randH / 3.0f;
                }

                else if (i+n >= SIZEMAP) {
					curr = (height[j + n + (i)*SIZEMAP]
						+ height[j - n + (i)*SIZEMAP]
						+ height[j + (i - n)*SIZEMAP]) / 3.0f
						+ ((float)rand() / (double)RAND_MAX)*(float)randH - randH / 3.0f;
                }

                else {
					curr = (height[j + n + (i)*SIZEMAP]
						+ height[j - n + (i)*SIZEMAP]
						+ height[j + (i - n)*SIZEMAP]
						+ height[j + (i + n)*SIZEMAP]) / 4.0f
						+ ((float)rand() / (double)RAND_MAX)*(float)randH - randH / 3.0f;
                }
                height[j+i*SIZEMAP] = curr;

            }
        }
        for (float i = n; i < SIZEMAP; i += (n * 2)) {
            for (float j = 0.0f; j<SIZEMAP; j+=(n * 2)) {
                float curr = height[j+i*SIZEMAP];
                if (j - n < 0.0f) {
					curr = (height[j + n + (i)*SIZEMAP]
						+ height[j + (i - n)*SIZEMAP]
						+ height[j + (i + n)*SIZEMAP]) / 3.0f
						+ ((float)rand() / (double)RAND_MAX)*(float)randH - randH / 3.0f;
                }
                else if(j+n >= SIZEMAP){
					curr = (height[j - n + (i)*SIZEMAP]
						+ height[j + (i - n)*SIZEMAP]
						+ height[j + (i + n)*SIZEMAP]) / 3.0f
						+ ((float)rand() / (double)RAND_MAX)*(float)randH - randH / 3.0f;
                }
                else{
					curr = (height[j + n + (i)*SIZEMAP]
						+ height[j - n + (i)*SIZEMAP]
						+ height[j + (i - n)*SIZEMAP]
						+ height[j + (i + n)*SIZEMAP]) / 4.0f
						+ ((float)rand() / (double)RAND_MAX)*(float)randH - randH / 3.0f;
                }               
                height[j+i*SIZEMAP] = curr;

            }
        }       
        n/=2.0f;
        randH/=2.0f;    
    } 
}



std::vector<float> Terrain::getHeightMap(){
    return height;
}


void Terrain::createNormals(){
    
    //calculate triangles normals with an outer box
    std::vector<glm::vec3> triangles;
    float index = 0;
    //creates border triangles
    for(int i = 0; i<(SIZEMAP+1)*(SIZEMAP+1)*2; i++){
        triangles.push_back(glm::vec3(0.0f));        
    }

    
    for(int i = -1; i<SIZEMAP-1; i++){
        for(int j = -1; j<SIZEMAP-1; j++){
            if(i<0 || j<0 || i+1 >=SIZEMAP || j+1 >= SIZEMAP){
                index++;
                continue;
            }
            
            float p1 = height[i*SIZEMAP+j];
            float p2 = height[i*SIZEMAP+j+1];
            float p4 = height[(i+1)*SIZEMAP+j];
            float p3 = height[(i+1)*SIZEMAP+j+1];

            //first and second triangle normal
            glm::vec3 norm1 = glm::vec3(p1-p3, 0.0f,p1-p2);
            glm::vec3 norm2 = glm::vec3(p2-p4, 0.0f,p3-p4);
            
            triangles[index] = norm1;
            index++;
            triangles[index] = norm2;
            index++;

        }
    }
    
    //calculates normals at vertex
    float offset = (SIZEMAP+1)*2-1;
    index = 1;
    for(int i = 0; i<SIZEMAP; i++){
        for(int j = 0; j<SIZEMAP; j++){
            float index2 = index+1;
            float index3 = index+2;
            
            glm::vec3 tri1 = triangles[index];
            glm::vec3 tri2 = triangles[index2];
            glm::vec3 tri3 = triangles[index3];
            glm::vec3 tri4 = triangles[index+offset];
            glm::vec3 tri5 = triangles[index2+offset];
            glm::vec3 tri6 = triangles[index3+offset];
            
            glm::vec3 avgNorm = tri1+tri2+tri3+tri4+tri5+tri6;
            avgNorm /= 6.0f;
            normals.push_back(avgNorm);
            
            index += 2.0f;
        }
        index += 2.0f;
    }
}


void Terrain::updatePoints(){
    for(float i = 0; i<SIZEMAP; i++){
        for(float j = 0; j<SIZEMAP; j++){
            vertices[i*SIZEMAP+j].y = height[i*SIZEMAP+j];
            
            if(height[i*SIZEMAP+j]<minY){
                minY = height[i*SIZEMAP+j];
            }
            if(height[i*SIZEMAP+j]>maxY){
                maxY = height[i*SIZEMAP+j];
            }
        }
    }
}




Terrain::~Terrain(){
    
}

void Terrain::draw(){
    glUseProgram(Window::terrainShader);
    
  
    // Get the location of the uniform variables 
    uProjection = glGetUniformLocation(Window::terrainShader, "projection");
    uModel = glGetUniformLocation(Window::terrainShader, "model");
    uView = glGetUniformLocation(Window::terrainShader, "view");
    uCamPos = glGetUniformLocation(Window::terrainShader, "camPos");
    uMinY = glGetUniformLocation(Window::terrainShader, "minY");
    uMaxY = glGetUniformLocation(Window::terrainShader, "maxY");
    
    // Now send these values to the shader program
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);
    glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);
    glUniform3fv(uCamPos, 1, &camPos[0]);
    glUniform1f(uMinY, minY);
    glUniform1f(uMaxY, maxY);
    
    glEnable(GL_CLIP_PLANE0);
    // ... set view and projection matrix
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indicesV.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glDisable(GL_CLIP_PLANE0);
    glUseProgram(Window::terrainShader);
}


void Terrain::updateCamPos(glm::vec3 cam_pos){
    camPos = cam_pos;
    
}
void Terrain::changeTerrain(){
    randH=randH2;

    srand(seed);
	seed++;

    createHeights();
    createPoints();
    updatePoints();
    createNormals();    
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &normalbuffer);

    // Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    // 3rd attribute buffer : normals
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
    // the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    // Enable the usage of layout location 0 (check the vertex shader to see what this is)
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    // We've sent the vertex data over to OpenGL, but there's still something missing.
    // In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesV.size() * sizeof(unsigned int) , &indicesV[0], GL_STATIC_DRAW);

    // Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind the VAO now so we don't accidentally tamper with it.
    // NOTE: You must NEVER unbind the element array buffer associated with a VAO!
    glBindVertexArray(0);
}
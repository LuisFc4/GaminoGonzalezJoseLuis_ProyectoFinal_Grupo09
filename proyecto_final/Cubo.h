#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include "Texture.h"
class Cubo {
private:
    GLuint VAO, VBO, EBO;
    int texture_id;
    GLfloat vertices[192] ={
      // Positions            // Normals              // Texture Coords
        -0.5f, -0.5f, 0.5f,    0.0f, 0.0f,1.0f,     0.5f,0.25f,
        0.5f, -0.5f, 0.5f,     0.0f, 0.0f,1.0f,     0.75f,0.25f,
        0.5f,  0.5f, 0.5f,     0.0f, 0.0f,1.0f,     0.75f,0.5f,
        -0.5f,  0.5f, 0.5f,    0.0f, 0.0f,1.0f,     0.5f,0.5f,

        //Right
        // Positions            // Normals              // Texture Coords
        0.5f, -0.5f, 0.5f,     1.0f, 0.0f,0.0f,     0.75f,0.25f,
        0.5f, -0.5f, -0.5f,    1.0f, 0.0f,0.0f,     1.0f,0.25f,
        0.5f,  0.5f, -0.5f,    1.0f, 0.0f,0.0f,     1.0f,0.5f,
        0.5f,  0.5f, 0.5f,     1.0f, 0.0f,0.0f,     0.75f,0.5f,

        //Back
        // Positions            // Normals              // Texture Coords
         0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f,      0.0f,0.25f,
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f,      0.25f,0.25f,
        -0.5f,  0.5f, -0.5f,    0.0f, 0.0f, -1.0f,      0.25f,0.5f,
         0.5f,  0.5f, -0.5f,    0.0f, 0.0f, -1.0f,      0.0f,0.5f,

         //Left
         // Positions            // Normals              // Texture Coords
         -0.5f, -0.5f, -0.5f,    -1.0f, 0.0f,0.0f,       0.25f,0.25f,
         -0.5f, -0.5f, 0.5f,     -1.0f, 0.0f,0.0f,       0.5f,0.25f,
         -0.5f,  0.5f, 0.5f,     -1.0f, 0.0f,0.0f,       0.5f,0.5f,
         -0.5f,  0.5f, -0.5f,    -1.0f, 0.0f,0.0f,       0.25f,0.5f,

         //top
         // Positions            // Normals              // Texture Coords
         -0.5f, 0.5f, 0.5f,      0.0f, 1.0f,0.0f,        0.25f,0.5f,
          0.5f, 0.5f, 0.5f,      0.0f, 1.0f,0.0f,        0.5f,0.5f,
          0.5f,  0.5f,-0.5f,     0.0f, 1.0f,0.0f,        0.5f,0.75f,
         -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,0.0f,        0.25f,0.75f,

         //bottom
         // Positions            // Normals              // Texture Coords
         -0.5f, -0.5f, 0.5f,     0.0f, -1.0f, 0.0f,      0.25f,0.0f,
          0.5f, -0.5f, 0.5f,     0.0f, -1.0f, 0.0f,      0.5f,0.0f,
          0.5f,  -0.5f,-0.5f,    0.0f, -1.0f, 0.0f,      0.5f,0.25f,
         -0.5f,  -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,      0.25f,0.25f,

        /*
        // vertices                             //normales              //coordenadas de textura
        -1.0f,-1.0f,-1.0f,                      -1.0f,0.0f,0.0f,        0.0f,0.0f,
        -1.0f,-1.0f,1.0f,                       -1.0f,0.0f,0.0f,        1.0f,0.0f,
         1.0f,-1.0f,1.0f,                       -1.0f,0.0f,0.0f,        1.0f,1.0f,
        -1.0f,-1.0f,-1.0f,                      -1.0f,0.0f,0.0f,        0.0f,1.0f,
        */
    };
    GLint indices[36] = {
        0,1,3,
        1,2,3,
        // Iferior derecha
        4,5,7,
        5,6,7,
        //Superior derecha
        8,9,11,
        9,10,11,
        // Superior izquierda
        12,13,15,
        13,14,15,

        16,17,19,
        17,18,19,

        20,21,23,
        21,22,23
    };

public:
    Cubo(GLchar* texture_path) {
        this->texture_id = TextureLoading::LoadTexture(texture_path);
        //Creando buffers
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glGenBuffers(1, &this->EBO);

        glBindVertexArray(this->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), &this->vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indices), this->indices, GL_STATIC_DRAW);


        // Set the vertex attribute pointers
        // Vertex Positions
        // Position attribute
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        // Vertex Normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        // Vertex Texture Coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));

        glBindVertexArray(0);
    }
    void draw() {
        // Draw mesh
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->texture_id);

        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES, int(sizeof(this->indices)/sizeof(GLfloat)), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};
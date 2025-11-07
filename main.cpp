#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

// for ease
#include "gl_framework.hpp"

// for hierarchical model
#include "hierarchy_node.hpp"

// for texture
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;
using namespace csX75;

// Window dimensions
const int WIDTH = 1280;
const int HEIGHT = 720;

// Camera settings
glm::vec3 cameraPos1 = glm::vec3(0.0f, 4.5f, 12.0f);
glm::vec3 cameraTarget1 = glm::vec3(0.0f, 2.0f, 0.0f);
int currentCamera = 1; // 1 for static, 2 for following

// excavator parts
HNode * base, * cabin, * boom, * stick, * bucket; 
GLuint modelLoc;

// Excavator parameters
float excavatorX = 0.0f;
float excavatorZ = 0.0f;
float cabinRotation = 0.0f;
float boomAngle = -30.0f;
float stickAngle = 45.0f;
float bucketAngle = 30.0f;
float boomRoll = 0.0f;
float stickTwist = 0.0f;

// Lighting toggles
bool light1On = true;  // Ceiling lantern
bool light2On = true;  // Window moonlight
bool excavatorLightsOn = true;

// textures
GLuint walltexture;
GLuint floortexture;

// spongebob textures
GLuint sponge_front, sponge_left, sponge_right, sponge_back, sponge_top;

float limb_length = 0.3f;

// Generate cube vertices
void generateCube(vector<float>& vertices) {
    float cubeData[] = {
        // Positions          // Normals           // Tex Coords
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
        // Left face
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        // Right face
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        // Top face
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    
    vertices.assign(cubeData, cubeData + sizeof(cubeData) / sizeof(float));
}

// Generate cylinder vertices
void generateCylinder(vector<float>& vertices, int segments = 32) {
    vertices.clear();
    
    // Generate side faces
    for (int i = 0; i <= segments; i++) {
        float theta = (float)i / segments * 2.0f * M_PI;
        float x = cos(theta) * 0.5f;
        float z = sin(theta) * 0.5f;
        float nx = cos(theta);
        float nz = sin(theta);
        
        // Bottom vertex
        vertices.insert(vertices.end(), {x, -0.5f, z, nx, 0.0f, nz, (float)i/segments, 0.0f});
        // Top vertex
        vertices.insert(vertices.end(), {x, 0.5f, z, nx, 0.0f, nz, (float)i/segments, 1.0f});
    }
}

// Create VAO for a mesh
GLuint createVAO(const vector<float>& vertices) {
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);
    return VAO;
}

// converts to glm::vec4
void createCubeData(std::vector<glm::vec4>& out_vertices, std::vector<glm::vec4>& out_colors) {
    std::vector<float> rawVerts;
    generateCube(rawVerts);

    // Each vertex has 8 floats: position (3), normal (3), texcoord (2)
    const int stride = 8;
    int numVerts = rawVerts.size() / stride;

    out_vertices.resize(numVerts);
    out_colors.resize(numVerts);

    for (int i = 0; i < numVerts; i++) {
        float x = rawVerts[i * stride + 0];
        float y = rawVerts[i * stride + 1];
        float z = rawVerts[i * stride + 2];
        out_vertices[i] = glm::vec4(x, y, z, 1.0f);
        out_colors[i] = glm::vec4(1.0f, 0.85f, 0.2f, 1.0f);
    }
}

void createExcavatorModel() {
    vector<float> cubeVerts, cubeColors;
    generateCube(cubeVerts);
    cubeColors.resize(cubeVerts.size(), 1.0f); // white for now

    // Define scale factors for each part
    glm::vec3 baseScale = glm::vec3(1.2f, 0.3f, 0.8f);
    glm::vec3 cabinScale = glm::vec3(0.8f, 0.6f, 0.7f);
    glm::vec3 boomScale = glm::vec3(0.15f, 1.5f, 0.15f);
    glm::vec3 stickScale = glm::vec3(0.12f, 1.2f, 0.12f);
    glm::vec3 bucketScale = glm::vec3(0.3f, 0.3f, 0.25f);

    // Calculate actual heights (since cube is from -0.5 to 0.5, height = scale.y)
    float baseHeight = baseScale.y;
    float cabinHeight = cabinScale.y;
    float boomHeight = boomScale.y;
    float stickHeight = stickScale.y;
    float bucketHeight = bucketScale.y;

    // Base / Tracks
    base = new HNode(
        nullptr,
        36,
        reinterpret_cast<glm::vec4*>(cubeVerts.data()),
        reinterpret_cast<glm::vec4*>(cubeColors.data()),
        cubeVerts.size() * sizeof(float),
        0,
        baseScale,
        glm::vec3(0.0f, -0.5f * baseScale.y, 0.0f)  // pivot at bottom of base
    );

    // Cabin (rotates)
    cabin = new HNode(
        base,
        36,
        reinterpret_cast<glm::vec4*>(cubeVerts.data()),
        reinterpret_cast<glm::vec4*>(cubeColors.data()),
        cubeVerts.size() * sizeof(float),
        0,
        cabinScale,
        glm::vec3(0.0f, -0.5f * cabinScale.y, 0.0f)    // pivot at bottom of cabin
    );

    // Boom (rotates around joint)
    boom = new HNode(
        cabin,
        36,
        reinterpret_cast<glm::vec4*>(cubeVerts.data()),
        reinterpret_cast<glm::vec4*>(cubeColors.data()),
        cubeVerts.size() * sizeof(float),
        0,
        boomScale,
        glm::vec3(0.0f, -0.5f * boomScale.y, 0.0f)    // pivot at bottom of boom
    );

    // Stick (rotates around joint)
    stick = new HNode(
        boom,
        36,
        reinterpret_cast<glm::vec4*>(cubeVerts.data()),
        reinterpret_cast<glm::vec4*>(cubeColors.data()),
        cubeVerts.size() * sizeof(float),
        0,
        stickScale,
        glm::vec3(0.0f, -0.5f * stickScale.y, 0.0f)    // pivot at bottom of stick
    );

    // Bucket (rotates around joint at its bottom edge, where it connects to stick)
    bucket = new HNode(
        stick,
        36,
        reinterpret_cast<glm::vec4*>(cubeVerts.data()),
        reinterpret_cast<glm::vec4*>(cubeColors.data()),
        cubeVerts.size() * sizeof(float),
        0,
        bucketScale,
        glm::vec3(0.0f, -0.5f * bucketScale.y, 0.0f)   // pivot at bottom of bucket
    );

    // Initialize hierarchical positions (stacked vertically with no gaps)
    // Base sits on the ground at y = baseHeight/2
    base->change_parameters(excavatorX, baseHeight * 0.5f, excavatorZ, 0.0f, 0.0f, 0.0f);
    
    // Cabin sits on top of base, so offset by baseHeight/2 + cabinHeight/2
    cabin->change_parameters(0.0f, baseHeight * 0.5f + cabinHeight * 0.5f, 0.0f, 0.0f, 0.0f, 0.0f);
    
    // Boom attaches to top of cabin, offset by cabinHeight/2 + boomHeight/2
    boom->change_parameters(0.0f, cabinHeight * 0.5f + boomHeight * 0.5f, 0.0f, 0.0f, 0.0f, 0.0f);
    
    // Stick attaches to top of boom
    stick->change_parameters(0.0f, boomHeight * 0.5f + stickHeight * 0.5f, 0.0f, 0.0f, 0.0f, 0.0f);
    
    // Bucket attaches to top of stick
    bucket->change_parameters(0.0f, stickHeight * 0.5f + bucketHeight * 0.5f, 0.0f, 0.0f, 0.0f, 0.0f);
}

void drawSpongebobBody(GLuint shaderProgram) {
    // Spongebob body dimensions
    float hw = 0.5f;  // half width
    float hh = 0.5f;  // half height
    float hd = 0.2f;  // half depth

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.8f, 2.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    GLuint idx[] = {0, 1, 2, 2, 3, 0};
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);

    auto drawFace = [&](float vertices[], GLuint texture, bool useTex, glm::vec3 color = glm::vec3(1.0f)) {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 32, vertices, GL_STATIC_DRAW); // 8 floats per vertex, 4 vertices

        // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Texture coords
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), useTex ? 1 : 0);
        if (useTex) {
            glBindTexture(GL_TEXTURE_2D, texture);
        } else {
            glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(color));
        }

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    };

    // --- Front ---
    float front[] = {
        -hw, -hh,  hd, 0, 0, 1, 0.0f, 0.0f,  // bottom-left
         hw, -hh,  hd, 0, 0, 1, 1.0f, 0.0f,  // bottom-right
         hw,  hh,  hd, 0, 0, 1, 1.0f, 1.0f,  // top-right
        -hw,  hh,  hd, 0, 0, 1, 0.0f, 1.0f   // top-left
    };
    drawFace(front, sponge_front, true);

    // --- Back ---
    float back[] = {
         hw, -hh, -hd, 0, 0, -1, 0.0f, 0.0f,
        -hw, -hh, -hd, 0, 0, -1, 1.0f, 0.0f,
        -hw,  hh, -hd, 0, 0, -1, 1.0f, 1.0f,
         hw,  hh, -hd, 0, 0, -1, 0.0f, 1.0f
    };
    drawFace(back, sponge_back, true);

    // --- Left ---
    float left[] = {
        -hw, -hh, -hd, -1, 0, 0, 0.0f, 0.0f,
        -hw, -hh,  hd, -1, 0, 0, 1.0f, 0.0f,
        -hw,  hh,  hd, -1, 0, 0, 1.0f, 1.0f,
        -hw,  hh, -hd, -1, 0, 0, 0.0f, 1.0f
    };
    drawFace(left, sponge_left, true);

    // --- Right ---
    float right[] = {
         hw, -hh,  hd, 1, 0, 0, 0.0f, 0.0f,
         hw, -hh, -hd, 1, 0, 0, 1.0f, 0.0f,
         hw,  hh, -hd, 1, 0, 0, 1.0f, 1.0f,
         hw,  hh,  hd, 1, 0, 0, 0.0f, 1.0f
    };
    drawFace(right, sponge_right, true);

    // --- Top ---
    float top[] = {
        -hw,  hh,  hd, 0, 1, 0, 0.0f, 0.0f,
         hw,  hh,  hd, 0, 1, 0, 1.0f, 0.0f,
         hw,  hh, -hd, 0, 1, 0, 1.0f, 1.0f,
        -hw,  hh, -hd, 0, 1, 0, 0.0f, 1.0f
    };
    drawFace(top, sponge_top, true);

    // --- Bottom (colored) ---
    float bottom[] = {
        -hw, -hh, -hd, 0, -1, 0, 0.0f, 0.0f,
         hw, -hh, -hd, 0, -1, 0, 1.0f, 0.0f,
         hw, -hh,  hd, 0, -1, 0, 1.0f, 1.0f,
        -hw, -hh,  hd, 0, -1, 0, 0.0f, 1.0f
    };
    drawFace(bottom, 0, false, glm::vec3(195.0f/255.0f, 107.0f/255.0f, 15.0f/255.0f));

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void drawSpongeBobLimbs(GLuint shaderProgram, GLuint cylVAO, int segments = 32) {
    glBindVertexArray(cylVAO);
    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
    glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 1.0f, 0.9f, 0.1f);

    float bodyY = limb_length + 0.5f; // bottom of body
    // float bodyHH = 0.5f;
    float bodyHW = 0.5f;

    glm::vec3 legSize(0.08f, limb_length, 0.08f); 
    float legYStart = 0.0f; // legs start at floor

    glm::vec3 leftLegPos(2.0f + -bodyHW + 0.1f, legYStart + limb_length / 2.0f, 2.0f + 0.1f);
    glm::vec3 rightLegPos(2.0f + bodyHW - 0.1f, legYStart + limb_length / 2.0f, 2.0f + 0.1f);

    // Draw legs
    for (glm::vec3 pos : {leftLegPos, rightLegPos}) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pos);
        model = glm::scale(model, legSize);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, (segments + 1) * 2);
    }

    // Arms
    glm::vec3 armSize(0.08f, limb_length, 0.08f);
    float armY = bodyY; 
    float armXOffset = bodyHW + 0.05f;

    glm::vec3 leftArmPos(2.0f - armXOffset, armY, 2.0f);
    glm::vec3 rightArmPos(2.0f + armXOffset, armY, 2.0f);

    // Left arm
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, leftArmPos);
        model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0,0,1));
        model = glm::scale(model, armSize);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"),1,GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, (segments + 1) * 2);
    }

    // Right arm
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, rightArmPos);
        model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0,0,1));
        model = glm::scale(model, armSize);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"),1,GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, (segments + 1) * 2);
    }

    glBindVertexArray(0);
}

// Draw excavator hierarchy using HNode tree
void drawExcavator(GLuint shaderProgram) {
    // no texture for now
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, walltexture);
    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0); // disable texturing
    
    // Start from the root node (base of the excavator)
    if (base) {
        glm::mat4 identity = glm::mat4(1.0f);
        base->render_tree(identity);
    }

    // Set excavator lights positions based on cabin node
    if (cabin) {
        glm::mat4 cabinGlobal = base->modelMatrix * cabin->modelMatrix; 
        glm::vec3 leftLightPos  = glm::vec3(cabinGlobal * glm::vec4(-0.3f, 0.3f, 0.4f, 1.0f));
        glm::vec3 rightLightPos = glm::vec3(cabinGlobal * glm::vec4(0.3f, 0.3f, 0.4f, 1.0f));
        glUniform3fv(glGetUniformLocation(shaderProgram, "excavatorLight1Pos"), 1, glm::value_ptr(leftLightPos));
        glUniform3fv(glGetUniformLocation(shaderProgram, "excavatorLight2Pos"), 1, glm::value_ptr(rightLightPos));
    }
}

// Draw scene
void drawScene(GLuint shaderProgram, GLuint cubeVAO, GLuint cylinderVAO) {
    glm::mat4 model;
    
    // Floor
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(10.0f, 0.1f, 10.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 0.4f, 0.3f, 0.2f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, floortexture);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 1);
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36); 

    // Back wall
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 3.0f, -5.0f));
    model = glm::scale(model, glm::vec3(10.0f, 6.0f, 0.2f));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 0.5f, 0.35f, 0.2f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, walltexture);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 1); // Enable texturing
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // Left wall
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-5.0f, 3.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f, 6.0f, 10.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 0.5f, 0.35f, 0.2f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, walltexture);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 1); // Enable texturing
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // Right wall
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(5.0f, 3.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f, 6.0f, 10.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 0.5f, 0.35f, 0.2f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, walltexture);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 1); // Enable texturing
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // Ceiling
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 6.0f, 0.0f));
    model = glm::scale(model, glm::vec3(10.0f, 0.1f, 10.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 0.5f, 0.35f, 0.2f);
    glBindTexture(GL_TEXTURE_2D, floortexture);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 1);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // Workbench
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-3.5f, 1.0f, -4.0f));
    model = glm::scale(model, glm::vec3(2.0f, 2.0f, 1.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 0.6f, 0.4f, 0.2f);
    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0); // Disable texturing
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // Watering can body
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(3.0f, 0.5f, 2.0f));
    model = glm::scale(model, glm::vec3(0.4f, 0.6f, 0.4f));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 0.2f, 0.6f, 0.3f);
    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0); // Disable texturing
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // Watering can spout
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(3.0f, 0.6f, 2.4f));
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.1f, 0.4f, 0.1f));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 0.2f, 0.6f, 0.3f);
    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0); // Disable texturing
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // spongebob
    drawSpongebobBody(shaderProgram);
    drawSpongeBobLimbs(shaderProgram, cylinderVAO);

    // Window (gives moonlight)
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(4.5f, 3.0f, -4.5f));
    model = glm::scale(model, glm::vec3(1.5f, 2.0f, 0.1f));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 0.2f, 0.3f, 0.6f);
    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0); // Disable texturing
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // Ceiling lantern (a small cube hanging)
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 5.6f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 1.0f, 0.9f, 0.6f);
    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0); // Disable texturing
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

// Input handling, add keys for stick twist and boom
void processInput(GLFWwindow* window, float deltaTime) {    
    // movement for excavator
    float moveSpeed = 2.0f * deltaTime;
    float rotSpeed = 20.0f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) excavatorZ -= moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) excavatorZ += moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) excavatorX -= moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) excavatorX += moveSpeed;

    base->change_parameters(excavatorX, 0.3f, excavatorZ, base->rx, base->ry, base->rz);

    // cabin rotate
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) cabin->inc_ry();
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) cabin->dec_ry();
    
    // arm controls
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) boomAngle += rotSpeed;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) boomAngle -= rotSpeed;
    boomAngle = glm::clamp(boomAngle, -10.0f, 50.0f);
    boom->rx = boomAngle;
    boom->update_matrices();

    // stick angle
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) stickAngle += rotSpeed;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) stickAngle -= rotSpeed;
    stickAngle = glm::clamp(stickAngle, -10.0f, 120.0f);
    stick->rx = stickAngle;
    stick->update_matrices();

    // bucket angle
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) bucketAngle += rotSpeed;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) bucketAngle -= rotSpeed;
    bucketAngle = glm::clamp(bucketAngle, -20.0f, 90.0f);
    bucket->rx = bucketAngle;
    bucket->update_matrices();

    // stick twist
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) stickTwist += rotSpeed;
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) stickTwist -= rotSpeed;
    stickTwist = glm::clamp(stickTwist, -45.0f, 45.0f);
    stick->rz = stickTwist;
    stick->update_matrices();

    // boom roll
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) boomRoll += rotSpeed;
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) boomRoll -= rotSpeed;
    boomRoll = glm::clamp(boomRoll, -30.0f, 30.0f);
    boom->rz = boomRoll;
    boom->update_matrices();

    // camera 1 controls with arrow keys
    float cameraSpeed = 3.0f * deltaTime;
    if (currentCamera == 1) {
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) cameraPos1.x -= cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) cameraPos1.x += cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) cameraPos1.y += cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) cameraPos1.y -= cameraSpeed;
    }
}

// to load textures
GLuint loadTexture(const char* path) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        stbi_image_free(data);
    } else {
        std::cout << "Failed to load texture: " << path << " (" << stbi_failure_reason() << ")" << std::endl;
    }
    return textureID;
}

int main() {
    // initialize glfw
    csX75::initGL(WIDTH, HEIGHT, "Excavator");

    // build shader
    GLuint shaderProgram = csX75::LoadShaders("vertex_shader.glsl", "fragment_shader.glsl");
    glUseProgram(shaderProgram);

    // set value of modelLoc
    modelLoc = glGetUniformLocation(shaderProgram, "model");

    // create the excavator model
    createExcavatorModel();

    // Generate meshes
    vector<float> cubeVerts;
    generateCube(cubeVerts);
    GLuint cubeVAO = createVAO(cubeVerts);

    vector<float> cylVerts;
    generateCylinder(cylVerts, 32);
    GLuint cylinderVAO = createVAO(cylVerts);

    // Uniform locations we will set every frame
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
    // GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");

    // Light uniforms (set once here for colors)
    glUniform3f(glGetUniformLocation(shaderProgram, "light1Color"), 1.0f, 0.9f, 0.7f); // warm lantern
    glUniform3f(glGetUniformLocation(shaderProgram, "light2Color"), 0.5f, 0.6f, 0.9f); // moonlight
    glUniform3f(glGetUniformLocation(shaderProgram, "excavatorLightColor"), 0.5f, 0.4f, 0.3f);

    // load textures
    stbi_set_flip_vertically_on_load(true);
    walltexture = loadTexture("wall.jpg");
    floortexture = loadTexture("floor.jpg");

    // load spongebob textures
    sponge_front = loadTexture("spongebob/spongebob-front.png");
    sponge_left = loadTexture("spongebob/spongebob-left.png");
    sponge_right = loadTexture("spongebob/spongebob-right.png");
    sponge_back = loadTexture("spongebob/spongebob-back.png");
    sponge_top = loadTexture("spongebob/spongebob-top.png");

    // Projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Timing
    float lastTime = (float)glfwGetTime();

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        float now = (float)glfwGetTime();
        float deltaTime = now - lastTime;
        lastTime = now;

        glfwPollEvents();
        processInput(window, deltaTime);

        // Choose camera
        glm::mat4 view;
        glm::vec3 viewPos;
        if (currentCamera == 1) {
            viewPos = cameraPos1;
            view = glm::lookAt(cameraPos1, cameraTarget1, glm::vec3(0.0f, 1.0f, 0.0f));
        } else {
            // follow camera: behind and above excavator
            glm::vec3 followPos = glm::vec3(excavatorX + 4.0f, 3.0f, excavatorZ + 4.0f);
            viewPos = followPos;
            view = glm::lookAt(followPos, glm::vec3(excavatorX, 0.8f, excavatorZ), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniform3fv(viewPosLoc, 1, glm::value_ptr(viewPos));

        // Light positions: ceiling lantern and window moonlight
        glm::vec3 light1Pos = glm::vec3(0.0f, 5.6f, 0.0f);
        glm::vec3 light2Pos = glm::vec3(4.5f, 3.0f, -4.8f);
        glUniform3fv(glGetUniformLocation(shaderProgram, "light1Pos"), 1, glm::value_ptr(light1Pos));
        glUniform3fv(glGetUniformLocation(shaderProgram, "light2Pos"), 1, glm::value_ptr(light2Pos));

        // Light toggles
        glUniform1i(glGetUniformLocation(shaderProgram, "light1On"), light1On ? 1 : 0);
        glUniform1i(glGetUniformLocation(shaderProgram, "light2On"), light2On ? 1 : 0);
        glUniform1i(glGetUniformLocation(shaderProgram, "excavatorLightsOn"), excavatorLightsOn ? 1 : 0);

        // Clear
        glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw scene objects
        drawScene(shaderProgram, cubeVAO, cylinderVAO);

        // Draw excavator and update its excavator light positions inside
        drawExcavator(shaderProgram);

        // Finally, set excavator lights color and positions were set in drawExcavator; ensure excavator light color uniform is present
        glUniform3f(glGetUniformLocation(shaderProgram, "excavatorLightColor"), 0.5f, 0.4f, 0.3f);

        // Swap buffers
        glfwSwapBuffers(window);
    }

    // Cleanup (delete VAOs, program)
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &cylinderVAO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
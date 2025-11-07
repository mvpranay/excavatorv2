#ifndef _GL_FRAMEWORK_HPP_
#define _GL_FRAMEWORK_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

// Define a helpful macro for handling offsets into buffer objects
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

namespace csX75
{
  extern GLFWwindow * window;
  extern glm::mat4 view_matrix;
  extern glm::mat4 projection_matrix;

  //! Initialize GL State
  GLFWwindow * initGL(int width, int height, const char *);
 
  // --- Shader compilation helper ---
    GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

  //!GLFW Error Callback
  void error_callback(int error, const char* description);
  //!GLFW framebuffer resize callback
  void framebuffer_size_callback(GLFWwindow* window, int width, int height);
  //!GLFW keyboard callback
  void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif

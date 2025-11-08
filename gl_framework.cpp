#include "gl_framework.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "hierarchy_node.hpp"
#include "keyframe.hpp"

using namespace std;

// Light toggles
extern bool light1On;
extern bool light2On;
extern bool excavatorLightsOn;

extern sceneKeyFrame current_scene;

// Camera toggle
extern int currentCamera;
extern bool displayCameraPath;

extern KeyFrameMangager keyFrameManager;
extern cameraKeyPoint current_camera;

namespace csX75
{
  GLFWwindow * window = nullptr;
  glm::mat4 view_matrix = glm::lookAt(glm::vec3(0.0f, 3.0f, 8.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

  //! Initialize GL State
  GLFWwindow * initGL(int width=800, int height=800, const char* title="Hierarchical Model")
  {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
      cerr << "GLFW initialization failed!" << endl;
      exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        cerr << "Failed to create GLFW window!" << endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cerr << "GLEW initialization failed!" << endl;
        exit(-1);
    }
    
    //Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    //Set framebuffer clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    //Set depth buffer furthest depth
    glClearDepth(1.0);
    //Set depth test to less-than
    glDepthFunc(GL_LESS);

    return window;
  }

  // load shaders
  GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {
    auto read_file = [](const char* path) {
        ifstream file(path);
        stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    };

    string vCode = read_file(vertex_file_path);
    string fCode = read_file(fragment_file_path);

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vSrc = vCode.c_str();
    glShaderSource(vShader, 1, &vSrc, NULL);
    glCompileShader(vShader);

    GLint success;
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(vShader, 512, NULL, log);
        cerr << "Vertex shader compilation failed:\n" << log << endl;
    }

    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fSrc = fCode.c_str();
    glShaderSource(fShader, 1, &fSrc, NULL);
    glCompileShader(fShader);

    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(fShader, 512, NULL, log);
        cerr << "Fragment shader compilation failed:\n" << log << endl;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(program, 512, NULL, log);
        cerr << "Shader linking failed:\n" << log << endl;
    }

    glDeleteShader(vShader);
    glDeleteShader(fShader);
    return program;
  }
  
  //!GLFW Error Callback
  void error_callback(int error, const char* description)
  {
    cerr << description << endl;
  }
  
  //!GLFW framebuffer resize callback
  void framebuffer_size_callback(GLFWwindow* window, int width, int height)
  {
    //!Resize the viewport to fit the window size - draw to entire window
    glViewport(0, 0, width, height);
    // adjust to new dimensions
    projection_matrix = glm::perspective(glm::radians(45.0f), float(width)/float(height), 0.1f, 100.0f);
  }
  
  void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(action != GLFW_PRESS) return;

    switch(key) {
        //! Exit
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;

        //! Light toggles
        case GLFW_KEY_1:
            light1On = !light1On;
            break;
        case GLFW_KEY_2:
            light2On = !light2On;
            break;
        case GLFW_KEY_3:
            excavatorLightsOn = !excavatorLightsOn;
            break;

        //! Camera toggle
        case GLFW_KEY_C:
            if (mods & GLFW_MOD_SHIFT){
                if (keyFrameManager.saveCameraKeyFramesToFile("camera.txt")){
                    cout << "Camera keyframes saved to camera.txt" << endl;
                }
                else{
                    cout << "Failed to save camera keyframes." << endl;
                }
            }
            else{
                currentCamera = (currentCamera == 1) ? 2 : 1;
                break;
            }

        //! save current camera parameters in vector
        case GLFW_KEY_X: {
            keyFrameManager.cameraKeyPoints.push_back(current_camera);
            cout << "Pushed camera keyframe #" << keyFrameManager.cameraKeyPoints.size() << " to camera path." << endl;
            break;
        }

        // to display camera path
        case GLFW_KEY_V: {
            int num = keyFrameManager.cameraKeyPoints.size();
            if (num >= 4){
                displayCameraPath = !displayCameraPath;
                cout << "Camera path display toggled to " << (displayCameraPath ? "ON" : "OFF") << "." << endl;
            }
            break;
        }

        // save current scene parameters in vector
        case GLFW_KEY_R: {
            keyFrameManager.sceneKeyFrames.push_back(current_scene);
            cout << "Pushed scene keyframe #" << keyFrameManager.sceneKeyFrames.size() << "." << endl;
            break;
        }

        default:
            break;
    }
  }
};


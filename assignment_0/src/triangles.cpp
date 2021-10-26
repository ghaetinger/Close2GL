//////////////////////////////////////////////////////////////////////////////
//
//  Triangles.cpp
//
//////////////////////////////////////////////////////////////////////////////

#include "../include/LoadShaders.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, ColorBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0, vColor = 1 };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices = 3;

void initializeGLFW() {
  if (!glfwInit()) {
    std::cout << "Couldn't initialize GLFW" << std::endl;
  } else
    std::cout << "Initialized GLFW" << std::endl;
}

void initializeGLEW() {
  if (glewInit() != GLEW_OK) {
    std::cout << "Couldn't initialize GLEW" << std::endl;
  } else
    std::cout << "Initialized GLEW" << std::endl;
}

//----------------------------------------------------------------------------
//
// init
//

void init(void) {
  glGenVertexArrays(NumVAOs, VAOs);
  glBindVertexArray(VAOs[Triangles]);

  GLfloat vertices[NumVertices][2] = {
      {-0.90f, -0.90f}, {0.85f, -0.90f}, {-0.90f, 0.85f}};

  GLfloat colors[NumVertices][3] = {
      {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}};

  glCreateBuffers(NumBuffers, Buffers);
  glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
  glBufferStorage(GL_ARRAY_BUFFER, sizeof(vertices), vertices, 0);

  ShaderInfo shaders[] = {{GL_VERTEX_SHADER, "src/shaders/vert.glsl"},
                          {GL_FRAGMENT_SHADER, "src/shaders/frag.glsl"},
                          {GL_NONE, NULL}};

  GLuint program = LoadShaders(shaders);
  glUseProgram(program);

  glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vPosition);

  glBindBuffer(GL_ARRAY_BUFFER, Buffers[ColorBuffer]);
  glBufferStorage(GL_ARRAY_BUFFER, sizeof(colors), colors, 0);

  glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vColor);
}

//----------------------------------------------------------------------------
//
// display
//

void display(void) {
  static const float black[] = {0.0f, 0.0f, 0.0f, 0.0f};

  glClearBufferfv(GL_COLOR, 0, black);

  glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

//----------------------------------------------------------------------------
//
// main
//

#ifdef _WIN32
int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance,
                     _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
#else
int main(int argc, char **argv)
#endif
{

  initializeGLFW();

  GLFWwindow *window = glfwCreateWindow(800, 600, "Triangles", NULL, NULL);

  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
  }

  glfwMakeContextCurrent(window);

  initializeGLEW();

  init();

  while (!glfwWindowShouldClose(window)) {
    display();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);

  glfwTerminate();
}

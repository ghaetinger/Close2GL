// ╭───────────────────────────────────╮
// │ Made by Guilherme Gomes Haetinger │
// │  UFRGS - Computer Science (2021)  │
// ╰───────────────────────────────────╯

#include "../include/LoadShaders.hpp"
#include "../include/ShaderCompiler.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/matrix.hpp>
#include <iostream>
#include <sstream>

const int WIDTH = 1000;
const int HEIGHT = 1000;

enum Buffer_IDs { ArrayBuffer, NormalBuffer, NumBuffers };
enum Attrib_IDs { vPosition, vNormal };
enum CameraTypes { Free, Around };
enum WindingOrder { CCW = GL_CCW, CW = GL_CW };

GLuint Buffers[NumBuffers];

glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, -2.0f);
glm::vec3 lightPosition = glm::vec3(0.0f, 0.0f, -2.0f);
glm::vec3 objectPosition = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);

GLfloat adjustmentScale = 1;

GLuint NumVertices;

float camYaw = 0.0f;
float camPitch = 0.0f;

glm::vec3 u = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 v = glm::vec3(0.0f, 1.0f, 0.0f);

CameraTypes camType = Free;

glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.7f);
glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);

WindingOrder order = CW;

float near = 0.01f;
float far = 3000.0f;

bool holdingMouse = false;

glm::vec3 div(glm::vec3 vec, int d) {
  return glm::vec3(vec.x / d, vec.y / d, vec.z / d);
}

void initializeGLFW() {
  if (!glfwInit())
    exit(1);
  else
    std::cout << "Initialized GLFW" << std::endl;
}

void initializeGLEW() {
  if (glewInit() != GLEW_OK)
    exit(1);
  else
    std::cout << "Initialized GLEW" << std::endl;
}

void skipLines(std::istream &file, int num) {
  std::string useless_string;
  for (int i = 0; i < num; i++)
    std::getline(file, useless_string);
}

GLfloat getObjectAdjustmentScale(GLfloat x_bounds[2], GLfloat y_bounds[2],
                                 GLfloat z_bounds[2]) {

  return 1.0f / max(x_bounds[1] - x_bounds[0],
                    max(y_bounds[1] - y_bounds[0], z_bounds[1] - z_bounds[0]));
}

GLfloat setupObjectPositionAndScale(GLfloat *vertices, int numVertices) {
  GLfloat x_bounds[2] = {INFINITY, -INFINITY};
  GLfloat y_bounds[2] = {INFINITY, -INFINITY};
  GLfloat z_bounds[2] = {INFINITY, -INFINITY};

  for (int i = 0; i < NumVertices; i++) {

    GLfloat x = vertices[i * 3];
    GLfloat y = vertices[i * 3 + 1];
    GLfloat z = vertices[i * 3 + 2];

    if (x < x_bounds[0])
      x_bounds[0] = x;
    else if (x > x_bounds[1])
      x_bounds[1] = x;

    if (y < y_bounds[0])
      y_bounds[0] = y;
    else if (y > y_bounds[1])
      y_bounds[1] = y;

    if (z < z_bounds[0])
      z_bounds[0] = z;
    else if (z > z_bounds[1])
      z_bounds[1] = z;
  }

  GLfloat x_mean = x_bounds[0] + (x_bounds[1] - x_bounds[0]) / 2;
  GLfloat y_mean = y_bounds[0] + (y_bounds[1] - y_bounds[0]) / 2;
  GLfloat z_mean = z_bounds[0] + (z_bounds[1] - z_bounds[0]) / 2;

  objectPosition = glm::vec3(x_mean, y_mean, z_mean);
  return getObjectAdjustmentScale(x_bounds, y_bounds, z_bounds);
}

GLfloat **readFile(const char *fileName) {
  std::ifstream file(fileName);

  if (file) {
    std::string s;
    int numTriangles;

    skipLines(file, 1);
    if (file >> s >> s >> s >> numTriangles) {
      NumVertices = numTriangles * 3;
      GLfloat *vertices = (GLfloat *)malloc(NumVertices * 3 * sizeof(GLfloat));
      GLfloat *normals = (GLfloat *)malloc(NumVertices * 3 * sizeof(GLfloat));

      GLfloat x, y, z, nx, ny, nz;

      skipLines(file, 6);
      for (int i = 0; i < numTriangles; i++) {
        skipLines(file, 1);
        for (int j = 0; j < 3; j++) {
          file >> s >> x >> y >> z >> nx >> ny >> nz;
          vertices[i * 9 + j * 3] = x;
          vertices[i * 9 + j * 3 + 1] = y;
          vertices[i * 9 + j * 3 + 2] = z;

          normals[i * 9 + j * 3] = nx;
          normals[i * 9 + j * 3 + 1] = ny;
          normals[i * 9 + j * 3 + 2] = -nz;

          skipLines(file, 1);
        }
      }

      file.close();

      GLfloat **attrSet = (GLfloat **)malloc(2 * sizeof(GLfloat *));
      attrSet[0] = vertices;
      attrSet[1] = normals;
      return attrSet;
    } else {
      std::cout << "No number of triangles in " << fileName << std::endl;
      exit(1);
    }
  } else {
    std::cout << "Failed to read file " << fileName << std::endl;
    exit(1);
  }
}

void setupCamera(GLuint program, GLfloat scale) {
  glm::mat4 Model = glm::mat4(1.0f);
  Model = glm::scale(Model, glm::vec3(scale, scale, scale));
  Model = glm::translate(Model, origin - objectPosition);

  glm::mat4 Projection =
      glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, near, far);
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

  glm::mat4 View = glm::lookAt(cameraPosition, origin, up);

  if (camType == Free) {
    u = glm::vec3(cos(camYaw), 0.0f, -sin(camYaw));
    v = glm::vec3(0.0f, cos(camPitch), sin(camPitch));
    glm::vec3 front = normalize(cross(u, v));
    View = glm::lookAt(cameraPosition, cameraPosition + front, up);
  } else {
    v = up;
    glm::vec3 front = glm::normalize(-cameraPosition);
    u = cross(up, front);
  }

  glm::mat4 MVP = Projection * View * Model;

  GLuint MatrixID = glGetUniformLocation(program, "MVP");
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

  GLuint ModelID = glGetUniformLocation(program, "Model");
  glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Model[0][0]);

  GLuint CameraID = glGetUniformLocation(program, "CamPos");
  glUniform3fv(CameraID, 1, &cameraPosition[0]);
}

void setupColor(GLuint program) {
  GLuint ColorID = glGetUniformLocation(program, "Color");
  glUniform3fv(ColorID, 1, &color[0]);
  GLuint AmbientColorID = glGetUniformLocation(program, "AmbientColor");
  glUniform3fv(AmbientColorID, 1, &ambient[0]);
  GLuint LightPosID = glGetUniformLocation(program, "LightPos");
  glUniform3fv(LightPosID, 1, &lightPosition[0]);
}

void setRenderOptions(WindingOrder order) {
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  glFrontFace(order);
}

void init(char *filename) {
  glCreateBuffers(NumBuffers, Buffers);
  GLfloat **attrSet = readFile(filename);

  GLfloat *vertices = attrSet[0];
  GLfloat *normals = attrSet[1];

  adjustmentScale = setupObjectPositionAndScale(vertices, NumVertices);

  glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
  glBufferStorage(GL_ARRAY_BUFFER, NumVertices * 3 * sizeof(GLfloat), vertices,
                  0);

  glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vPosition);

  glBindBuffer(GL_ARRAY_BUFFER, Buffers[NormalBuffer]);
  glBufferStorage(GL_ARRAY_BUFFER, NumVertices * 3 * sizeof(GLfloat), normals,
                  0);

  glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vNormal);
}

void setupDisplay() {
  const char *vertexShaderPath = "src/shaders/vert.glsl";
  const char *fragmentShaderPath = "src/shaders/frag.glsl";

  string vertexShader = ParseShader(vertexShaderPath);
  string fragmentShader = ParseShader(fragmentShaderPath);

  GLuint program = CreateShader(vertexShader, fragmentShader);

  glUseProgram(program);
  setupCamera(program, adjustmentScale);
  setupColor(program);
  setRenderOptions(order);
}

void resetCam() {
  cameraPosition = glm::vec3(0.0f, 0.0f, -2.0f);
  u = glm::vec3(1.0f, 0.0f, 0.0f);
  u = glm::vec3(0.0f, 1.0f, 0.0f);
  camYaw = 0.0f;
  camPitch = 0.0f;
}

static void cursor_position_callback(GLFWwindow *window, double xpos,
                                     double ypos) {

  if (holdingMouse) {
    xpos = 2 * xpos - WIDTH;
    ypos = 2 * ypos - HEIGHT;
    camYaw = -xpos / WIDTH;
    camPitch = ypos / HEIGHT;
  }
}

bool is_shift_pressed(int mods) { return (mods & GLFW_MOD_SHIFT) != 0; }

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  float force = 0.5f;
  float color_change = 0.1f;

  if (action == GLFW_PRESS) {
    std::cout << "\r\e[K" << std::flush;
    switch (key) {
    case GLFW_KEY_W:
      cameraPosition += force * glm::cross(u, v);
      break;

    case GLFW_KEY_A:
      cameraPosition += force * u;
      break;

    case GLFW_KEY_S:
      cameraPosition -= force * glm::cross(u, v);
      break;

    case GLFW_KEY_D:
      cameraPosition -= force * u;
      break;

    case GLFW_KEY_C:
      std::cout << "Camera is now of type ";
      if (camType == Free) {
        camType = Around;
        std::cout << "Around" << std::flush;
      } else {
        camType = Free;
        std::cout << "Free" << std::flush;
      }
      break;

    case GLFW_KEY_R:
      resetCam();
      break;

    case GLFW_KEY_U:
      if (is_shift_pressed(mods))
        color.r = max(color.r - color_change, 0.0f);
      else
        color.r = min(color.r + color_change, 1.0f);

      std::cout << "Color: " << glm::to_string(color) << std::flush;
      break;

    case GLFW_KEY_I:
      if (is_shift_pressed(mods))
        color.g = max(color.g - color_change, 0.0f);
      else
        color.g = min(color.g + color_change, 1.0f);

      std::cout << "Color: " << glm::to_string(color) << std::flush;
      break;

    case GLFW_KEY_O:
      if (is_shift_pressed(mods))
        color.b = max(color.b - color_change, 0.0f);
      else
        color.b = min(color.b + color_change, 1.0f);

      std::cout << "Color: " << glm::to_string(color) << std::flush;
      break;

    case GLFW_KEY_J:
      if (is_shift_pressed(mods))
        ambient.r = max(ambient.r - color_change, 0.0f);
      else
        ambient.r = min(ambient.r + color_change, 1.0f);

      std::cout << "Ambient Color: " << glm::to_string(ambient) << std::flush;
      break;

    case GLFW_KEY_K:
      if (is_shift_pressed(mods))
        ambient.g = max(ambient.g - color_change, 0.0f);
      else
        ambient.g = min(ambient.g + color_change, 1.0f);

      std::cout << "Ambient Color: " << glm::to_string(ambient) << std::flush;
      break;

    case GLFW_KEY_L:
      if (is_shift_pressed(mods))
        ambient.b = max(ambient.b - color_change, 0.0f);
      else
        ambient.b = min(ambient.b + color_change, 1.0f);

      std::cout << "Ambient Color: " << glm::to_string(ambient) << std::flush;
      break;

    case GLFW_KEY_T:
      std::cout << "Winding Order is now ";
      if (order == CW) {
        order = CCW;
        std::cout << "Counter Clock-Wise" << std::flush;
      } else {
        order = CW;
        std::cout << "Clock-Wise" << std::flush;
      }
      break;

    case GLFW_KEY_M:
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      std::cout << "Normal Polygon Mode" << std::flush;
      break;

    case GLFW_KEY_N:
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      std::cout << "Wireframe Polygon Mode" << std::flush;
      break;

    case GLFW_KEY_B:
      glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
      std::cout << "Point Polygon Mode" << std::flush;
      break;

    case GLFW_KEY_F:
      if (is_shift_pressed(mods))
        far /= 2;
      else
        far *= 2;

      std::cout << "Far clipping plane = " << far << std::flush;
      break;

    case GLFW_KEY_G:
      if (is_shift_pressed(mods))
        near /= 2;
      else
        near *= 2;

      std::cout << "Near clipping plane = " << near << std::flush;
      break;

    case GLFW_KEY_Q:
      exit(0);
      break;
    }
  }
}

void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    holdingMouse = true;

  else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    holdingMouse = false;
}

void setupCallbacks(GLFWwindow *window) {
  glfwSetCursorPosCallback(window, cursor_position_callback);
  glfwSetKeyCallback(window, key_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
}

void printTutorial() {
  const char *tutorialText =
      "=== Commands ===\n"
      "Left Mouse Click -> Drag camera in free mode \n"
      "W, A, S, D -> Movement \n"
      "U, I, O -> Increases vertex color value (R, G, "
      "B, respectively) (+Shift decreases value) \n"
      "J, K, L -> Increases ambient color value (R, G, "
      "B, respectively) (+Shift decreases value) \n"
      "M -> Normal Polygon Mode \n"
      "N -> Wireframe Polygon Mode \n"
      "B -> Point Polygon Mode \n"
      "C -> Toggle Camera type (Free, Around) \n"
      "R -> Reset Camera Position \n"
      "F -> Increases Far clipping plane distance (+Shift decreases value) \n"
      "G -> Increases Near clipping plane distance (+Shift decreases value) \n"
      "T -> Toggle Winding Order \n";

  std::cout << tutorialText << std::endl;

  std::cout << " ======== Last Command Executed ======== " << std::endl;
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

int main(int argc, char **argv) {

  if (argc < 2) {
    std::cout << "Please input file name" << std::endl;
    exit(1);
  }

  char *filename = argv[1];

  initializeGLFW();

  GLFWwindow *window =
      glfwCreateWindow(WIDTH, HEIGHT, "Object Viewer", NULL, NULL);

  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
  }

  glfwMakeContextCurrent(window);

  initializeGLEW();

  init(filename);

  setupCallbacks(window);

  printTutorial();

  while (!glfwWindowShouldClose(window)) {
    setupDisplay();
    display();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);

  glfwTerminate();
}

// ╭───────────────────────────────────╮
// │ Made by Guilherme Gomes Haetinger │
// │  UFRGS - Computer Science (2021)  │
// ╰───────────────────────────────────╯

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gtkmm-3.0/gtkmm.h>
#include <gtkmm-3.0/gtkmm/application.h>

#include <fstream>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/matrix.hpp>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

#include "../include/LoadShaders.hpp"
#include "../include/ShaderCompiler.hpp"
#include "../include/camera.hpp"
#include "../include/matrices.hpp"
#include "../include/option_window.hpp"
#include "../include/options.hpp"
#include "../include/triangle.hpp"

int width = 1000;
int height = 1000;

enum Buffer_IDs { ArrayBuffer, NormalBuffer, NumBuffers };
enum Attrib_IDs { vPosition, vNormal };

GLuint Buffers[NumBuffers];

glm::vec3 lightPosition = glm::vec3(0.0f, 0.0f, -2.0f);
glm::vec3 objectPosition = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);

glm::mat4 M, V, P;

GLfloat adjustmentScale = 1;

GLuint NumVertices;
GLuint TrueNumVertices;

float camYaw = 0.0f;
float camPitch = 0.0f;

Camera camera(glm::vec3(0.0f, 0.0f, -2.0f));

bool holdingMouse = false;

Options *options;

static GLuint texture;

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
  for (int i = 0; i < num; i++) std::getline(file, useless_string);
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
      TrueNumVertices = NumVertices;
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
  if (options->getShouldResetCam()) {
    options->setShouldResetCam(false);
    camera.reset();
  }

  glm::mat4 Model = glm::mat4(1.0f);
  Model = glm::scale(Model, glm::vec3(scale, scale, scale));
  Model = glm::translate(Model, origin - objectPosition);

  glViewport(0, 0, width, height);
  glm::mat4 Projection = glm::perspective(
      options->getVfov(), 4.0f / 3.0f, options->getNear(), options->getFar());
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

  glm::mat4 View = glm::lookAt(camera.getPosition(), origin, up);

  if (options->getCamType() == Free) {
    camera.setUV(glm::vec3(cos(camYaw), 0.0f, -sin(camYaw)),
                 glm::vec3(0.0f, cos(camPitch), sin(camPitch)));
    glm::vec3 front = normalize(cross(camera.getU(), camera.getV()));
    View = glm::lookAt(camera.getPosition(), camera.getPosition() + front, up);
  } else {
    glm::vec3 front = glm::normalize(-camera.getPosition());
    camera.setUV(cross(up, front), up);
  }

  glm::mat4 MVP = Projection * View * Model;

  GLuint MatrixID = glGetUniformLocation(program, "MVP");
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

  GLuint ModelID = glGetUniformLocation(program, "Model");
  glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Model[0][0]);
}

GLfloat *close2GLTriangleProcessing(GLfloat *vtxArray, GLfloat *normalArray,
                                    float scale) {
  int numTriangles = NumVertices / 3;
  // NumVertices = 0;

  if (options->getShouldResetCam()) {
    options->setShouldResetCam(false);
    camera.reset();
  }

  glm::vec3 up = glm::vec3(0, 1, 0);

  glViewport(0, 0, width, height);

  camera.setFOV(options->getHfov(), options->getVfov());

  M = mtxs::scaleMatrix(scale) * mtxs::translateMatrix(origin - objectPosition);
  P = mtxs::projectionMatrix(camera, options->getNear(), options->getFar());
  V = mtxs::viewMatrix(camera, origin);
  glm::mat4 Viewport = mtxs::viewportMatrix(width, height);

  glm::mat4 VPInv = glm::inverse(Viewport * P);

  GLfloat *colors =
      (GLfloat *)malloc((width * height + 1) * 4 * sizeof(GLfloat));
  GLfloat *zbuffer =
      (GLfloat *)malloc(width * height * sizeof(GLfloat) + sizeof(GLfloat));

  for (int i = 0; i < (width * height + 1) * 4; i++) colors[i] = 0.0f;

  for (int i = 0; i < (width * height + 1); i++)
    zbuffer[i] = 10000.0f;  // Very high value

  if (options->getCamType() == Free) {
    camera.setUV(glm::vec3(cos(camYaw), 0.0f, -sin(camYaw)),
                 glm::vec3(0.0f, cos(camPitch), sin(camPitch)));
    glm::vec3 front = normalize(cross(camera.getU(), camera.getV()));

    V = mtxs::viewMatrix(camera, camera.getPosition() + front);
  } else {
    glm::vec3 front = glm::normalize(-camera.getPosition());
    camera.setUV(cross(up, front), up);
  }

  int count = 0;

  for (int i = 0; i < numTriangles; i++) {
    int x = i * 9;
    int y = i * 9 + 1;
    int z = i * 9 + 2;

    glm::vec3 vtxs[3] = {
        glm::vec3(vtxArray[x], vtxArray[y], vtxArray[z]),
        glm::vec3(vtxArray[x + 3], vtxArray[y + 3], vtxArray[z + 3]),
        glm::vec3(vtxArray[x + 6], vtxArray[y + 6], vtxArray[z + 6]),
    };

    glm::vec3 normals[3] = {
        glm::vec3(normalArray[x], normalArray[y], normalArray[z]),
        glm::vec3(normalArray[x + 3], normalArray[y + 3], normalArray[z + 3]),
        glm::vec3(normalArray[x + 6], normalArray[y + 6], normalArray[z + 6]),
    };

    Triangle triangle(vtxs, normals);

    triangle.applyMatrix(M);

    triangle.computeColors(options->getShaderMode(),
                           glm::vec4(options->getColor(), 1.0f),
                           glm::vec4(options->getAmbient(), 1.0f),
                           glm::vec4(camera.getPosition(), 1.0f),
                           glm::vec4(lightPosition, 1.0f), M);

    triangle.applyMatrix(P * V);

    if (triangle.shouldFrustumCull() ||
        triangle.shouldPlaneCull(options->getNear(), options->getFar()))
      continue;

    // We should cull points outside the -1 to 1 after the normalization
    triangle.normalize();

    if (!triangle.isFrontFacing(options->getOrder()) &&
        options->getShouldBackfaceCull())
      continue;

    triangle.applyMatrix(Viewport);
    triangle.rasterize(colors, zbuffer, width, height, options->getShaderMode(),
                       options->getRasterMode(),
                       glm::vec4(options->getAmbient(), 1.0f),
                       glm::vec4(camera.getPosition(), 1.0f),
                       glm::vec4(lightPosition, 1.0f), M, P * V * M, Viewport);
  }

  free(zbuffer);
  return colors;
}

void setupColor(GLuint program, GLfloat *vertices, GLfloat *normals) {
  GLuint ColorID = glGetUniformLocation(program, "Color");
  glUniform3fv(ColorID, 1, &options->getColor()[0]);
  GLuint AmbientColorID = glGetUniformLocation(program, "AmbientColor");
  glUniform3fv(AmbientColorID, 1, &options->getAmbient()[0]);
  GLuint LightPosID = glGetUniformLocation(program, "LightPos");
  glUniform3fv(LightPosID, 1, &lightPosition[0]);

  GLuint idx = options->getShaderMode();
  glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &idx);
  glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &idx);

  if (options->getUsingClose2GL()) {
    GLfloat *colors =
        close2GLTriangleProcessing(vertices, normals, adjustmentScale);
    float z = 0.0f;
    glBindTexture(GL_TEXTURE_2D, texture);
    glClearTexImage(texture, 0, GL_RGBA, GL_FLOAT, &z);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT,
                 colors);

    free(colors);
  }
}

void setRenderOptions(GLuint program) {
  if (!options->getUsingClose2GL() && options->getShouldBackfaceCull()) {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(options->getOrder());
  } else {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
  }

  GLuint renderOpID = glGetUniformLocation(program, "renderOp");
  glUniform1i(renderOpID, options->getUsingClose2GL());
}

void setupDisplay(GLfloat *vertices, GLfloat *normals) {
  const char *vertexShaderPath = "src/shaders/vert.glsl";
  const char *fragmentShaderPath = "src/shaders/frag.glsl";

  string vertexShader = ParseShader(vertexShaderPath);
  string fragmentShader = ParseShader(fragmentShaderPath);

  GLuint program = CreateShader(vertexShader, fragmentShader);

  glUseProgram(program);
  if (!options->getUsingClose2GL()) {
    glPolygonMode(GL_FRONT_AND_BACK, options->getRasterMode());
    setupCamera(program, adjustmentScale);
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    GLuint MatrixID = glGetUniformLocation(program, "MVP");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(P * V * M)[0][0]);

    GLuint ModelID = glGetUniformLocation(program, "Model");
    glUniformMatrix4fv(ModelID, 1, GL_FALSE, &M[0][0]);
  }

  GLuint CameraID = glGetUniformLocation(program, "CamPos");
  glUniform3fv(CameraID, 1, &camera.getPosition()[0]);

  setRenderOptions(program);
  setupColor(program, vertices, normals);
}

void init(GLfloat *vertices, GLfloat *normals) {
  glCreateBuffers(NumBuffers, Buffers);

  adjustmentScale = setupObjectPositionAndScale(vertices, NumVertices);

  if (options->getUsingClose2GL()) {
    float vtxCoords[] = {-1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,
                         1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f};

    float textureCoords[] = {0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                             1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
    glBufferStorage(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vtxCoords, 0);

    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPosition);

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[NormalBuffer]);
    glBufferStorage(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), textureCoords, 0);

    glVertexAttribPointer(vNormal, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vNormal);
  } else if (NumVertices > 0) {
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
    glBufferStorage(GL_ARRAY_BUFFER, NumVertices * 3 * sizeof(GLfloat),
                    vertices, 0);

    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPosition);

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[NormalBuffer]);
    glBufferStorage(GL_ARRAY_BUFFER, NumVertices * 3 * sizeof(GLfloat), normals,
                    0);

    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vNormal);
  } else {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
}

static void cursor_position_callback(GLFWwindow *window, double xpos,
                                     double ypos) {
  if (holdingMouse) {
    xpos = 2 * xpos - width;
    ypos = 2 * ypos - height;
    camYaw = -xpos / width;
    camPitch = ypos / height;
  }
}

bool is_shift_pressed(int mods) { return (mods & GLFW_MOD_SHIFT) != 0; }

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  float force = 0.5f;

  if (action == GLFW_PRESS) switch (key) {
      case GLFW_KEY_W:
        camera.move(force, glm::cross(camera.getU(), camera.getV()));
        break;

      case GLFW_KEY_A:
        camera.move(force, camera.getU());
        break;

      case GLFW_KEY_S:
        camera.move(-force, glm::cross(camera.getU(), camera.getV()));
        break;

      case GLFW_KEY_D:
        camera.move(-force, camera.getU());
        break;

      case GLFW_KEY_Q:
        exit(0);
        break;
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
      "W, A, S, D -> Movement \n";

  std::cout << tutorialText << std::endl;
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (options->getUsingClose2GL()) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
  } else
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void displayFPS(int frames) {
  std::cout << "\r\e[K" << std::flush;
  std::cout << frames << std::flush;
}

void glEventLoop(char **argv) {
  char *filename = argv[1];

  initializeGLFW();

  GLFWwindow *window =
      glfwCreateWindow(width, height, "Object Viewer", NULL, NULL);

  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
  }

  glfwMakeContextCurrent(window);

  initializeGLEW();

  GLfloat **attrSet = readFile(filename);

  GLfloat *vertices = attrSet[0];
  GLfloat *normals = attrSet[1];

  if (!options->getUsingClose2GL()) init(vertices, normals);

  setupCallbacks(window);

  GLfloat *cpvertices = (GLfloat *)malloc(NumVertices * 3 * sizeof(GLfloat));
  GLfloat *cpnormals = (GLfloat *)malloc(NumVertices * 3 * sizeof(GLfloat));

  double previousTime = glfwGetTime();
  int frameCount = 0;

  printTutorial();
  GLfloat *colors = (GLfloat *)malloc(width * height * 4 * sizeof(GLfloat) +
                                      4 * sizeof(GLfloat));

  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT,
               colors);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  free(colors);

  while (!glfwWindowShouldClose(window)) {
    double currentTime = glfwGetTime();

    displayFPS(1 / (currentTime - previousTime));

    previousTime = currentTime;

    if (options->getShouldSwitchRenderMode() || options->getUsingClose2GL()) {
      if (options->getShouldSwitchRenderMode())
        options->setShouldSwitchRenderMode(false);

      memcpy(cpvertices, vertices, NumVertices * 3 * sizeof(GLfloat));
      memcpy(cpnormals, normals, NumVertices * 3 * sizeof(GLfloat));

      init(cpvertices, cpnormals);
      NumVertices = TrueNumVertices;
    }

    glfwGetWindowSize(window, &width, &height);

    setupDisplay(cpvertices, cpnormals);
    display();
    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  glfwDestroyWindow(window);

  glfwTerminate();
  exit(1);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Please input file name" << std::endl;
    exit(1);
  }

  options = new Options();

  std::thread glThread(glEventLoop, argv);

  auto app = Gtk::Application::create("org.gtkmm.example");
  OptionWindow opwin(options);
  return app->run(opwin);

  // TODO: LOW PRIORITY Scale for ambience influence
}

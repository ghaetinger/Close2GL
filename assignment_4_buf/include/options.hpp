#ifndef OPTIONS_H
#define OPTIONS_H

// GLM
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

enum CameraType { Free, Around };
enum WindingOrder { CCW = GL_CCW, CW = GL_CW };
enum RasterMode { Solid = GL_FILL, Wireframe = GL_LINE, Point = GL_POINT };
enum ShaderMode { Color = 0, Gourad = 1, Phong = 2, NoShading = 3 };
enum TextureMode { NearestNeighbor = 0, Bilinear = 1, MipMap = 2 };

class Options {
public:
  Options();

  // Getters
  CameraType getCamType();
  glm::vec3 getColor();
  glm::vec3 getAmbient();
  WindingOrder getOrder();
  RasterMode getRasterMode();
  ShaderMode getShaderMode();
  TextureMode getTextureMode();
  float getNear();
  float getFar();
  float getHfov();
  float getVfov();
  bool getUsingClose2GL();
  bool getShouldResetCam();
  bool getShouldSwitchRenderMode();
  bool getShouldBackfaceCull();
  bool getShouldUseTexture();

  // Setters
  void setCamType(CameraType type);
  void setColor(glm::vec3 vec);
  void setAmbient(glm::vec3 vec);
  void setOrder(WindingOrder order);
  void setRasterMode(RasterMode mode);
  void setShaderMode(ShaderMode mode);
  void setTextureMode(TextureMode texMode);
  void setNear(float val);
  void setFar(float val);
  void setHfov(float val);
  void setVfov(float val);
  void setUsingClose2GL(bool isUsing);
  void setShouldResetCam(bool should);
  void setShouldSwitchRenderMode(bool should);
  void setShouldBackfaceCull(bool should);
  void setShouldUseTexture(bool should);

private:
  CameraType camType;
  glm::vec3 color;
  glm::vec3 ambient;

  WindingOrder order;
  RasterMode rasterMode;
  ShaderMode shaderMode;

  float near;
  float far;

  float hfov;
  float vfov;

  bool usingClose2GL;

  bool shouldResetCam;
  bool shouldSwitchRenderMode;

  bool shouldBackfaceCull;

  bool usesTexture;
  TextureMode texMode;
};

#endif

#ifndef CAMERA_HPP
#define CAMERA_HPP

// GLM
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

class Camera {
public:
  Camera(glm::vec3 position);

  void setUVN(glm::vec3 u, glm::vec3 v, glm::vec3 n);
  void setUV(glm::vec3 u, glm::vec3 v);
  void setPosition(glm::vec3 pos);
  void setFOV(float hfov, float vfov);
  void move(float mult, glm::vec3 direction);

  glm::vec3 getU();
  glm::vec3 getV();
  glm::vec3 getN();
  glm::vec3 getPosition();
  void getFOV(float *hfov, float *vfov);

  void reset();

private:
  glm::vec3 u, v, n, pos, initialPos;
  float hfov, vfov;
};

#endif

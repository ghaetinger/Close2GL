#ifndef MATRICES_HPP
#define MATRICES_HPP

#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../include/camera.hpp"

namespace mtxs
{
  glm::mat4 translateMatrix(glm::vec3 direction);
  glm::mat4 scaleMatrix(float scale);
  glm::mat4 viewportMatrix(int width, int height);
  glm::mat4 projectionMatrix(Camera camera, float near, float far);
  glm::mat4 viewMatrix(Camera camera, glm::vec3 center);
}

#endif

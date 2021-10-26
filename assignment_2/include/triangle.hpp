#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

#include "../include/camera.hpp"
#include "../include/options.hpp"

class Triangle {
public:
  Triangle(glm::vec3 vtxs[3], glm::vec3 normals[3]);

  bool shouldFrustumCull();
  bool shouldPlaneCull(float near, float far);
  bool isFrontFacing(WindingOrder order);
  void fillVertexArray(float *vtxArray, float *normalArray, int index);
  void applyMatrix(glm::mat4 mtx);
  void normalize();
  void print();

private:
  glm::vec4 vtxs[3];
  glm::vec3 normals[3];
};

#endif

#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <algorithm>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <vector>

#include "../include/camera.hpp"
#include "../include/options.hpp"
#include "../include/vertex.hpp"

class Triangle {
 public:
  Triangle(glm::vec3 vtxs[3], glm::vec3 normals[3]);

  bool shouldFrustumCull();
  bool shouldPlaneCull(float near, float far);
  bool isFrontFacing(WindingOrder order);
  void computeColors(ShaderMode shader, glm::vec4 vertColor,
                     glm::vec4 ambientColor, glm::vec4 camPos,
                     glm::vec4 lightPos, glm::mat4 Model);
  void fillVertexArray(float *vtxArray, float *normalArray, int index);
  void rasterize(float *colors, float *zbuffer, int width, int height,
                 ShaderMode shader, RasterMode raster, glm::vec4 ambientColor,
                 glm::vec4 camPos, glm::vec4 lightPos, glm::mat4 Model,
                 glm::mat4 MVP, glm::mat4 Viewport);
  void applyMatrix(glm::mat4 mtx);
  void normalize();
  void print();

 private:
  Vertex vtxs[3];
};

#endif

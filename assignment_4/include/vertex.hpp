#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

class Vertex {
public:
  Vertex();
  Vertex(glm::vec4 pos, glm::vec4 color, glm::vec3 normal, float w,
         glm::vec2 texcoords);
  glm::vec4 pos;
  glm::vec4 color;
  glm::vec3 normal;
  float w;
  glm::vec2 texcoords;

  glm::vec4 specular;
  float diff;
};

#endif

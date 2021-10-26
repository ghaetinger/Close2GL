#include "../include/vertex.hpp"

Vertex::Vertex() {
  this->pos = glm::vec4();
  this->color = glm::vec4();
  this->normal = glm::vec3();
  this->w = 1.0f;
  this->texcoords = glm::vec2();
}

Vertex::Vertex(glm::vec4 pos, glm::vec4 color, glm::vec3 normal, float w,
               glm::vec2 texcoords) {
  this->pos = pos;
  this->color = color;
  this->normal = normal;
  this->texcoords = texcoords;
  this->w = w;
}

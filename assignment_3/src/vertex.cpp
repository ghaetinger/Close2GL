#include "../include/vertex.hpp"

Vertex::Vertex()
{
    this->pos = glm::vec4();
    this->color = glm::vec4();
    this->normal = glm::vec3();
    this->w = 1.0f;
}

Vertex::Vertex(glm::vec4 pos, glm::vec4 color, glm::vec3 normal, float w)
{
    this->pos = pos;
    this->color = color;
    this->normal = normal;
    this->w = w;
}
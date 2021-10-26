#include "../include/camera.hpp"

Camera::Camera(glm::vec3 position) {
  this->pos = position;
  this->initialPos = position;
  this->u = glm::vec3(1.0f, 0.0f, 0.0f);
  this->v = glm::vec3(0.0f, 1.0f, 0.0f);
  this->n = glm::vec3(0.0f, 0.0f, -1.0f);
  this->hfov = glm::radians(45.0f);
  this->vfov = glm::radians(45.0f);
}

void Camera::move(float mult, glm::vec3 direction) {
  this->pos += mult * direction;
}

void Camera::setUVN(glm::vec3 u, glm::vec3 v, glm::vec3 n) {
  this->u = u;
  this->v = v;
  this->n = n;
}

void Camera::setUV(glm::vec3 u, glm::vec3 v) {
  this->u = u;
  this->v = v;
  // this->n = glm::cross(u, v);
}

void Camera::setPosition(glm::vec3 pos) {
  this->pos = pos;
}

void Camera::setFOV(float hfov, float vfov) {
  this->hfov = hfov;
  this->vfov = vfov;
}

glm::vec3 Camera::getU() {return this->u;}
glm::vec3 Camera::getV() {return this->v;}
glm::vec3 Camera::getN() {return this->n;}
glm::vec3 Camera::getPosition() {return this->pos;}
void Camera::getFOV(float *hfov, float *vfov) {
  *hfov = this->hfov;
  *vfov = this->vfov;
}

void Camera::reset() {
  this->pos = this->initialPos;
  this->u = glm::vec3(1.0f, 0.0f, 0.0f);
  this->v = glm::vec3(0.0f, 1.0f, 0.0f);
  this->n = glm::vec3(0.0f, 0.0f, -1.0f);
  this->hfov = glm::radians(45.0f);
  this->vfov = glm::radians(45.0f);
}

#include "../include/matrices.hpp"

glm::mat4 mtxs::translateMatrix(glm::vec3 direction)
{
  return glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, direction.x, direction.y,
                   direction.z, 1);
}

glm::mat4 mtxs::scaleMatrix(float scale)
{
  return glm::mat4(scale, 0, 0, 0, 0, scale, 0, 0, 0, 0, scale, 0, 0, 0, 0, 1);
}

glm::mat4 mtxs::viewportMatrix(int width, int height)
{
  return glm::mat4(
      width / 2.0f, 0, 0, 0, 0, -height / 2.0f, 0, 0, 0, 0, 1.0f, 0, width / 2.0f, height / 2.0f, 0, 1.0f);
}

glm::mat4 mtxs::projectionMatrix(Camera camera, float near, float far)
{
  float hfov, vfov;
  camera.getFOV(&hfov, &vfov);
  float hS = 1.0f / ((4.0f / 3.0f) * glm::tan(hfov / 2.0f));
  float vS = 1.0f / (glm::tan(vfov / 2.0f));
  float fieldDiff = far - near;
  return glm::mat4(hS, 0, 0, 0, 0, vS, 0, 0, 0, 0, -(far + near) / fieldDiff,
                   -1, 0, 0, -(2 * far * near) / fieldDiff, 0);
}

glm::mat4 mtxs::viewMatrix(Camera camera, glm::vec3 center)
{

  glm::vec3 dir = glm::normalize(camera.getPosition() - center);
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 u = glm::normalize(glm::cross(up, dir));
  glm::vec3 v = glm::cross(dir, u);

  float dx = -glm::dot(u, camera.getPosition());
  float dy = -glm::dot(v, camera.getPosition());
  float dz = -glm::dot(dir, camera.getPosition());

  return glm::mat4(u.x, v.x, dir.x, 0, u.y, v.y, dir.y, 0, u.z, v.z, dir.z, 0,
                   dx, dy, dz, 1);
}

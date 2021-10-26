#include "../include/triangle.hpp"

const float ERROR = 0.00f;

Triangle::Triangle(glm::vec3 vtxs[3], glm::vec3 normals[3]) {
  this->vtxs[0] = glm::vec4(vtxs[0], 1.0f);
  this->vtxs[1] = glm::vec4(vtxs[1], 1.0f);
  this->vtxs[2] = glm::vec4(vtxs[2], 1.0f);

  this->normals[0] = normals[0];
  this->normals[1] = normals[1];
  this->normals[2] = normals[2];
}

bool vecPosOutOfNormalizedSpace(glm::vec4 v) {
  v /= v.w;
  return v.x < (-1 - ERROR) || v.x > (1 + ERROR) || v.y < (-1 - ERROR) ||
         v.y > (1 + ERROR) || v.z < (-1 - ERROR) || v.z > (1 + ERROR);
}

bool Triangle::shouldFrustumCull() {
  for (int i = 0; i < 3; i++)
    if (!vecPosOutOfNormalizedSpace(this->vtxs[i]))
      return false;

  return true;
};

bool Triangle::shouldPlaneCull(float near, float far) {
  for (int i = 0; i < 3; i++)
    if (this->vtxs[i].w < near || this->vtxs[i].w > far)
      return true;

  return false;
}

int modAdd(int a, int b, int m) { return (a + b) % m; }
glm::vec4 screenSpaceVtx(glm::vec4 v) {
  v += 1.0f;
  v /= 2.0f;
  return v;
}

bool Triangle::isFrontFacing(WindingOrder order) {
  float a = 0;
  for (int i = 0; i < 3; i++) {
    glm::vec4 screenV = screenSpaceVtx(this->vtxs[i]);
    glm::vec4 nextScreenV = screenSpaceVtx(this->vtxs[modAdd(i, 1, 3)]);
    a += screenV.x * nextScreenV.y - screenV.y * nextScreenV.x;
  }

  a /= 2.0f;

  if (order == CCW)
    return a >= 0.0f;
  else
    return a <= 0.0f;
};

void Triangle::fillVertexArray(float *vtxArray, float *normalArray, int index) {

  int x = index * 9;
  int y = index * 9 + 1;
  int z = index * 9 + 2;

  vtxArray[x] = this->vtxs[0].x;
  vtxArray[y] = this->vtxs[0].y;
  vtxArray[z] = this->vtxs[0].z;
  vtxArray[x + 3] = this->vtxs[1].x;
  vtxArray[y + 3] = this->vtxs[1].y;
  vtxArray[z + 3] = this->vtxs[1].z;
  vtxArray[x + 6] = this->vtxs[2].x;
  vtxArray[y + 6] = this->vtxs[2].y;
  vtxArray[z + 6] = this->vtxs[2].z;

  normalArray[x] = this->normals[0].x;
  normalArray[y] = this->normals[0].y;
  normalArray[z] = this->normals[0].z;
  normalArray[x + 3] = this->normals[1].x;
  normalArray[y + 3] = this->normals[1].y;
  normalArray[z + 3] = this->normals[1].z;
  normalArray[x + 6] = this->normals[2].x;
  normalArray[y + 6] = this->normals[2].y;
  normalArray[z + 6] = this->normals[2].z;
};

void Triangle::applyMatrix(glm::mat4 mtx) {
  for (int i = 0; i < 3; i++)
    this->vtxs[i] = mtx * this->vtxs[i];
}

void Triangle::normalize() {
  for (int i = 0; i < 3; i++)
    this->vtxs[i] /= this->vtxs[i].w;
}

void Triangle::print() {
  std::cout << glm::to_string(this->vtxs[0]) << " -- "
            << glm::to_string(this->vtxs[1]) << " -- "
            << glm::to_string(this->vtxs[2]) << std::endl;
}

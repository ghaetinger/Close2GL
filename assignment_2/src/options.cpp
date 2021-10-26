#include "../include/options.hpp"
#include <glm/trigonometric.hpp>

Options::Options() {
  this->camType = Free;
  this->color = glm::vec3(0.0f, 0.0f, 0.7f);
  this->ambient = glm::vec3(0.2f, 0.2f, 0.2f);
  this->order = CW;
  this->rasterMode = Solid;
  this->shaderMode = Phong;
  this->near = 0.01f;
  this->far = 30.0f;
  this->usingClose2GL = false;
  this->shouldBackfaceCull = true;
  this->hfov = glm::radians(60.0f);
  this->vfov = glm::radians(60.0f);
}

CameraType Options::getCamType() { return this->camType; }
glm::vec3 Options::getAmbient() { return this->ambient; }
glm::vec3 Options::getColor() { return this->color; }
WindingOrder Options::getOrder() { return this->order; };
RasterMode Options::getRasterMode() { return this->rasterMode; };
ShaderMode Options::getShaderMode() { return this->shaderMode; };
float Options::getNear() { return this->near; };
float Options::getFar() { return this->far; };
float Options::getHfov() { return this->hfov; };
float Options::getVfov() { return this->vfov; };
bool Options::getUsingClose2GL() { return this->usingClose2GL; }
bool Options::getShouldResetCam() { return this->shouldResetCam; }
bool Options::getShouldSwitchRenderMode() {
  return this->shouldSwitchRenderMode;
}
bool Options::getShouldBackfaceCull() { return this->shouldBackfaceCull; }

void Options::setCamType(CameraType type) { this->camType = type; };
void Options::setColor(glm::vec3 vec) { this->color = vec; };
void Options::setAmbient(glm::vec3 vec) { this->ambient = vec; };
void Options::setOrder(WindingOrder order) { this->order = order; };
void Options::setRasterMode(RasterMode mode) { this->rasterMode = mode; };
void Options::setShaderMode(ShaderMode mode) { this->shaderMode = mode; };
void Options::setNear(float val) { this->near = val; };
void Options::setFar(float val) { this->far = val; };
void Options::setHfov(float val) { this->hfov = val; };
void Options::setVfov(float val) { this->vfov = val; };
void Options::setUsingClose2GL(bool isUsing) { this->usingClose2GL = isUsing; }
void Options::setShouldResetCam(bool should) { this->shouldResetCam = should; }
void Options::setShouldSwitchRenderMode(bool should) {
  this->shouldSwitchRenderMode = should;
};
void Options::setShouldBackfaceCull(bool should) {
  this->shouldBackfaceCull = should;
}

#version 400 core

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 normal;

uniform mat4 Model;
uniform mat4 MVP;
uniform vec3 CamPos;
uniform vec3 Color;
uniform vec3 AmbientColor;
uniform vec3 LightPos;

out vec4 fPosition;
out vec4 fNormal;
out vec3 fCamPos;
out vec3 fVertColor;
out vec3 fAmbient;
out vec3 fLightPos;

void main() {
  fCamPos = CamPos;
  fLightPos = LightPos;
  fVertColor = Color;
  fAmbient = AmbientColor;
  fPosition = Model * vPosition;
  fNormal = transpose(inverse(Model)) * normal;
  gl_Position = MVP * vPosition;
}

#version 450 core

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 normal;

uniform mat4 Model;
uniform mat4 MVP;
uniform vec3 CamPos;
uniform vec3 Color;
uniform vec3 AmbientColor;
uniform vec3 LightPos;
uniform int renderOp;

out vec4 fPosition;
out vec4 fNormal;
out vec4 fVertColor;

subroutine vec4 shadingRoutine(vec4); 

layout(index=0) subroutine(shadingRoutine) vec4 gouraud_ad(vec4 pos) {
  vec4 norm = normalize(transpose(inverse(Model)) * normal);
  vec4 lightDir = normalize(vec4(LightPos, 1.0f) - pos);
  float diff = max(dot(norm, lightDir), 0);

  vec4 viewDir = normalize(vec4(CamPos, 1.0f) - pos);
  vec4 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

  vec4 diffuse = diff * vec4(Color, 1.0f);
  vec4 ambient = 0.3f * vec4(AmbientColor, 1.0f);

  return diffuse + ambient;
}

layout(index=1) subroutine(shadingRoutine) vec4 gouraud_ads(vec4 pos) {
  vec4 norm = normalize(transpose(inverse(Model)) * normal);
  vec4 lightDir = normalize(vec4(LightPos, 1.0f) - pos);
  float diff = max(dot(norm, lightDir), 0);

  vec4 viewDir = normalize(vec4(CamPos, 1.0f) - pos);
  vec4 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

  float specularStrength = 0.5f;
  vec4 specular = specularStrength * spec * vec4(1.0f, 1.0f, 1.0f, 1.0f);
  vec4 diffuse = (diff * vec4(Color, 1.0f)) * (1.0f - specularStrength);
  vec4 ambient = 0.3f * vec4(AmbientColor, 1.0f);

  return diffuse + specular + ambient;
}

layout(index=2) subroutine(shadingRoutine) vec4 phong(vec4 pos) {
    return vec4(Color, 1.0f);
}

layout(index=3) subroutine(shadingRoutine) vec4 no_shade(vec4 pos) {
    return vec4(Color, 1.0f);
}

vec4 close_2_gl_pos() {
  return normalize(Model * inverse(MVP) * vPosition);
}

vec4 open_gl_pos() {
  return Model * vPosition;
}

void prepare_close_2_gl_output() {
  gl_Position = vPosition;
  fNormal = transpose(inverse(Model)) * normal;
  fPosition = normalize(Model * inverse(MVP) * vPosition);
}

void prepare_open_gl_output() {
  gl_Position = MVP * vPosition;
  fNormal = transpose(inverse(Model)) * normal;
  fPosition = normalize(Model * vPosition);
}

subroutine uniform shadingRoutine myShadingRoutine;

void main() {
  vec4 pos;
  if (renderOp == 1) {
    gl_Position = vPosition;
    fPosition = vPosition;
    fNormal = normal;
    fVertColor = vec4(1, 0, 0, 1);
  }
  else{
    pos = open_gl_pos();
    fVertColor = myShadingRoutine(pos);
    prepare_open_gl_output();
  }
}

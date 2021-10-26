#version 450 core

in vec4 fPosition;
in vec4 fNormal;
in vec4 fVertColor;

uniform sampler2D ourTexture;

uniform vec3 CamPos;
uniform vec3 Color;
uniform vec3 AmbientColor;
uniform vec3 LightPos;

uniform int renderOp;

out vec4 fColor;

subroutine vec4 shadingRoutine(); 

layout(index=0) subroutine(shadingRoutine) vec4 no_shading() {
  return fVertColor;
}

layout(index=1) subroutine(shadingRoutine) vec4 gouraud() {
  return fVertColor;
}

layout(index=2) subroutine(shadingRoutine) vec4 phong() {
  vec4 norm = normalize(fNormal);
  vec4 lightDir = normalize(vec4(LightPos, 1.0f) - fPosition);
  float diff = max(dot(norm, lightDir), 0);
  vec4 ambient = 0.3f * vec4(AmbientColor, 1.0f);

  vec4 viewDir = normalize(vec4(CamPos, 1.0f) - fPosition);
  vec4 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

  float specularStrength = 0.5f;
  vec4 specular = specularStrength * spec * vec4(1.0f, 1.0f, 1.0f, 1.0f);
  vec4 diffuse = (diff * vec4(Color, 1.0f)) * (1.0f - specularStrength);

  return diffuse + specular + ambient;
}

layout(index=3) subroutine(shadingRoutine) vec4 flat_shade() {
  return vec4(Color, 1.0f);
}

subroutine uniform shadingRoutine myShadingRoutine;

void main() {
  if (renderOp == 1) {
    fColor = texture(ourTexture, fNormal.xy);
  } else {
    fColor = myShadingRoutine();
  }
}

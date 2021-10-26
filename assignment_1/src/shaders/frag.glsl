#version 450 core

in vec4 fPosition;
in vec4 fNormal;
in vec3 fCamPos;
in vec3 fVertColor;
in vec3 fAmbient;
in vec3 fLightPos;

out vec4 fColor;

void main() {
  vec4 norm = normalize(fNormal);
  vec4 lightDir = normalize(vec4(fLightPos, 1.0f) - fPosition);
  float diff = max(dot(norm, lightDir), 0);

  vec4 viewDir = normalize(vec4(fCamPos, 1.0f) - fPosition);
  vec4 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

  float specularStrength = 0.5f;
  vec4 specular = specularStrength * spec * vec4(1.0f, 1.0f, 1.0f, 1.0f);
  vec4 diffuse = (diff * vec4(fVertColor, 1.0f)) * (1.0f - specularStrength);
  vec4 ambient = vec4(fAmbient, 1.0f);

  fColor = diffuse + specular + ambient;
}

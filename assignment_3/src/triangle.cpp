#include "../include/triangle.hpp"

const float ERROR = 0.00f;

Triangle::Triangle(glm::vec3 vtxs[3], glm::vec3 normals[3]) {
  this->vtxs[0] = Vertex(glm::vec4(vtxs[0], 1.0f), glm::vec4(normals[0], 1.0f),
                         normals[0], 1.0f);
  this->vtxs[1] = Vertex(glm::vec4(vtxs[1], 1.0f), glm::vec4(normals[1], 1.0f),
                         normals[1], 1.0f);
  this->vtxs[2] = Vertex(glm::vec4(vtxs[2], 1.0f), glm::vec4(normals[2], 1.0f),
                         normals[2], 1.0f);

  // glm::vec4 red = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
  // this->vtxs[0] = Vertex(glm::vec4(vtxs[0], 1.0f), red, normals[0], 1.0f);
  // this->vtxs[1] = Vertex(glm::vec4(vtxs[1], 1.0f), red, normals[1], 1.0f);
  // this->vtxs[2] = Vertex(glm::vec4(vtxs[2], 1.0f), red, normals[2], 1.0f);
}

bool vecPosOutOfNormalizedSpace(glm::vec4 v) {
  v /= v.w;
  return v.x < (-1 - ERROR) || v.x > (1 + ERROR) || v.y < (-1 - ERROR) ||
         v.y > (1 + ERROR) || v.z < (-1 - ERROR) || v.z > (1 + ERROR);
}

bool Triangle::shouldFrustumCull() {
  for (int i = 0; i < 3; i++)
    if (!vecPosOutOfNormalizedSpace(this->vtxs[i].pos)) return false;

  return true;
};

bool Triangle::shouldPlaneCull(float near, float far) {
  for (int i = 0; i < 3; i++)
    if (this->vtxs[i].pos.w < near || this->vtxs[i].pos.w > far) return true;

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
    glm::vec4 screenV = screenSpaceVtx(this->vtxs[i].pos);
    glm::vec4 nextScreenV = screenSpaceVtx(this->vtxs[modAdd(i, 1, 3)].pos);
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

  vtxArray[x] = this->vtxs[0].pos.x;
  vtxArray[y] = this->vtxs[0].pos.y;
  vtxArray[z] = this->vtxs[0].pos.z;
  vtxArray[x + 3] = this->vtxs[1].pos.x;
  vtxArray[y + 3] = this->vtxs[1].pos.y;
  vtxArray[z + 3] = this->vtxs[1].pos.z;
  vtxArray[x + 6] = this->vtxs[2].pos.x;
  vtxArray[y + 6] = this->vtxs[2].pos.y;
  vtxArray[z + 6] = this->vtxs[2].pos.z;

  normalArray[x] = this->vtxs[0].normal.x;
  normalArray[y] = this->vtxs[0].normal.y;
  normalArray[z] = this->vtxs[0].normal.z;
  normalArray[x + 3] = this->vtxs[1].normal.x;
  normalArray[y + 3] = this->vtxs[1].normal.y;
  normalArray[z + 3] = this->vtxs[1].normal.z;
  normalArray[x + 6] = this->vtxs[2].normal.x;
  normalArray[y + 6] = this->vtxs[2].normal.y;
  normalArray[z + 6] = this->vtxs[2].normal.z;
};

void Triangle::applyMatrix(glm::mat4 mtx) {
  for (int i = 0; i < 3; i++) this->vtxs[i].pos = mtx * this->vtxs[i].pos;
}

void Triangle::normalize() {
  for (int i = 0; i < 3; i++) {
    this->vtxs[i].normal /= this->vtxs[i].pos.w;
    this->vtxs[i].color /= this->vtxs[i].pos.w;
    this->vtxs[i].w /= this->vtxs[i].pos.w;
    this->vtxs[i].pos /= this->vtxs[i].pos.w;
  }
}

void Triangle::print() {
  std::cout << glm::to_string(this->vtxs[0].pos) << " -- "
            << glm::to_string(this->vtxs[1].pos) << " -- "
            << glm::to_string(this->vtxs[2].pos) << std::endl;
}

bool sortvec(Vertex a, Vertex b) { return (a.pos.y < b.pos.y); }

void sortVertices(Vertex *a, Vertex *b, Vertex *c) {
  std::vector<Vertex> vertices;
  vertices.push_back(*a);
  vertices.push_back(*b);
  vertices.push_back(*c);

  std::sort(vertices.begin(), vertices.end(), sortvec);

  *a = vertices[0];
  *b = vertices[1];
  *c = vertices[2];
}

void putPointInColors(float *colors, float *zbuffer, int width, int height,
                      Vertex point) {
  int x = point.pos.x;
  int y = point.pos.y;

  if (x < 0 || y < 0 || x >= width || y >= height) return;

  point.pos.z /= point.w;

  if (zbuffer[y * width + x] < point.pos.z) return;

  point.color /= point.w;

  // if (point.color != glm::vec4(1, 0, 0, 1))
  //   std::cout << glm::to_string(point.color) << std::endl;

  colors[y * width * 4 + x * 4] = point.color.r;
  colors[y * width * 4 + x * 4 + 1] = point.color.g;
  colors[y * width * 4 + x * 4 + 2] = point.color.b;
  colors[y * width * 4 + x * 4 + 3] = point.color.a;

  zbuffer[y * width + x] = point.pos.z;

  return;
}

void Triangle::computeColors(ShaderMode shader, glm::vec4 vertColor,
                             glm::vec4 ambientColor, glm::vec4 camPos,
                             glm::vec4 lightPos, glm::mat4 Model) {
  for (int i = 0; i < 3; i++) {
    if (shader == Phong || shader == NoShading) {
      this->vtxs[i].color = vertColor;
    } else if (shader == Gourad || shader == Color) {
      glm::vec4 norm = glm::normalize(glm::transpose(glm::inverse(Model)) *
                                      glm::vec4(this->vtxs[i].normal, 1.0f));

      glm::vec4 lightDir = glm::normalize(lightPos - this->vtxs[i].pos);
      float diff = glm::max(glm::dot(norm, lightDir), 0.0f);

      glm::vec4 viewDir = glm::normalize(camPos - this->vtxs[i].pos);
      glm::vec4 reflectDir = glm::reflect(-lightDir, norm);
      float spec = glm::pow(glm::max(glm::dot(viewDir, reflectDir), 0.0f), 32);

      glm::vec4 ambient = 0.3f * ambientColor;
      if (shader == Gourad) {
        float specularStrength = 0.5f;
        glm::vec4 specular =
            specularStrength * spec * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        glm::vec4 diffuse = (diff * vertColor) * (1.0f - specularStrength);

        this->vtxs[i].color = diffuse + ambient + specular;
      } else {
        glm::vec4 diffuse = diff * vertColor;
        this->vtxs[i].color = diffuse + ambient;
      }
    }
  }
}

void interpolatePoints(float *colors, float *zbuffer, int width, int height,
                       ShaderMode shader, RasterMode raster, Vertex enter, Vertex exit,
                       glm::vec4 ambientColor, glm::vec4 lightPos,
                       glm::vec4 camPos, glm::mat4 Viewport, glm::mat4 MVP,
                       glm::mat4 Model) {
  if (exit.pos.x < enter.pos.x) {
    Vertex swap = exit;
    exit = enter;
    enter = swap;
  }

  // Phong definitions
  float specularStrength = 0.5f;
  glm::vec4 pos;
  glm::vec4 norm;
  float diff;
  glm::vec4 ambient;
  glm::vec4 viewDir;
  glm::vec4 lightDir;
  glm::vec4 reflectDir;
  float spec;
  glm::vec4 specular;
  glm::vec4 diffuse;
  ambient = 0.3f * ambientColor;
  glm::mat4 inversePosMat = Model * glm::inverse(Viewport * MVP);
  glm::mat4 transpostModel = glm::transpose(glm::inverse(Model));

  float x_dist = ceil(exit.pos.x) - floor(enter.pos.x);
  int y = enter.pos.y;
  float alpha;
  Vertex pixel;
  int x_step = 1;

  if (raster == Wireframe)
    x_step = ceil(x_dist);


  for (int x_incr = 0; x_incr <= ceil(x_dist); x_incr+=x_step) {
    alpha = ((float)x_incr) / x_dist;
    int x = floor(enter.pos.x) + x_incr;

    pixel = Vertex(
        glm::vec4(x, y, alpha * exit.pos.z + (1.0f - alpha) * enter.pos.z,
                  1.0f),
        alpha * exit.color + (1.0f - alpha) * enter.color,
        alpha * exit.normal + (1.0f - alpha) * enter.normal,
        alpha * exit.w + (1.0f - alpha) * enter.w);

    if (shader == Phong) {
      pixel.normal /= pixel.w;
      pixel.color /= pixel.w;
      float z = pixel.pos.z / pixel.w;
      pixel.w /= pixel.w;

      pos = glm::normalize(inversePosMat * pixel.pos);
      norm = glm::normalize(transpostModel * glm::vec4(pixel.normal, 1.0f));

      lightDir = glm::normalize(lightPos - pos);
      diff = glm::max(dot(norm, lightDir), 0.0f);

      viewDir = glm::normalize(camPos - pos);
      reflectDir = reflect(-lightDir, norm);
      spec = pow(glm::max(glm::dot(viewDir, reflectDir), 0.0f), 32);

      specular = specularStrength * spec * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
      diffuse = (diff * pixel.color) * (1.0f - specularStrength);

      pixel.color = diffuse + specular + ambient;
      pixel.pos.z = z;
    }

    putPointInColors(colors, zbuffer, width, height, pixel);
  }
}

void Triangle::rasterize(float *colors, float *zbuffer, int width, int height,
                         ShaderMode shader, RasterMode raster,
                         glm::vec4 ambientColor, glm::vec4 camPos,
                         glm::vec4 lightPos, glm::mat4 Model, glm::mat4 MVP,
                         glm::mat4 Viewport) {
  sortVertices(&this->vtxs[0], &this->vtxs[1], &this->vtxs[2]);

  glm::vec3 d1, d2, d3;

  d1 = this->vtxs[1].pos - this->vtxs[0].pos;
  d2 = this->vtxs[2].pos - this->vtxs[0].pos;
  d3 = this->vtxs[2].pos - this->vtxs[1].pos;

  Vertex p1Pair[2], p2Pair[2];
  float error = 1.0f;

  if (d2.y <= error) return;
  if (d1.y <= error) {
    p1Pair[0] = this->vtxs[0];
    p2Pair[0] = this->vtxs[1];

    p1Pair[1] = this->vtxs[2];
    p2Pair[1] = this->vtxs[2];

    if (raster != Point)
      interpolatePoints(colors, zbuffer, width, height, shader,raster,  this->vtxs[0],
                        this->vtxs[1], ambientColor, lightPos, camPos, Viewport,
                        MVP, Model);
  } else {
    p1Pair[0] = this->vtxs[0];
    p2Pair[0] = this->vtxs[0];

    p1Pair[1] = this->vtxs[1];
    p2Pair[1] = this->vtxs[2];

    if (d3.y <= error && raster != Point) {
      interpolatePoints(colors, zbuffer, width, height, shader,raster,  this->vtxs[1],
                        this->vtxs[2], ambientColor, lightPos, camPos, Viewport,
                        MVP, Model);
    }
  }

  Vertex enter, exit;
  float enterAlpha, exitAlpha;

  if (raster == Point) {
    putPointInColors(colors, zbuffer, width, height, p1Pair[0]);
    putPointInColors(colors, zbuffer, width, height, p1Pair[1]);
    putPointInColors(colors, zbuffer, width, height, p2Pair[0]);
    return;
  }

  bool switched = false;

  glm::vec3 dEnter = p1Pair[1].pos - p1Pair[0].pos;
  glm::vec3 dExit = p2Pair[1].pos - p2Pair[0].pos;

  int init_y = 0, end_y = floor(d1.y), max_y = ceil(d2.y);

  if (d1.y <= error || d3.y <= error) {
    end_y = ceil(d2.y);
  }

  for (int y_incr = init_y; y_incr <= max_y; y_incr++) {
    if (y_incr > d1.y && d1.y > error && d3.y > error && !switched) {
      init_y = ceil(d1.y);
      end_y = ceil(d2.y);
      p1Pair[0] = this->vtxs[1];
      p1Pair[1] = this->vtxs[2];
      dEnter = p1Pair[1].pos - p1Pair[0].pos;
      switched = true;
    }

    enterAlpha = ((float)y_incr - init_y) / (end_y - init_y);
    exitAlpha = ((float)y_incr) / ceil(d2.y);

    enter = Vertex(
        glm::vec4(p1Pair[0].pos.x + enterAlpha * dEnter.x,
                  this->vtxs[0].pos.y + y_incr,
                  p1Pair[0].pos.z + enterAlpha * dEnter.z, 1.0f),
        enterAlpha * p1Pair[1].color + (1.0f - enterAlpha) * p1Pair[0].color,
        enterAlpha * p1Pair[1].normal + (1.0f - enterAlpha) * p1Pair[0].normal,
        enterAlpha * p1Pair[1].w + (1.0f - enterAlpha) * p1Pair[0].w);

    exit = Vertex(
        glm::vec4(p2Pair[0].pos.x + exitAlpha * dExit.x,
                  this->vtxs[0].pos.y + y_incr,
                  p2Pair[0].pos.z + exitAlpha * dExit.z, 1.0f),
        exitAlpha * p2Pair[1].color + (1.0f - exitAlpha) * p2Pair[0].color,
        exitAlpha * p2Pair[1].normal + (1.0f - exitAlpha) * p2Pair[0].normal,
        exitAlpha * p2Pair[1].w + (1.0f - exitAlpha) * p2Pair[0].w);

    interpolatePoints(colors, zbuffer, width, height, shader, raster, enter, exit,
                      ambientColor, lightPos, camPos, Viewport, MVP, Model);
  }
}

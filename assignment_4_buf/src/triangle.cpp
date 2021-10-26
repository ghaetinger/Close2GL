#include "../include/triangle.hpp"
#include <glm/fwd.hpp>
#include <glm/gtx/string_cast.hpp>

const float ERROR = 0.0f;

Triangle::Triangle(glm::vec3 vtxs[3], glm::vec3 normals[3],
                   glm::vec2 texcoords[3]) {
  this->vtxs[0] =
      Vertex(glm::vec4(vtxs[0], 1.0f), glm::vec4(texcoords[0], 0, 1.0f),
             normals[0], 1.0f, texcoords[0]);
  this->vtxs[1] =
      Vertex(glm::vec4(vtxs[1], 1.0f), glm::vec4(texcoords[1], 0, 1.0f),
             normals[1], 1.0f, texcoords[1]);
  this->vtxs[2] =
      Vertex(glm::vec4(vtxs[2], 1.0f), glm::vec4(texcoords[2], 0, 1.0f),
             normals[2], 1.0f, texcoords[2]);
}

bool vecPosOutOfNormalizedSpace(glm::vec4 v) {
  v /= v.w;
  return v.x < (-1 - ERROR) || v.x > (1 + ERROR) || v.y < (-1 - ERROR) ||
         v.y > (1 + ERROR) || v.z < (-1 - ERROR) || v.z > (1 + ERROR);
}

bool Triangle::shouldFrustumCull() {
  for (int i = 0; i < 3; i++)
    if (!vecPosOutOfNormalizedSpace(this->vtxs[i].pos))
      return false;

  return true;
};

bool Triangle::shouldPlaneCull(float near, float far) {
  for (int i = 0; i < 3; i++)
    if (this->vtxs[i].pos.w < near || this->vtxs[i].pos.w > far)
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
  for (int i = 0; i < 3; i++)
    this->vtxs[i].pos = mtx * this->vtxs[i].pos;
}

void Triangle::normalize() {
  for (int i = 0; i < 3; i++) {
    this->vtxs[i].normal /= this->vtxs[i].pos.w;
    this->vtxs[i].texcoords /= this->vtxs[i].pos.w;
    this->vtxs[i].diff /= this->vtxs[i].pos.w;
    this->vtxs[i].specular /= this->vtxs[i].pos.w;
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

glm::vec3 NN(glm::vec2 coord, uint8_t *data, int width, int height) {
  int x = coord.x * width, y = (1.0f - coord.y) * height;
  int idx = round(y) * width * 4 + round(x) * 4;

  glm::vec3 color = glm::vec3((data[idx] / 256.0f), (data[idx + 1] / 256.0f),
                              (data[idx + 2] / 256.0f));

  return color;
}

glm::vec3 BiLinear(glm::vec2 coord, uint8_t *data, int width, int height) {
  float x = coord.x * width, y = (1.0f - coord.y) * height;

  glm::vec2 discrete_points[4] = {
      glm::vec2(glm::floor(x), glm::floor(y)),
      glm::vec2(glm::floor(x) + 1, glm::floor(y)),
      glm::vec2(glm::floor(x), glm::floor(y) + 1),
      glm::vec2(glm::floor(x) + 1, glm::floor(y) + 1),
  };

  glm::vec3 colors[4];
  int idx;

  for (int i = 0; i < 4; i++) {
    idx = discrete_points[i].y * width * 4 + discrete_points[i].x * 4;
    colors[i] = glm::vec3((data[idx] / 256.0f), (data[idx + 1] / 256.0f),
                          (data[idx + 2] / 256.0f));
  }

  glm::vec3 top_color =
      (x - glm::floor(x)) * colors[1] + (glm::floor(x) + 1.0f - x) * colors[0];
  glm::vec3 bottom_color =
      (x - glm::floor(x)) * colors[3] + (glm::floor(x) + 1.0f - x) * colors[2];

  return (y - glm::floor(y)) * bottom_color +
         (glm::floor(y) + 1.0f - y) * top_color;
}

glm::vec3 Mimap(glm::vec2 coord, uint8_t *data, int width, int height,
                uint8_t **levels, glm::vec2 delta) {

  float level;

  if (delta.x * width < 1.0f && delta.y * height < 1.0f)
    level = 0;
  else {
    level =
        (float)(log(glm::max((int)(delta.x * width), (int)(delta.y * height))) /
                log(2.0));
  }

  glm::vec3 high_color = BiLinear(coord, levels[(int)glm::floor(level)],
                                  width / pow(2.0f, glm::floor(level)),
                                  height / pow(2.0f, glm::floor(level)));

  glm::vec3 low_color = BiLinear(coord, levels[(int)glm::ceil(level)],
                                 width / pow(2.0f, glm::ceil(level)),
                                 height / pow(2.0f, glm::ceil(level)));

  return (level - glm::floor(level)) * low_color +
         (glm::floor(level) + 1.0f - level) * high_color;
}

void putPointInColors(float *colors, float *zbuffer, int width, int height,
                      Vertex point) {

  int x = point.pos.x;
  int y = point.pos.y;

  if (x < 0 || y < 0 || x >= width || y >= height)
    return;

  point.pos.z /= point.w;
  point.color /= point.w;

  if (zbuffer[y * width + x] < point.pos.z)
    return;

  colors[y * width * 4 + x * 4] = point.color.r;
  colors[y * width * 4 + x * 4 + 1] = point.color.g;
  colors[y * width * 4 + x * 4 + 2] = point.color.b;
  colors[y * width * 4 + x * 4 + 3] = point.color.a;

  zbuffer[y * width + x] = point.pos.z;

  return;
}

glm::vec4 getTexColor(Vertex point, TextureMode texmode, uint8_t *image,
                      int im_width, int im_height, uint8_t **levels,
                      glm::vec2 delta) {

  switch (texmode) {
  case NearestNeighbor:
    return glm::vec4(NN(point.texcoords, image, im_width, im_height), 1.0f);

  case Bilinear:
    return glm::vec4(BiLinear(point.texcoords, image, im_width, im_height),
                     1.0f);

  case MipMap:
    return glm::vec4(
        Mimap(point.texcoords, image, im_width, im_height, levels, delta),
        1.0f);
  }
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
      this->vtxs[i].diff = diff;

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
        this->vtxs[i].specular = specular;
      } else {
        glm::vec4 diffuse = diff * vertColor;
        this->vtxs[i].color = diffuse + ambient;
      }
    }
  }
}

void interpolatePoints(float *colors, float *zbuffer, int width, int height,
                       ShaderMode shader, RasterMode raster, Vertex enter,
                       Vertex exit, glm::vec4 ambientColor, glm::vec4 lightPos,
                       glm::vec4 camPos, glm::mat4 Viewport, glm::mat4 MVP,
                       glm::mat4 Model, bool useTex, TextureMode texmode,
                       uint8_t *image, int im_width, int im_height,
                       uint8_t **levels) {

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

  glm::vec2 delta = (exit.texcoords - enter.texcoords) / x_dist;

  if (raster == Wireframe)
    x_step = ceil(x_dist);

  for (int x_incr = 0; x_incr <= ceil(x_dist); x_incr += x_step) {
    alpha = ((float)x_incr) / x_dist;
    int x = floor(enter.pos.x) + x_incr;

    pixel = Vertex(glm::vec4(x, y,
                             alpha * exit.pos.z + (1.0f - alpha) * enter.pos.z,
                             1.0f),
                   alpha * exit.color + (1.0f - alpha) * enter.color,
                   alpha * exit.normal + (1.0f - alpha) * enter.normal,
                   alpha * exit.w + (1.0f - alpha) * enter.w,
                   alpha * exit.texcoords + (1.0f - alpha) * enter.texcoords);

    pixel.diff = alpha * exit.diff + (1.0f - alpha) * enter.diff;

    pixel.specular = alpha * exit.specular + (1.0f - alpha) * enter.specular;

    glm::vec2 pixel_delta = (exit.texcoords - enter.texcoords) / x_dist;
    pixel_delta /= pixel.w;

    if (shader == Phong) {
      pixel.normal /= pixel.w;
      pixel.texcoords /= pixel.w;
      pixel.color /= pixel.w;
      float z = pixel.pos.z / pixel.w;
      pixel.w /= pixel.w;

      if (useTex) {
        pixel.color = getTexColor(pixel, texmode, image, im_width, im_height,
                                  levels, pixel_delta);
      }

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

      putPointInColors(colors, zbuffer, width, height, pixel);
    } else {

      if (useTex) {
        pixel.specular /= pixel.w;
        pixel.diff /= pixel.w;
        pixel.texcoords /= pixel.w;
        pixel.normal /= pixel.w;
        pixel.w /= pixel.w;

        if (shader == Gourad) {
          glm::vec4 diffuse = pixel.diff *
                              getTexColor(pixel, texmode, image, im_width,
                                          im_height, levels, pixel_delta) *
                              0.5f;
          pixel.color = diffuse + ambient + pixel.specular;
        } else if (shader == Color) {

          glm::vec4 diffuse = pixel.diff *
                              getTexColor(pixel, texmode, image, im_width,
                                          im_height, levels, pixel_delta) *
                              0.5f;
          pixel.color = diffuse + ambient;
        } else {
          pixel.color = getTexColor(pixel, texmode, image, im_width, im_height,
                                    levels, pixel_delta);
        }
      }

      putPointInColors(colors, zbuffer, width, height, pixel);
    }
  }
}

void Triangle::rasterize(float *colors, float *zbuffer, int width, int height,
                         ShaderMode shader, RasterMode raster,
                         glm::vec4 ambientColor, glm::vec4 camPos,
                         glm::vec4 lightPos, glm::mat4 Model, glm::mat4 MVP,
                         glm::mat4 Viewport, bool useTex, TextureMode texmode,
                         uint8_t *image, int im_width, int im_height,
                         uint8_t **levels) {

  sortVertices(&this->vtxs[0], &this->vtxs[1], &this->vtxs[2]);

  glm::vec3 d1, d2, d3;

  d1 = this->vtxs[1].pos - this->vtxs[0].pos;
  d2 = this->vtxs[2].pos - this->vtxs[0].pos;
  d3 = this->vtxs[2].pos - this->vtxs[1].pos;

  Vertex p1Pair[2], p2Pair[2];
  float error = 1.0f;

  if (d2.y <= error)
    return;
  if (d1.y <= error) {
    p1Pair[0] = this->vtxs[0];
    p2Pair[0] = this->vtxs[1];

    p1Pair[1] = this->vtxs[2];
    p2Pair[1] = this->vtxs[2];

    if (raster != Point)
      interpolatePoints(colors, zbuffer, width, height, shader, raster,
                        this->vtxs[0], this->vtxs[1], ambientColor, lightPos,
                        camPos, Viewport, MVP, Model, useTex, texmode, image,
                        im_width, im_height, levels);
  } else {
    p1Pair[0] = this->vtxs[0];
    p2Pair[0] = this->vtxs[0];

    p1Pair[1] = this->vtxs[1];
    p2Pair[1] = this->vtxs[2];

    if (d3.y <= error && raster != Point) {
      interpolatePoints(colors, zbuffer, width, height, shader, raster,
                        this->vtxs[1], this->vtxs[2], ambientColor, lightPos,
                        camPos, Viewport, MVP, Model, useTex, texmode, image,
                        im_width, im_height, levels);
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
        enterAlpha * p1Pair[1].w + (1.0f - enterAlpha) * p1Pair[0].w,
        enterAlpha * p1Pair[1].texcoords +
            (1.0f - enterAlpha) * p1Pair[0].texcoords);

    enter.diff =
        enterAlpha * p1Pair[1].diff + (1.0f - enterAlpha) * p1Pair[0].diff;
    enter.specular = enterAlpha * p1Pair[1].specular +
                     (1.0f - enterAlpha) * p1Pair[0].specular;

    exit = Vertex(
        glm::vec4(p2Pair[0].pos.x + exitAlpha * dExit.x,
                  this->vtxs[0].pos.y + y_incr,
                  p2Pair[0].pos.z + exitAlpha * dExit.z, 1.0f),
        exitAlpha * p2Pair[1].color + (1.0f - exitAlpha) * p2Pair[0].color,
        exitAlpha * p2Pair[1].normal + (1.0f - exitAlpha) * p2Pair[0].normal,
        exitAlpha * p2Pair[1].w + (1.0f - exitAlpha) * p2Pair[0].w,
        exitAlpha * p2Pair[1].texcoords +
            (1.0f - exitAlpha) * p2Pair[0].texcoords);

    exit.diff =
        exitAlpha * p1Pair[1].diff + (1.0f - exitAlpha) * p1Pair[0].diff;
    exit.specular = exitAlpha * p1Pair[1].specular +
                    (1.0f - exitAlpha) * p1Pair[0].specular;

    interpolatePoints(colors, zbuffer, width, height, shader, raster, enter,
                      exit, ambientColor, lightPos, camPos, Viewport, MVP,
                      Model, useTex, texmode, image, im_width, im_height,
                      levels);
  }
}

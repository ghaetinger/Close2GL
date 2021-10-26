#include "../include/option_window.hpp"
#include <glm/trigonometric.hpp>
#include <gtkmm-3.0/gtkmm.h>
#include <gtkmm-3.0/gtkmm/application.h>
#include <gtkmm-3.0/gtkmm/checkbutton.h>
#include <iostream>

OptionWindow::OptionWindow(Options *options, bool hasTex) {
  this->options = options;
  this->setupWindowWidgets(hasTex);
  show_all();
}

OptionWindow::~OptionWindow() {}

void OptionWindow::setupWindowWidgets(bool hasTex) {
  // Init
  this->ccwButton = Gtk::RadioButton("Counter Clockwise");
  this->cwButton = Gtk::RadioButton("Clockwise");

  this->close2GLButton = Gtk::RadioButton("Close2GL");
  this->openGLButton = Gtk::RadioButton("OpenGL");

  this->wireframeButton = Gtk::RadioButton("Wireframe");
  this->normalButton = Gtk::RadioButton("Normal");
  this->pointButton = Gtk::RadioButton("Point");

  this->colorShadingButton = Gtk::RadioButton("Flat (Gouraud - Specular)");
  this->gouradButton = Gtk::RadioButton("Gouraud");
  this->phongButton = Gtk::RadioButton("Phong");
  this->noShadingButton = Gtk::RadioButton("No Shading");

  this->freeCamButton = Gtk::RadioButton("Free Camera");
  this->orbitCameraButton = Gtk::RadioButton("Orbit Camera");

  this->backfaceCullButton = Gtk::CheckButton("Enable Backface Culling");

  this->textureButton = Gtk::CheckButton("Use Texture");

  if (hasTex) {
    this->nearestButton = Gtk::RadioButton("Nearest Neighbor");
    this->bilinearButton = Gtk::RadioButton("Bilinear");
    this->mipmapButton = Gtk::RadioButton("MipMap");
  }

  this->resetCamButton = Gtk::Button("Reset Camera");

  this->vertexColor = Gtk::ColorButton();
  this->colorButtonLabel = Gtk::Label("Vertex Color");

  this->ambientColor = Gtk::ColorButton();
  this->ambientColorButtonLabel = Gtk::Label("Ambient Color");

  this->farPlaneScale = Gtk::Scale();
  this->farPlaneScaleLabel = Gtk::Label("Far PLane");
  this->nearPlaneScale = Gtk::Scale();
  this->nearPlaneScaleLabel = Gtk::Label("Near Plane");

  this->hfovScale = Gtk::Scale();
  this->hfovScaleLabel = Gtk::Label("HFOV (only on Close2GL)");
  this->vfovScale = Gtk::Scale();
  this->vfovScaleLabel = Gtk::Label("VFOV (OpenGL's FOV)");

  this->windBox = Gtk::Box();
  this->glBox = Gtk::Box();
  this->renderBox = Gtk::Box();
  this->shaderBox = Gtk::Box();
  this->camTypeBox = Gtk::Box();
  this->textureBox = Gtk::Box();

  this->mainBox = Gtk::Box(Gtk::ORIENTATION_VERTICAL);

  // Winding

  auto windingGroup = this->cwButton.get_group();

  this->cwButton.set_active(true);

  this->cwButton.signal_clicked().connect(
      sigc::mem_fun(*this, &OptionWindow::setCW));
  this->ccwButton.signal_clicked().connect(
      sigc::mem_fun(*this, &OptionWindow::setCCW));
  this->ccwButton.set_group(windingGroup);

  this->windBox.pack_start(this->ccwButton);
  this->windBox.pack_start(this->cwButton);

  // GL

  auto glGroup = this->openGLButton.get_group();

  this->openGLButton.set_active(true);

  this->close2GLButton.signal_clicked().connect(
      sigc::mem_fun(*this, &OptionWindow::setClose2GL));
  this->openGLButton.signal_clicked().connect(
      sigc::mem_fun(*this, &OptionWindow::setOpenGL));
  this->close2GLButton.set_group(glGroup);

  this->glBox.pack_start(this->close2GLButton);
  this->glBox.pack_start(this->openGLButton);

  // Raster type

  auto rasterGroup = normalButton.get_group();
  this->wireframeButton.set_group(rasterGroup);
  this->pointButton.set_group(rasterGroup);

  this->normalButton.signal_clicked().connect(
      sigc::mem_fun(*this, &OptionWindow::setNormal));
  this->wireframeButton.signal_clicked().connect(
      sigc::mem_fun(*this, &OptionWindow::setWireframe));
  this->pointButton.signal_clicked().connect(
      sigc::mem_fun(*this, &OptionWindow::setPoint));

  renderBox.pack_start(this->normalButton);
  renderBox.pack_start(this->wireframeButton);
  renderBox.pack_start(this->pointButton);

  // Camera Type

  auto cameraGroup = freeCamButton.get_group();

  this->freeCamButton.set_active(true);

  this->freeCamButton.signal_clicked().connect(
      sigc::mem_fun(*this, &OptionWindow::setFreeCamera));
  this->orbitCameraButton.signal_clicked().connect(
      sigc::mem_fun(*this, &OptionWindow::setOrbitCamera));
  this->orbitCameraButton.set_group(cameraGroup);

  this->camTypeBox.pack_start(this->freeCamButton);
  this->camTypeBox.pack_start(this->orbitCameraButton);

  // Backface Culling
  this->backfaceCullButton.set_active(true);
  this->backfaceCullButton.signal_toggled().connect(
      sigc::mem_fun(*this, &OptionWindow::setBackfaceCull));

  if (hasTex) {

    // Texture Use
    this->textureButton.set_active(false);
    this->textureButton.signal_toggled().connect(
        sigc::mem_fun(*this, &OptionWindow::setTextureUse));
    this->textureBox.pack_start(this->textureButton);

    // Texture Render

    auto texModeGroup = nearestButton.get_group();
    this->bilinearButton.set_group(texModeGroup);
    this->mipmapButton.set_group(texModeGroup);

    this->nearestButton.signal_clicked().connect(
        sigc::mem_fun(*this, &OptionWindow::setNN));
    this->bilinearButton.signal_clicked().connect(
        sigc::mem_fun(*this, &OptionWindow::setBilinear));
    this->mipmapButton.signal_clicked().connect(
        sigc::mem_fun(*this, &OptionWindow::setMipMap));

    textureBox.pack_start(this->nearestButton);
    textureBox.pack_start(this->bilinearButton);
    textureBox.pack_start(this->mipmapButton);

    this->mipmapButton.set_active(true);
  }

  // Shading

  auto shadingGroup = colorShadingButton.get_group();

  this->colorShadingButton.signal_clicked().connect(
      sigc::mem_fun(*this, &OptionWindow::setColorShading));
  this->gouradButton.signal_clicked().connect(
      sigc::mem_fun(*this, &OptionWindow::setGourad));
  this->phongButton.signal_clicked().connect(
      sigc::mem_fun(*this, &OptionWindow::setPhong));
  this->noShadingButton.signal_clicked().connect(
      sigc::mem_fun(*this, &OptionWindow::setNoShading));

  this->phongButton.set_group(shadingGroup);
  this->gouradButton.set_group(shadingGroup);
  this->noShadingButton.set_group(shadingGroup);

  this->phongButton.set_active(true);

  this->shaderBox.pack_start(this->colorShadingButton);
  this->shaderBox.pack_start(this->gouradButton);
  this->shaderBox.pack_start(this->phongButton);
  this->shaderBox.pack_start(this->noShadingButton);

  // Reset Camera

  this->resetCamButton.signal_clicked().connect(
      sigc::mem_fun(*this, &OptionWindow::resetCamera));

  // Color

  Gdk::RGBA vtxclr;
  vtxclr.set_red(this->options->getColor().r);
  vtxclr.set_green(this->options->getColor().g);
  vtxclr.set_blue(this->options->getColor().b);
  vtxclr.set_alpha(1.0);
  vertexColor.set_rgba(vtxclr);

  Gdk::RGBA ambclr;
  ambclr.set_red(this->options->getAmbient().r);
  ambclr.set_green(this->options->getAmbient().g);
  ambclr.set_blue(this->options->getAmbient().b);
  ambclr.set_alpha(1.0);
  ambientColor.set_rgba(ambclr);

  this->vertexColor.signal_color_set().connect(
      sigc::mem_fun(*this, &OptionWindow::setColor));
  this->ambientColor.signal_color_set().connect(
      sigc::mem_fun(*this, &OptionWindow::setAmbientColor));

  // Planes

  farPlaneScale.set_range(options->getNear() + 0.1f, 50);
  farPlaneScale.set_value(options->getFar());
  this->farPlaneScale.signal_value_changed().connect(
      sigc::mem_fun(*this, &OptionWindow::setFarPlane));

  nearPlaneScale.set_range(0.1f, options->getFar() - 0.1f);
  nearPlaneScale.set_value(options->getNear());
  this->nearPlaneScale.signal_value_changed().connect(
      sigc::mem_fun(*this, &OptionWindow::setNearPlane));

  // FOV

  hfovScale.set_range(5, 150);
  hfovScale.set_value(60);
  this->hfovScale.signal_value_changed().connect(
      sigc::mem_fun(*this, &OptionWindow::setHfov));

  vfovScale.set_range(5, 150);
  vfovScale.set_value(60);
  this->vfovScale.signal_value_changed().connect(
      sigc::mem_fun(*this, &OptionWindow::setVfov));

  // Main Box

  this->mainBox.pack_start(this->windBox);
  this->mainBox.pack_start(this->glBox);
  this->mainBox.pack_start(this->shaderBox);
  this->mainBox.pack_start(this->renderBox);
  this->mainBox.pack_start(this->camTypeBox);

  if (hasTex)
    this->mainBox.pack_start(this->textureBox);

  this->mainBox.pack_start(this->backfaceCullButton);
  this->mainBox.pack_start(this->resetCamButton);
  this->mainBox.pack_start(this->vertexColor);
  this->mainBox.pack_start(this->colorButtonLabel);
  this->mainBox.pack_start(this->ambientColor);
  this->mainBox.pack_start(this->ambientColorButtonLabel);
  this->mainBox.pack_start(this->farPlaneScale);
  this->mainBox.pack_start(this->farPlaneScaleLabel);
  this->mainBox.pack_start(this->nearPlaneScale);
  this->mainBox.pack_start(this->nearPlaneScaleLabel);
  this->mainBox.pack_start(this->hfovScale);
  this->mainBox.pack_start(this->hfovScaleLabel);
  this->mainBox.pack_start(this->vfovScale);
  this->mainBox.pack_start(this->vfovScaleLabel);

  add(this->mainBox);

  setCW();
  setNormal();
  setPhong();
  setFreeCamera();
  setOpenGL();
  setMipMap();
}

void OptionWindow::setCW() { this->options->setOrder(CW); }
void OptionWindow::setCCW() { this->options->setOrder(CCW); }

void OptionWindow::setClose2GL() {
  this->options->setUsingClose2GL(true);
  this->options->setShouldSwitchRenderMode(true);
}

void OptionWindow::setOpenGL() {
  this->options->setUsingClose2GL(false);
  this->options->setShouldSwitchRenderMode(true);
}

void OptionWindow::setNormal() { this->options->setRasterMode(Solid); };
void OptionWindow::setWireframe() { this->options->setRasterMode(Wireframe); };
void OptionWindow::setPoint() { this->options->setRasterMode(Point); };

void OptionWindow::setColorShading() { this->options->setShaderMode(Color); }
void OptionWindow::setGourad() { this->options->setShaderMode(Gourad); }
void OptionWindow::setPhong() { this->options->setShaderMode(Phong); }
void OptionWindow::setNoShading() { this->options->setShaderMode(NoShading); }

void OptionWindow::setFreeCamera() { this->options->setCamType(Free); };
void OptionWindow::setOrbitCamera() { this->options->setCamType(Around); };

// Race condition!!
void OptionWindow::resetCamera() { this->options->setShouldResetCam(true); }

void OptionWindow::setColor() {
  Gdk::RGBA selectedColor = vertexColor.get_rgba();
  this->options->setColor(glm::vec3(selectedColor.get_red(),
                                    selectedColor.get_green(),
                                    selectedColor.get_blue()));
};

void OptionWindow::setAmbientColor() {
  Gdk::RGBA selectedColor = ambientColor.get_rgba();
  this->options->setAmbient(glm::vec3(selectedColor.get_red(),
                                      selectedColor.get_green(),
                                      selectedColor.get_blue()));
};

void OptionWindow::setFarPlane() {
  options->setFar(this->farPlaneScale.get_value());
  this->nearPlaneScale.set_range(0.1f, options->getFar() - 0.1f);
}

void OptionWindow::setNearPlane() {
  options->setNear(this->nearPlaneScale.get_value());
  this->farPlaneScale.set_range(options->getNear() + 0.1f, 50);
}

void OptionWindow::setHfov() {
  options->setHfov(glm::radians(this->hfovScale.get_value()));
}

void OptionWindow::setVfov() {
  options->setVfov(glm::radians(this->vfovScale.get_value()));
}

void OptionWindow::setBackfaceCull() {
  options->setShouldBackfaceCull(this->backfaceCullButton.get_active());
}

void OptionWindow::setTextureUse() {
  options->setShouldUseTexture(this->textureButton.get_active());
}

void OptionWindow::setNN() { options->setTextureMode(NearestNeighbor); }
void OptionWindow::setBilinear() { options->setTextureMode(Bilinear); }
void OptionWindow::setMipMap() { options->setTextureMode(MipMap); }

#ifndef GTK_OPTION_WINDOW_H
#define GTK_OPTION_WINDOW_H

#include "../include/options.hpp"

// GTK
#include <gtkmm-3.0/gtkmm/box.h>
#include <gtkmm-3.0/gtkmm/button.h>
#include <gtkmm-3.0/gtkmm/checkbutton.h>
#include <gtkmm-3.0/gtkmm/colorbutton.h>
#include <gtkmm-3.0/gtkmm/label.h>
#include <gtkmm-3.0/gtkmm/radiobutton.h>
#include <gtkmm-3.0/gtkmm/scale.h>
#include <gtkmm-3.0/gtkmm/window.h>

// // GLM
// #include <glm/fwd.hpp>
// #include <glm/geometric.hpp>
// #include <glm/glm.hpp>
// #include <glm/gtx/string_cast.hpp>

// // OpenGL
// #include <GL/glew.h>
// #include <GLFW/glfw3.h>

class OptionWindow : public Gtk::Window {

public:
  OptionWindow(Options *options);
  virtual ~OptionWindow();

protected:
  // Option Handler
  Options *options;

  // Setup Functions
  void setupWindowWidgets();

  // Window Options
  Gtk::RadioButton ccwButton, cwButton;
  void setCW();
  void setCCW();

  // GL lib
  Gtk::RadioButton close2GLButton, openGLButton;
  void setClose2GL();
  void setOpenGL();

  Gtk::RadioButton wireframeButton, normalButton, pointButton;
  void setNormal();
  void setWireframe();
  void setPoint();

  Gtk::RadioButton colorShadingButton, gouradButton, phongButton,
      noShadingButton;
  void setColorShading();
  void setGourad();
  void setPhong();
  void setNoShading();

  Gtk::RadioButton freeCamButton, orbitCameraButton;
  void setFreeCamera();
  void setOrbitCamera();

  Gtk::CheckButton backfaceCullButton;
  void setBackfaceCull();

  Gtk::ColorButton vertexColor;
  Gtk::Label colorButtonLabel;
  Gtk::ColorButton ambientColor;
  Gtk::Label ambientColorButtonLabel;
  void setColor();
  void setAmbientColor();

  Gtk::Button resetCamButton;
  void resetCamera();

  Gtk::Scale farPlaneScale;
  Gtk::Label farPlaneScaleLabel;
  Gtk::Scale nearPlaneScale;
  Gtk::Label nearPlaneScaleLabel;
  void setFarPlane();
  void setNearPlane();

  Gtk::Scale hfovScale;
  Gtk::Label hfovScaleLabel;
  Gtk::Scale vfovScale;
  Gtk::Label vfovScaleLabel;
  void setHfov();
  void setVfov();

  Gtk::Box mainBox, windBox, glBox, shaderBox, renderBox, camTypeBox;
};

#endif

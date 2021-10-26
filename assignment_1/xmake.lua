target("assignment1")
  set_kind("binary")
  set_targetdir("./")
  set_objectdir("bin")
  add_includedirs("include")
  add_cxflags("-std=c++11")
  add_links("pthread", "X11", "glfw", "GLEW", "GL", "freeimage")
  add_files("src/*.cpp")

# this is the build file for project 
# it is autogenerated by the xmake build system.
# do not edit by hand.

GC=/usr/bin/go
CU=/opt/cuda/bin/nvcc
FC=/usr/bin/gfortran
MXX=/usr/bin/gcc
AS=/usr/bin/gcc
MM=/usr/bin/gcc
RC=/usr/bin/rustc
CC=/usr/bin/gcc
CXX=/usr/bin/gcc

CULD=/opt/cuda/bin/nvcc
RCSH=/usr/bin/rustc
FCSH=/usr/bin/gfortran
SH=/usr/bin/g++
RCLD=/usr/bin/rustc
FCLD=/usr/bin/gfortran
GCLD=/usr/bin/go
LD=/usr/bin/g++
RCAR=/usr/bin/rustc
AR=/usr/bin/ar
GCAR=/usr/bin/go

assignment2_CXXFLAGS=-m64 -Iinclude -std=c++11 -I/usr/include/gtkmm-3.0 -I/usr/lib/gtkmm-3.0/include -I/usr/include/giomm-2.4 -I/usr/lib/giomm-2.4/include -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/libmount -I/usr/include/blkid -I/usr/include/glibmm-2.4 -I/usr/lib/glibmm-2.4/include -I/usr/include/sigc++-2.0 -I/usr/lib/sigc++-2.0/include -I/usr/include/gtk-3.0 -I/usr/include/pango-1.0 -I/usr/include/harfbuzz -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/fribidi -I/usr/include/cairo -I/usr/include/lzo -I/usr/include/pixman-1 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/gio-unix-2.0 -I/usr/include/cloudproviders -I/usr/include/atk-1.0 -I/usr/include/at-spi2-atk/2.0 -I/usr/include/dbus-1.0 -I/usr/lib/dbus-1.0/include -I/usr/include/at-spi-2.0 -I/usr/include/cairomm-1.0 -I/usr/lib/cairomm-1.0/include -I/usr/include/pangomm-1.4 -I/usr/lib/pangomm-1.4/include -I/usr/include/atkmm-1.6 -I/usr/lib/atkmm-1.6/include -I/usr/include/gtk-3.0/unix-print -I/usr/include/gdkmm-3.0 -I/usr/lib/gdkmm-3.0/include -pthread
assignment2_LDFLAGS=-m64 -lpthread -lX11 -lglfw -lGLEW -lGL -lfreeimage -lglibmm-2.4 -lgobject-2.0 -lglib-2.0 -lsigc-2.0 -lgtkmm-3.0 -latkmm-1.6 -lgdkmm-3.0 -lgiomm-2.4 -lgtk-3 -lgdk-3 -lz -latk-1.0 -lcairo-gobject -lgio-2.0 -lpangomm-1.4 -lcairomm-1.0 -lpangocairo-1.0 -lpango-1.0 -lharfbuzz -lcairo -lgdk_pixbuf-2.0

default:  assignment2

all:  assignment2

.PHONY: default all  assignment2

assignment2: bin/assignment2/linux/x86_64/release/src/matrices.cpp.o bin/assignment2/linux/x86_64/release/src/options.cpp.o bin/assignment2/linux/x86_64/release/src/triangle.cpp.o bin/assignment2/linux/x86_64/release/src/option_window.cpp.o bin/assignment2/linux/x86_64/release/src/camera.cpp.o bin/assignment2/linux/x86_64/release/src/ShaderCompiler.cpp.o bin/assignment2/linux/x86_64/release/src/main.cpp.o bin/assignment2/linux/x86_64/release/src/LoadShaders.cpp.o
	@echo linking.release assignment2
	@mkdir -p .
	@$(LD) -o ./assignment2 bin/assignment2/linux/x86_64/release/src/matrices.cpp.o bin/assignment2/linux/x86_64/release/src/options.cpp.o bin/assignment2/linux/x86_64/release/src/triangle.cpp.o bin/assignment2/linux/x86_64/release/src/option_window.cpp.o bin/assignment2/linux/x86_64/release/src/camera.cpp.o bin/assignment2/linux/x86_64/release/src/ShaderCompiler.cpp.o bin/assignment2/linux/x86_64/release/src/main.cpp.o bin/assignment2/linux/x86_64/release/src/LoadShaders.cpp.o $(assignment2_LDFLAGS) > build/.build.log 2>&1

bin/assignment2/linux/x86_64/release/src/matrices.cpp.o: src/matrices.cpp
	@echo compiling.release src/matrices.cpp
	@mkdir -p bin/assignment2/linux/x86_64/release/src
	@$(CXX) -c $(assignment2_CXXFLAGS) -o bin/assignment2/linux/x86_64/release/src/matrices.cpp.o src/matrices.cpp > build/.build.log 2>&1

bin/assignment2/linux/x86_64/release/src/options.cpp.o: src/options.cpp
	@echo compiling.release src/options.cpp
	@mkdir -p bin/assignment2/linux/x86_64/release/src
	@$(CXX) -c $(assignment2_CXXFLAGS) -o bin/assignment2/linux/x86_64/release/src/options.cpp.o src/options.cpp > build/.build.log 2>&1

bin/assignment2/linux/x86_64/release/src/triangle.cpp.o: src/triangle.cpp
	@echo compiling.release src/triangle.cpp
	@mkdir -p bin/assignment2/linux/x86_64/release/src
	@$(CXX) -c $(assignment2_CXXFLAGS) -o bin/assignment2/linux/x86_64/release/src/triangle.cpp.o src/triangle.cpp > build/.build.log 2>&1

bin/assignment2/linux/x86_64/release/src/option_window.cpp.o: src/option_window.cpp
	@echo compiling.release src/option_window.cpp
	@mkdir -p bin/assignment2/linux/x86_64/release/src
	@$(CXX) -c $(assignment2_CXXFLAGS) -o bin/assignment2/linux/x86_64/release/src/option_window.cpp.o src/option_window.cpp > build/.build.log 2>&1

bin/assignment2/linux/x86_64/release/src/camera.cpp.o: src/camera.cpp
	@echo compiling.release src/camera.cpp
	@mkdir -p bin/assignment2/linux/x86_64/release/src
	@$(CXX) -c $(assignment2_CXXFLAGS) -o bin/assignment2/linux/x86_64/release/src/camera.cpp.o src/camera.cpp > build/.build.log 2>&1

bin/assignment2/linux/x86_64/release/src/ShaderCompiler.cpp.o: src/ShaderCompiler.cpp
	@echo compiling.release src/ShaderCompiler.cpp
	@mkdir -p bin/assignment2/linux/x86_64/release/src
	@$(CXX) -c $(assignment2_CXXFLAGS) -o bin/assignment2/linux/x86_64/release/src/ShaderCompiler.cpp.o src/ShaderCompiler.cpp > build/.build.log 2>&1

bin/assignment2/linux/x86_64/release/src/main.cpp.o: src/main.cpp
	@echo compiling.release src/main.cpp
	@mkdir -p bin/assignment2/linux/x86_64/release/src
	@$(CXX) -c $(assignment2_CXXFLAGS) -o bin/assignment2/linux/x86_64/release/src/main.cpp.o src/main.cpp > build/.build.log 2>&1

bin/assignment2/linux/x86_64/release/src/LoadShaders.cpp.o: src/LoadShaders.cpp
	@echo compiling.release src/LoadShaders.cpp
	@mkdir -p bin/assignment2/linux/x86_64/release/src
	@$(CXX) -c $(assignment2_CXXFLAGS) -o bin/assignment2/linux/x86_64/release/src/LoadShaders.cpp.o src/LoadShaders.cpp > build/.build.log 2>&1

clean:  clean_assignment2

clean_assignment2: 
	@rm -rf ./assignment2
	@rm -rf ./assignment2.sym
	@rm -rf bin/assignment2/linux/x86_64/release/src/matrices.cpp.o
	@rm -rf bin/assignment2/linux/x86_64/release/src/options.cpp.o
	@rm -rf bin/assignment2/linux/x86_64/release/src/triangle.cpp.o
	@rm -rf bin/assignment2/linux/x86_64/release/src/option_window.cpp.o
	@rm -rf bin/assignment2/linux/x86_64/release/src/camera.cpp.o
	@rm -rf bin/assignment2/linux/x86_64/release/src/ShaderCompiler.cpp.o
	@rm -rf bin/assignment2/linux/x86_64/release/src/main.cpp.o
	@rm -rf bin/assignment2/linux/x86_64/release/src/LoadShaders.cpp.o


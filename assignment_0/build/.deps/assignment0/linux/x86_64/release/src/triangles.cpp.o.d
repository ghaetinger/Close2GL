{
    values = {
        "/usr/bin/gcc",
        {
            "-m64",
            "-Iinclude",
            "-std=c++11"
        }
    },
    depfiles_gcc = "bin/assignment0/linux/x86_64/release/src/triangles.cpp.o:  src/triangles.cpp src/../include/LoadShaders.hpp\
",
    files = {
        "src/triangles.cpp"
    }
}
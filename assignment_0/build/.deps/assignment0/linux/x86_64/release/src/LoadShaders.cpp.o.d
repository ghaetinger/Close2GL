{
    depfiles_gcc = "bin/assignment0/linux/x86_64/release/src/LoadShaders.cpp.o:  src/LoadShaders.cpp src/../include/LoadShaders.hpp\
",
    files = {
        "src/LoadShaders.cpp"
    },
    values = {
        "/usr/bin/gcc",
        {
            "-m64",
            "-Iinclude",
            "-std=c++11"
        }
    }
}
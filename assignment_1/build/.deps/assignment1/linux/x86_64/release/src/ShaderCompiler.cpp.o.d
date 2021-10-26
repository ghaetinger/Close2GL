{
    depfiles_gcc = "bin/assignment1/linux/x86_64/release/src/ShaderCompiler.cpp.o:  src/ShaderCompiler.cpp src/../include/ShaderCompiler.hpp\
",
    files = {
        "src/ShaderCompiler.cpp"
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
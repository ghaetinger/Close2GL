{
    depfiles_gcc = "bin/assignment1/linux/x86_64/release/src/main.cpp.o: src/main.cpp  src/../include/LoadShaders.hpp src/../include/ShaderCompiler.hpp\
",
    files = {
        "src/main.cpp"
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
cmake_minimum_required(VERSION 3.1.3)
project(VFV)

#Common compiler flags
set(COMMON_COMPILE_OPTIONS -Wall -Wextra -pedantic -std=c++0x -g)
# set(COMMON_COMPILE_OPTIONS -Wall -Wextra -pedantic -std=c++0x -O3)

set(
    SRCFILES
    "src/Plotter/FrameBuffer.cpp"
    "src/Plotter/lodepng.cpp"
    "src/Plotter/Renderer.cpp"
    "src/Plotter/VectorFieldPlot.cpp"
    "src/Interface/AppContext.cpp"
    "src/Interface/Main.cpp"
    "src/Interface/ScalarFunction.cpp"
    "src/Interface/Tokenizer.cpp"
)

add_executable(visualizer ${SRCFILES})
set_target_properties(visualizer PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
target_compile_options(visualizer PRIVATE ${COMMON_COMPILE_OPTIONS})
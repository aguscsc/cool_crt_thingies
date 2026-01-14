cmake_minimum_required(VERSION 3.10)

project(oscilloscope)
set(OpenGL_GL_PREFERENCE GLVND)
# 1. FIX: Split the find_package calls.
# We add "REQUIRED" so CMake stops immediately if a tool is missing.
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

# 2. Define the executable
# CRITICAL: Check your path. Is main.cpp in root or inside src/?
# If it is inside the src folder, change this to "src/main.cpp"
add_executable(oscilloscope src/main.cpp) 

# 3. FIX: Link the libraries
# Without this, your app doesn't know where the SDL/OpenGL code actually lives.
target_link_libraries(oscilloscope PRIVATE
    ${SDL2_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${OPENGL_LIBRARIES}
)

# 4. FIX: Include directories
# Without this, your #include <SDL2/SDL.h> lines might fail.
target_include_directories(oscilloscope PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
)

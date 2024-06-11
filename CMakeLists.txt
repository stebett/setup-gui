cmake_minimum_required(VERSION 3.28)
project(setup_gui)

set(CMAKE_CXX_STANDARD 20)



find_package(raylib REQUIRED)
#find_package(LDtkLoader 1.5)
set(EXEC setup-control)
set(rlImGui_ROOT "vendors/rlImGui")
set(imgui_ROOT "vendors/imgui")
set(TOML_ROOT "vendors/toml++/include")
set(INCLUDE "includes/")

include_directories(${rlImGui_ROOT})
include_directories(${imgui_ROOT})
include_directories(${TOML_ROOT})
include_directories(${INCLUDE})

file(GLOB rlImGui_SOURCES ${rlImGui_ROOT}/*.cpp)
file(GLOB imgui_SOURCES ${imgui_ROOT}/*.cpp)
file(GLOB_RECURSE INCLUDE_SOURCES ${INCLUDE}/*.cpp)

set(SOURCES
        ${imgui_SOURCES}
        ${rlImGui_SOURCES}
        ${INCLUDE_SOURCES}
)

add_executable(${EXEC} main.cpp ${SOURCES})
target_link_libraries(${EXEC} PRIVATE raylib)

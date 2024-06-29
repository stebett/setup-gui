cmake_minimum_required(VERSION 3.28)
project(setup_gui)

set(CMAKE_CXX_STANDARD 20)
find_package(tomlplusplus)
find_package(Boost)
find_package(imgui)
find_package(spdlog)

# Dependencies
set(RAYLIB_VERSION 5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
            raylib
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif ()
endif ()


set(EXEC setup-control)
set(rlImGui_ROOT "vendors/rlImGui")
set(INCLUDE "includes/")

include_directories(${rlImGui_ROOT})
include_directories(${INCLUDE})

file(GLOB rlImGui_SOURCES ${rlImGui_ROOT}/*.cpp)
file(GLOB_RECURSE INCLUDE_SOURCES ${INCLUDE}/*.cpp)

set(SOURCES
        ${rlImGui_SOURCES}
        ${INCLUDE_SOURCES}
)
add_library(Modules)
target_sources(Modules
        PUBLIC
        FILE_SET CXX_MODULES FILES
        includes/compress_videos.ixx
        includes/session_control.ixx
        includes/timer.ixx)

add_executable(${EXEC} main.cpp ${SOURCES})
target_link_libraries(${EXEC}
        tomlplusplus::tomlplusplus
        boost::boost
        imgui::imgui
        raylib
        spdlog::spdlog
        Modules
)

if (MINGW)
    target_link_libraries(${EXEC} ws2_32)
endif ()

target_link_libraries(Modules
        spdlog::spdlog
        tomlplusplus::tomlplusplus
)

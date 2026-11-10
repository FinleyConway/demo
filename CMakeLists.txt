cmake_minimum_required(VERSION 3.22...3.30)
project(demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF) # hush clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# build flags
option(ENABLE_DEBUGGING "Enable logging and asserting" ON)

# set build dir
set(BINARY_DIR "demo")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/${BINARY_DIR}"
)

if(UNIX AND NOT APPLE)
    set(GLFW_BUILD_WAYLAND ON)
    set(GLFW_BUILD_X11 OFF)
endif()

add_subdirectory(vendors/raylib)
add_subdirectory(vendors/entt)
add_subdirectory(vendors/json)

add_executable(demo
    src/main.cpp
)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if (ENABLE_DEBUGGING)
    target_compile_definitions(demo PRIVATE ENABLE_DEBUGGING)
endif()

target_include_directories(demo PUBLIC
    src # provide explicit cleaner headers
)

target_link_libraries(demo PUBLIC
    raylib
    EnTT::EnTT
    nlohmann_json::nlohmann_json
)

target_precompile_headers(${PROJECT_NAME} PRIVATE
    src/pch.hpp
)
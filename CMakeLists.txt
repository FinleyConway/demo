cmake_minimum_required(VERSION 3.22...3.30)
project(demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF) # hush clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set build dir
set(BINARY_DIR "demo")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/${BINARY_DIR}"
)

add_subdirectory(vendors/raylib)
add_subdirectory(vendors/entt)

add_executable(demo
    src/main.cpp
)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

target_include_directories(demo PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src # provide explicit cleaner headers
)

target_link_libraries(demo PUBLIC
    raylib
    EnTT::EnTT
)
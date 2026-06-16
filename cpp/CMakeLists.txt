cmake_minimum_required(VERSION 3.16)

project(CppApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# cxxopts
FetchContent_Declare(
    cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
    GIT_TAG v3.2.0
)

FetchContent_MakeAvailable(cxxopts)

# Matplot++
FetchContent_Declare(
    matplot
    GIT_REPOSITORY https://github.com/alandefreitas/matplotplusplus.git
    GIT_TAG master
)

FetchContent_MakeAvailable(matplot)

# Найти все .cpp-файлы внутри src
file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(app
    ${APP_SOURCES}
)

target_include_directories(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(app PRIVATE
    PROJECT_ROOT="${CMAKE_SOURCE_DIR}"
)

target_link_libraries(app PRIVATE
    cxxopts::cxxopts
    Matplot++::matplot
)

target_compile_options(app PRIVATE
    -Wall
    -Wextra
    -Wpedantic

    $<$<CONFIG:Debug>:-g;-O0>
    $<$<CONFIG:Release>:-O3>
)
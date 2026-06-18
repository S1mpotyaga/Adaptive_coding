cmake_minimum_required(VERSION 3.16)

project(CppApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
    cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
    GIT_TAG v3.2.0
)

FetchContent_MakeAvailable(cxxopts)

FetchContent_Declare(
    matplot
    GIT_REPOSITORY https://github.com/alandefreitas/matplotplusplus.git
    GIT_TAG v1.2.2
)

FetchContent_MakeAvailable(matplot)

find_package(
    OpenCV REQUIRED
    COMPONENTS
        core
        imgcodecs
        imgproc
)

message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV include directories: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")

set(
    OpenCV_DIR
    "/usr/lib/x86_64-linux-gnu/cmake/opencv4"
)

find_package(
    OpenCV REQUIRED
    COMPONENTS
        core
        imgcodecs
        imgproc
)

file(
    GLOB_RECURSE APP_SOURCES
    CONFIGURE_D
    
    EPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(app ${APP_SOURCES})

target_include_directories(
    app
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${OpenCV_INCLUDE_DIRS}
)

target_compile_definitions(
    app
    PRIVATE
        PROJECT_ROOT="${PROJECT_SOURCE_DIR}"
)

target_link_libraries(
    app
    PRIVATE
        cxxopts::cxxopts
        Matplot++::matplot
        ${OpenCV_LIBS}
)

if(MSVC)
    target_compile_options(
        app
        PRIVATE
            /W4
            /permissive-
            $<$<CONFIG:Debug>:/Od;/Zi>
            $<$<CONFIG:Release>:/O2>
    )
else()
    target_compile_options(
        app
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            $<$<CONFIG:Debug>:-g>
            $<$<CONFIG:Debug>:-O0>
            $<$<CONFIG:Release>:-O3>
    )
endif()
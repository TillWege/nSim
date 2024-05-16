cmake_minimum_required(VERSION 3.27)
project(nSim LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

# Adding Raylib
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(raylib)

file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/external/imgui/*.h" "${CMAKE_CURRENT_LIST_DIR}/external/imgui/*.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/external/rlImGui/*.h" "${CMAKE_CURRENT_LIST_DIR}/external/rlImGui/*.cpp"
)

set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/src/")

# Declaring our executable
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)  # Enable C++17 features

include_directories("${CMAKE_CURRENT_LIST_DIR}/external/imgui"
                    "${CMAKE_CURRENT_LIST_DIR}/external/imgui/backends"
                    "${CMAKE_CURRENT_LIST_DIR}/external/rlImGui")

# Setting ASSETS_PATH
#target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/") # Set the asset path macro to the absolute path on the dev machine
target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="./assets") # Set the asset path macro in release mode to a relative path that assumes the assets folder is in the same directory as the game executable
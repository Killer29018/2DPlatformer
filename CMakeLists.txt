cmake_minimum_required (VERSION 3.27)

# Setup Project
project (Platformer)

find_package(OpenGL REQUIRED)

# Set C++ Standard
set (CMAKE_CXX_STANDARD 20)

# Change the output directory
set (binaryDir "/bin")
set (outputDirectory "${PROJECT_SOURCE_DIR}${binaryDir}")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    message ("Debug Build")
    set (outputDirectory "${outputDirectory}/debug")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
elseif (CMAKE_BUILD_TYPE MATCHES Release)
    message ("Release Build")
    set (outputDirectory "${outputDirectory}/release")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif ()

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${outputDirectory})

# Setup build flags
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -Wall -Wextra -Wpedantic \
-Wno-unused-parameter -Wno-unknown-pragmas -Wno-unused-variable")

# Add all files
file (GLOB_RECURSE projectFiles CONFIGURE_DEPENDS "src/*.cpp")
add_executable(${PROJECT_NAME} ${projectFiles})

add_subdirectory(vendor)

target_include_directories(${PROJECT_NAME} PUBLIC
    vendor/glad/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${linker} PUBLIC
    glad
    OpenGL::GL
    glm::glm
    glfw
    Imgui
    STB
    jsoncpp
)

set (InputRes "${PROJECT_SOURCE_DIR}/res")
set (OutputRes "${outputDirectory}/res")

add_custom_target(Resources ALL COMMENT "Copying Resources")


add_custom_command(
    TARGET Resources POST_BUILD
    COMMAND
    cmake -E make_directory
    "${OutputRes}/shaders/" "${OutputRes}/fonts/" "${OutputRes}/textures/" "${outputDirectory}/saves/"
    COMMENT "Create resource folder at ${OutputRes}"
)

add_custom_command(
    TARGET Resources POST_BUILD
    COMMAND
    cmake -E copy
    "${PROJECT_SOURCE_DIR}/imgui.ini" "${outputDirectory}"
    COMMENT "Copied imgui.ini"
)

add_custom_command(
    TARGET Resources POST_BUILD
    COMMAND
    cmake -E copy_directory
    "${InputRes}/shaders/" "${OutputRes}/shaders"
    COMMENT "Copied Shaders from ${InputRes}/shaders/ To ${OutputRes}/shaders"
)

add_custom_command(
    TARGET Resources POST_BUILD
    COMMAND
    cmake -E copy_directory
    "${InputRes}/fonts/" "${OutputRes}/fonts"
    COMMENT "Copied Fonts from ${InputRes}/fonts/ To ${OutputRes}/fonts"
)

add_custom_command(
    TARGET Resources POST_BUILD
    COMMAND
    cmake -E copy_directory
    "${PROJECT_SOURCE_DIR}/saves/" "${outputDirectory}/saves/"
    COMMENT "Copied Saves from ${PROJECT_SOURCE_DIR}/saves/ To ${outputDirectory}/saves/"
)

file (GLOB RESOURCES_TEXTURES "${InputRes}/textures/*.png")
add_custom_command(
    TARGET Resources POST_BUILD
    COMMAND
    cmake -E copy
    ${RESOURCES_TEXTURES} "${OutputRes}/textures"
    COMMENT "Copied Textures from ${InputRes}/textures/ To ${OutputRes}/textures"
)

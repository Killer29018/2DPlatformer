add_subdirectory(glad/cmake)
glad_add_library(glad STATIC API gl:core=4.6)

set(GLM_QUIET OFF CACHE BOOL "" FORCE)
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
add_subdirectory(glm)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)
set_target_properties(glfw PROPERTIES FOLDER vendor)

add_subdirectory(imgui)

project (STB LANGUAGES CXX)
file(GLOB_RECURSE stbFiles CONFIGURE_DEPENDS "stb/*.h")
add_library(STB STATIC ${stbFiles})
target_include_directories(STB PUBLIC "stb/")
set_target_properties(STB PROPERTIES LINKER_LANGUAGE CXX)

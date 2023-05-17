cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME OpenGLGame)
project(${PROJECT_NAME})

file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
source_group("src" FILES ${CPP_FILES})
file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
source_group("include" FILES ${HPP_FILES})

add_executable(${PROJECT_NAME} ${CPP_FILES} ${HPP_FILES})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)

set(IMGUI_INCLUDES
	external/imgui/imgui.h
	external/imgui/backends/imgui_impl_glfw.h
	external/imgui/backends/imgui_impl_opengl3.h
)

set(IMGUI_SOURCES
	external/imgui/imgui.cpp
	external/imgui/backends/imgui_impl_glfw.cpp
	external/imgui/backends/imgui_impl_opengl3.cpp
	external/imgui/imgui_demo.cpp
	external/imgui/imgui_tables.cpp
	external/imgui/imgui_widgets.cpp
	external/imgui/imgui_draw.cpp
)

add_library(ImGui STATIC
	${IMGUI_INCLUDES}
	${IMGUI_SOURCES}
)

target_include_directories(ImGui PUBLIC external)
target_include_directories(ImGui PUBLIC external/imgui)
target_link_libraries(ImGui PRIVATE glad glfw)

target_link_libraries(${PROJECT_NAME} PRIVATE ImGui)


set_property(TARGET ${PROJECT_NAME} PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/binaries)
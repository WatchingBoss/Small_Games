cmake_minimum_required (VERSION 3.6)

project (shooter)

find_package(OpenGL REQUIRED)

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -g")
endif()

if(WIN32)
  set (THIRD_PART_LIBS "D:/Documents/libs/third_part_libs/")
else()
  set (THIRD_PART_LIBS "/media/ubu/data/Documents/libs/third_part_libs/")
endif()

# Including headers of third part libraries
include_directories (${THIRD_PART_LIBS})

# Third part libraries
add_library(ThirdPartLib
  ${THIRD_PART_LIBS}imgui/imgui.cpp
  ${THIRD_PART_LIBS}imgui/imgui_demo.cpp
  ${THIRD_PART_LIBS}imgui/imgui_draw.cpp
  ${THIRD_PART_LIBS}imgui/imgui_impl_glfw.cpp
  ${THIRD_PART_LIBS}imgui/imgui_impl_opengl3.cpp)

# OpenGL
add_library(ForOpenGL INTERFACE)
if(MSVC)
  set (OPEN_GL_PATH "D:/Documents/libs/open_gl/")
  include_directories(${OPEN_GL_PATH}include)
  target_link_libraries(ForOpenGL INTERFACE
    opengl32.lib
	${OPEN_GL_PATH}lib/glew32s.lib
	${OPEN_GL_PATH}lib/glfw3.lib)
else()
  target_link_libraries(ForOpenGL INTERFACE glfw GL GLEW)
endif()

file(GLOB PROJECT_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB PROJECT_HEADERS ${PROJECT_SOURCE_DIR}/inc/*.hpp)
file(GLOB PROJECT_SHADERS ${PROJECT_SOURCE_DIR}/shader/*.vert ${PROJECT_SOURCE_DIR}/shader/*.frag)

source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})

add_executable (${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${PROJECT_SHADERS})
target_link_libraries (${PROJECT_NAME} PUBLIC ThirdPartLib ForOpenGL)

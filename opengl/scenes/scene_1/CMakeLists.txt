cmake_minimum_required(VERSION 3.5)

project(GluiExamples)

set(glui_DIR	"glui/lib/cmake/glui")

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

find_package(glui REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main glui::glui_static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})

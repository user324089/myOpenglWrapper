#include <GL/glew.h>
#include "openglTest.hpp"

void openglTestClass::SetUp () {
    glfwInit();
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint (GLFW_VISIBLE, GL_FALSE);
    window = glfwCreateWindow (100, 100, "myApp", nullptr, nullptr);
    glfwMakeContextCurrent (window);
    glewExperimental = GL_TRUE;
    glewInit();
}
void openglTestClass::TearDown () {
    glfwTerminate();
}

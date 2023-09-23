#include <gtest/gtest.h>
#include <GLFW/glfw3.h>

class openglTestClass : public ::testing::Test {
    protected:
        GLFWwindow* window;
        void SetUp () override;
        void TearDown () override;
};

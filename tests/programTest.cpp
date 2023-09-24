#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <gtest/gtest.h>
#include "buffer.hpp"
#include "openglTest.hpp"
#include "program.hpp"

#include <cstring>


class programTestClass : public openglTestClass {};

#ifdef DEBUG //tests of debug messages

#endif // DEBUG

constexpr static const GLchar * vertexSimpleSource = R"(
#version 460 core
void main () {
}
)";

constexpr static const GLchar * fragmentSimpleSource = R"(
#version 460 core
out vec4 color;
void main () {
}
)";

TEST_F (programTestClass, testShaderMaking) {
    shader s (GL_VERTEX_SHADER);
    s.make (1, &vertexSimpleSource);
    EXPECT_EQ (true, glIsShader (s.getName()));
}

TEST_F (programTestClass, testShaderMoveConstructor) {
    shader s (GL_VERTEX_SHADER);
    GLuint name = s.getName();
    EXPECT_EQ (true, glIsShader (name));
    shader s2 = std::move (s);
    EXPECT_EQ (name, s2.getName());
    EXPECT_EQ (0, s.getName());
    EXPECT_EQ (true, glIsShader (name));
}

TEST_F (programTestClass, testShaderDeleting) {
    GLuint sName;
    {
        shader s (GL_VERTEX_SHADER);
        s.make (1, &vertexSimpleSource);
        sName = s.getName();
        EXPECT_EQ (true, glIsShader (sName));
    }
    EXPECT_EQ (false, glIsShader (sName));
}

TEST_F (programTestClass, testProgramSingularMaking) {
    program prog;
    prog.makeSingular (GL_VERTEX_SHADER, 1, &vertexSimpleSource);
    GLuint progName = prog.getName();
    ASSERT_EQ (true, glIsProgram (progName));

    GLint linkStatus;
    glGetProgramiv (progName, GL_LINK_STATUS, &linkStatus);

    EXPECT_EQ (0, glGetError()); 
    EXPECT_EQ (GL_TRUE, linkStatus);
}

TEST_F (programTestClass, testProgramMoveConstructor) {
    program prog;
    GLuint name = prog.getName();
    EXPECT_EQ (true, glIsProgram (name));
    program prog2 = std::move (prog);
    EXPECT_EQ (prog2.getName(), name);
    EXPECT_EQ (prog.getName(), 0);
    EXPECT_EQ (true, glIsProgram (name));
}

TEST_F (programTestClass, testProgramMaking) {
    program prog;
    shader vertexShader (GL_VERTEX_SHADER);
    vertexShader.make (1, &vertexSimpleSource);
    shader fragmentShader (GL_FRAGMENT_SHADER);
    fragmentShader.make (1, &fragmentSimpleSource);
    prog.make (vertexShader, fragmentShader);

    GLuint progName = prog.getName();
    GLint numAttachedShaders;
    glGetProgramiv (progName, GL_ATTACHED_SHADERS, &numAttachedShaders);
    ASSERT_EQ (0, numAttachedShaders);
}

constexpr static const GLchar * vertexErrorSource = R"(
#version 460 core
void main () {
    error
}
)";

constexpr static const GLchar * fragmentErrorSource = R"(
#version 460 core
out vec4 color;
void main () {
    error
}
)";

TEST_F (programTestClass, testVertexShaderError) {
    shader s (GL_VERTEX_SHADER);
    EXPECT_THROW (s.make (1, &vertexErrorSource), std::runtime_error);
}

TEST_F (programTestClass, testFragmentShaderError) {
    shader s (GL_FRAGMENT_SHADER);
    EXPECT_THROW (s.make (1, &fragmentErrorSource), std::runtime_error);
}

constexpr static const GLchar * vertexLinkErrorSource = R"(
#version 460 core
out vec2 link;
void main () {
}
)";

constexpr static const GLchar * fragmentLinkErrorSource = R"(
#version 460 core
in vec3 link;
out vec4 color;
void main () {
}
)";

TEST_F (programTestClass, testProgramError) {
    shader s1 (GL_VERTEX_SHADER);
    s1.make (1, &vertexLinkErrorSource);
    shader s2 (GL_FRAGMENT_SHADER);
    s2.make (1, &fragmentLinkErrorSource);
    program pr;
    EXPECT_THROW (pr.make (s1, s2), std::runtime_error);
}

TEST_F (programTestClass, testProgramPipelineMoveConstructor) {
    programPipeline prog;
    GLuint name = prog.getName();
    EXPECT_EQ (true, glIsProgramPipeline (name));
    programPipeline prog2 = std::move (prog);
    EXPECT_EQ (prog2.getName(), name);
    EXPECT_EQ (prog.getName(), 0);
    EXPECT_EQ (true, glIsProgramPipeline (name));
}

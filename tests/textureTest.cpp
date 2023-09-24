#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <gtest/gtest.h>
#include "texture.hpp"
#include "openglTest.hpp"

#include <cstring>


class textureTestClass : public openglTestClass {};

#ifdef DEBUG //tests of debug messages
#endif // DEBUG

TEST_F (textureTestClass, texture1DCreationAndDeletionTest) {
    GLuint name;
    {
        texture1D tex (GL_TEXTURE_1D);

        name = tex.getName ();
        EXPECT_EQ (true, glIsTexture (name));
    }
    EXPECT_EQ (false, glIsTexture (name));
}

TEST_F (textureTestClass, texture1DMoveConstructorTest) {
    texture1D tex (GL_TEXTURE_1D);

    GLuint name = tex.getName ();
    EXPECT_EQ (true, glIsTexture (name));

    texture1D tex2 = std::move (tex);

    EXPECT_EQ (name, tex2.getName());
    EXPECT_EQ (0, tex.getName());
}

TEST_F (textureTestClass, texture2DCreationAndDeletionTest) {
    GLuint name;
    {
        texture2D tex (GL_TEXTURE_2D);

        name = tex.getName ();
        EXPECT_EQ (true, glIsTexture (name));
    }
    EXPECT_EQ (false, glIsTexture (name));
}

TEST_F (textureTestClass, texture2DMoveConstructorTest) {
    texture2D tex (GL_TEXTURE_2D);

    GLuint name = tex.getName ();
    EXPECT_EQ (true, glIsTexture (name));

    texture2D tex2 = std::move (tex);

    EXPECT_EQ (name, tex2.getName());
    EXPECT_EQ (0, tex.getName());
}

TEST_F (textureTestClass, texture3DCreationAndDeletionTest) {
    GLuint name;
    {
        texture3D tex (GL_TEXTURE_3D);

        name = tex.getName ();
        EXPECT_EQ (true, glIsTexture (name));
    }
    EXPECT_EQ (false, glIsTexture (name));
}

TEST_F (textureTestClass, texture3DMoveConstructorTest) {
    texture3D tex (GL_TEXTURE_3D);

    GLuint name = tex.getName ();
    EXPECT_EQ (true, glIsTexture (name));

    texture3D tex2 = std::move (tex);

    EXPECT_EQ (name, tex2.getName());
    EXPECT_EQ (0, tex.getName());
}

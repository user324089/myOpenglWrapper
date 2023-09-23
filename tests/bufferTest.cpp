#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <gtest/gtest.h>
#include "buffer.hpp"
#include "openglTest.hpp"

#include <cstring>


class bufferTestClass : public openglTestClass {};

#ifdef DEBUG //tests of debug messages
TEST_F (bufferTestClass, testFixedDebugSubDataDynamicChecking) {
    fixedSizeBuffer fix (10, GL_MAP_READ_BIT, nullptr);
    int x = 8;
    ASSERT_THROW(fix.subdata (0, 4, &x), std::logic_error);

    fixedSizeBuffer fix2 (10, GL_MAP_READ_BIT | GL_DYNAMIC_STORAGE_BIT, nullptr);
    ASSERT_NO_THROW(fix2.subdata (0, 4, &x));

    EXPECT_EQ (0, glGetError());
}

TEST_F (bufferTestClass, testFixedDebugMapFlagsChecking) {
    fixedSizeBuffer fix (10, 0, nullptr);
    ASSERT_THROW (fix.map (0, 10, GL_MAP_READ_BIT), std::logic_error);
    ASSERT_THROW (fix.map (0, 10, GL_MAP_WRITE_BIT), std::logic_error);

    fixedSizeBuffer fix2 (10, GL_MAP_READ_BIT, nullptr);
    ASSERT_NO_THROW (fix2.map (0, 10, GL_MAP_READ_BIT));
    ASSERT_THROW (fix2.map (0, 10, GL_MAP_WRITE_BIT), std::logic_error);

    fixedSizeBuffer fix3 (10, GL_MAP_WRITE_BIT, nullptr);
    ASSERT_NO_THROW (fix3.map (0, 10, GL_MAP_WRITE_BIT));
    ASSERT_THROW (fix3.map (0, 10, GL_MAP_READ_BIT), std::logic_error);

    fixedSizeBuffer fix4 (10, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT, nullptr);
    ASSERT_NO_THROW (fix4.map (0, 10, GL_MAP_WRITE_BIT));
    ASSERT_NO_THROW (fix4.map (0, 10, GL_MAP_READ_BIT));

    fixedSizeBuffer fix5 (10, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT, nullptr);
    ASSERT_THROW (fix5.map (0, 10, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT), std::logic_error);
    ASSERT_NO_THROW (fix5.map (0, 10, GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT));

    fixedSizeBuffer fix6 (10, GL_MAP_READ_BIT, nullptr);
    ASSERT_THROW (fix6.map (0, 10, GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT), std::logic_error);

    EXPECT_EQ (0, glGetError());
}

TEST_F (bufferTestClass, testFixedMapDataChecking3) {
    {
        fixedSizeBuffer fix3 (10, GL_MAP_WRITE_BIT, nullptr);
        bufferMapping fix3Mapping = fix3.map (0, 4, GL_MAP_WRITE_BIT);
        EXPECT_THROW (fix3Mapping.flushRange (0, 4), std::logic_error);
    }
}

#endif // DEBUG

TEST_F (bufferTestClass, testBufferBinding) {
    GLint currentBound;
    buffer buff;

    glGetIntegerv (GL_ARRAY_BUFFER_BINDING, &currentBound);
    EXPECT_EQ (0, currentBound);

    buff.bind (GL_ARRAY_BUFFER);

    glGetIntegerv (GL_ARRAY_BUFFER_BINDING, &currentBound);
    EXPECT_EQ (buff.getName(), currentBound);

    buffer::unbind (GL_ARRAY_BUFFER);
    glGetIntegerv (GL_ARRAY_BUFFER_BINDING, &currentBound);
    EXPECT_EQ (0, currentBound);
}

TEST_F (bufferTestClass, testFixedBufferBinding) {
    GLint currentBound;
    fixedSizeBuffer fix (10, 0);

    glGetIntegerv (GL_ARRAY_BUFFER_BINDING, &currentBound);
    EXPECT_EQ (0, currentBound);

    fix.bind (GL_ARRAY_BUFFER);

    glGetIntegerv (GL_ARRAY_BUFFER_BINDING, &currentBound);
    EXPECT_EQ (fix.getName(), currentBound);

    buffer::unbind (GL_ARRAY_BUFFER);
    glGetIntegerv (GL_ARRAY_BUFFER_BINDING, &currentBound);
    EXPECT_EQ (0, currentBound);
}

TEST_F (bufferTestClass, testFixedDataChecking) {
    constexpr static GLchar data [] = "hello world";
    constexpr static GLsizei dataSize = sizeof(data) / sizeof(GLchar);
    fixedSizeBuffer fix (dataSize, GL_DYNAMIC_STORAGE_BIT, data);
    GLchar actualData [dataSize];
    fix.bind (GL_ARRAY_BUFFER);
    glGetBufferSubData (GL_ARRAY_BUFFER, 0, dataSize, actualData);
    ASSERT_EQ (0, actualData[dataSize-1]);
    EXPECT_STREQ (data, actualData);
    constexpr static GLchar data2 [2] = {'w','w'};
    glBufferSubData (GL_ARRAY_BUFFER, 2, 2, data2);

    glGetBufferSubData (GL_ARRAY_BUFFER, 0, dataSize, actualData);
    ASSERT_EQ (0, actualData[dataSize-1]);
    EXPECT_STREQ ("hewwo world", actualData);

    EXPECT_EQ (0, glGetError());
}

TEST_F (bufferTestClass, testDataChecking) {
    constexpr static GLchar data [] = "hello world";
    constexpr static GLsizei dataSize = sizeof(data) / sizeof(GLchar);
    buffer buff (dataSize, GL_STATIC_DRAW, data);
    GLchar actualData [dataSize];
    buff.bind (GL_ARRAY_BUFFER);
    glGetBufferSubData (GL_ARRAY_BUFFER, 0, dataSize, actualData);
    ASSERT_EQ (0, actualData[dataSize-1]);
    EXPECT_STREQ (data, actualData);
    constexpr static GLchar data2 [2] = {'w','w'};
    glBufferSubData (GL_ARRAY_BUFFER, 2, 2, data2);

    glGetBufferSubData (GL_ARRAY_BUFFER, 0, dataSize, actualData);
    ASSERT_EQ (0, actualData[dataSize-1]);
    EXPECT_STREQ ("hewwo world", actualData);

    EXPECT_EQ (0, glGetError());
}

TEST_F (bufferTestClass, testMapDataChecking) {
    constexpr static GLchar str [] = "hello";
    constexpr static size_t strSize = sizeof(str)/sizeof(GLchar);
    buffer buff;
    buff.data (strSize, nullptr, GL_STATIC_DRAW);
    
    {
        bufferMapping buffMapping = buff.map (0, strSize, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
        EXPECT_EQ (0, glGetError());

        ASSERT_NE (nullptr, buffMapping.getData());
        strcpy (static_cast<GLchar*>(buffMapping.getData()), str);
    }
    {
        bufferMapping buffMapping = buff.map (0, strSize, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
        EXPECT_EQ (0, glGetError());

        ASSERT_NE (nullptr, buffMapping.getData());
        ASSERT_EQ (0, static_cast<GLchar*>(buffMapping.getData())[strSize-1]);
        EXPECT_STREQ (str, static_cast<GLchar*>(buffMapping.getData()));
    }

}

TEST_F (bufferTestClass, testFixedMapDataChecking1) {
    constexpr static GLchar str [] = "hello";
    constexpr static size_t strSize = sizeof(str)/sizeof(GLchar);
    fixedSizeBuffer fix (strSize, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT, nullptr);

    {
        bufferMapping fixMapping = fix.map (0, strSize, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);
        EXPECT_EQ (0, glGetError());

        ASSERT_NE (nullptr, fixMapping.getData());
        strcpy (static_cast<GLchar*>(fixMapping.getData()), str);
        fixMapping.flushRange (0, strSize);
        EXPECT_EQ (0, glGetError());

        fix.bind(GL_ARRAY_BUFFER);
        GLchar actualData [strSize];

        glGetBufferSubData (GL_ARRAY_BUFFER, 0, strSize, actualData);

        ASSERT_EQ (0, actualData[strSize-1]);
        EXPECT_STREQ(str, actualData);

        EXPECT_EQ (0, glGetError());
    }


}

TEST_F (bufferTestClass, testFixedMapDataChecking2) {
    constexpr static GLchar str [] = "hello";
    constexpr static size_t strSize = sizeof(str)/sizeof(GLchar);
    fixedSizeBuffer fix2 (strSize, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT, nullptr);

    {
        bufferMapping fix2Mapping = fix2.map (0, strSize, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
        EXPECT_EQ (0, glGetError());

        ASSERT_NE (nullptr, fix2Mapping.getData());
        strcpy (static_cast<GLchar*>(fix2Mapping.getData()), str);

        fix2.bind(GL_ARRAY_BUFFER);
        GLchar actualData [strSize];

        glGetBufferSubData (GL_ARRAY_BUFFER, 0, strSize, actualData);

        ASSERT_EQ (0, actualData[strSize-1]);
        EXPECT_STREQ(str, actualData);

        EXPECT_EQ (0, glGetError());
    }
}

link_libraries (glfw glew_s ${OPENGL_LIBRARIES} gtest gtest_main)
include_directories (${GLFW_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${GTEST_INCLUDE_DIR})
add_compile_definitions (GLEW_NO_GLU)

add_executable (bufferTest bufferTest.cpp openglTest.cpp ${MY_OPENGL_WRAPPER_INCLUDE_DIR}/buffer.hpp)
add_executable (programTest programTest.cpp openglTest.cpp ${MY_OPENGL_WRAPPER_INCLUDE_DIR}/program.hpp)

target_link_libraries (bufferTest bufferLib)
target_link_libraries (programTest programLib)
target_compile_features(programTest PRIVATE cxx_std_17)

enable_testing()
add_test (BufferTest bufferTest)
add_test (ProgramTest programTest)

link_libraries (glfw glew_s ${OPENGL_LIBRARIES})
include_directories (${GLFW_INCLUDE_DIR} ${GLEW_INCLUDE_DIR})
add_compile_definitions (GLEW_NO_GLU)

add_library (bufferLib buffer.cpp ${MY_OPENGL_WRAPPER_INCLUDE_DIR}/buffer.hpp)
add_library (programLib program.cpp ${MY_OPENGL_WRAPPER_INCLUDE_DIR}/program.hpp)
target_compile_features(programLib PRIVATE cxx_std_17)

set (MY_OPENGL_WRAPPER_LIBRARIES bufferLib programLib CACHE INTERNAL "")

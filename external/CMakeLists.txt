add_subdirectory (glfw)

set (GLFW_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include CACHE INTERNAL "")

set (GLEW_USE_STATIC_LIBS TRUE)
set (GLEW_VERBOSE TRUE)
set (BUILD_UTILS OFF CACHE BOOL "utilities")
add_subdirectory (glew-cmake/build/cmake)

set (GLEW_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glew-cmake/include CACHE INTERNAL "")

if (MY_OPENGL_WRAPPER_TEST)
    if (MSVC)
        set(gtest_force_shared_crt on)
    endif (MSVC)
    add_subdirectory (googletest)

    set (GTEST_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include CACHE INTERNAL "")
endif (MY_OPENGL_WRAPPER_TEST)

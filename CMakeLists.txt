cmake_minimum_required (VERSION 3.11)

project (myProject)

find_package (OpenGL REQUIRED)

add_subdirectory (external)
add_subdirectory (include)

include_directories (PRIVATE ${MY_OPENGL_WRAPPER_INCLUDE_DIR})

add_subdirectory (src)

if (NOT DEFINED MY_OPENGL_WRAPPER_TEST)
    set (MY_OPENGL_WRAPPER_TEST TRUE)
endif ()

if (MY_OPENGL_WRAPPER_TEST)
    add_subdirectory (tests)
endif()

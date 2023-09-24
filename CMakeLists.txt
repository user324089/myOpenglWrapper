cmake_minimum_required (VERSION 3.12)

project (myProject)

find_package (OpenGL REQUIRED)

if (NOT DEFINED MY_OPENGL_WRAPPER_TEST)
    set (MY_OPENGL_WRAPPER_TEST TRUE)
endif ()

add_subdirectory (external)

if (MSVC)
    if ("${CMAKE_BUILD_TYPE}" MATCHES "Debug")
        add_compile_options (/Od /Zi /W4)
        add_compile_definitions (DEBUG)
    elseif ("${CMAKE_BUILD_TYPE}" MATCHES "Release")
        add_compile_options (/Ot)
        add_compile_definitions (NDEBUG)
    endif()
else ()
    if ("${CMAKE_BUILD_TYPE}" MATCHES "Debug")
        add_compile_options (-O0 -g -Wall -Wextra)
        add_compile_definitions (DEBUG)
    elseif ("${CMAKE_BUILD_TYPE}" MATCHES "Release")
        add_compile_options (-O2)
        add_compile_definitions (NDEBUG)
    endif()
endif()

add_subdirectory (include)

include_directories (PRIVATE ${MY_OPENGL_WRAPPER_INCLUDE_DIR})

add_subdirectory (src)

if (MY_OPENGL_WRAPPER_TEST)
    add_subdirectory (tests)
endif(MY_OPENGL_WRAPPER_TEST)

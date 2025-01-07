# Dependencies Folder
set(DEPS_INCLUDES "${CMAKE_SOURCE_DIR}/extlibs/Include" CACHE PATH "External Includes")
set(DEPS_SRC "${CMAKE_SOURCE_DIR}/extlibs/src")

# GLFW
add_subdirectory(lib/GLFW)

# stb
add_library(stb STATIC 
    "${DEPS_SRC}/stb/stb_image.cpp"
)
target_include_directories(stb PUBLIC 
    "${DEPS_INCLUDES}"
)

# glm
add_library(glm INTERFACE
)
target_include_directories(glm INTERFACE 
    "${DEPS_INCLUDES}"
)

# glad
add_library(glad STATIC
    ${DEPS_SRC}/glad/glad.c
)
target_include_directories(glad PUBLIC
    "${DEPS_INCLUDES}"
)

# assimp
include(FetchContent)
FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG master
)

# Disable unnecessary parts of Assimp before fetching
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Disable Assimp tools")
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Disable Assimp tests")
set(ASSIMP_BUILD_DOCS OFF CACHE BOOL "Disable Assimp docs")
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "Disable Assimp samples")
set(ASSIMP_INSTALL OFF CACHE BOOL "Skip Assimp installation")
set(ASSIMP_INSTALL_PDB OFF CACHE BOOL "Skip PDB Assimp installation")
FetchContent_MakeAvailable(assimp)

# Explicitly copy revision.h and config.h to the assimp includes
set(ASSIMP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/extlibs/include/External/assimp")
file(COPY "${assimp_BINARY_DIR}/include/assimp/revision.h" DESTINATION ${ASSIMP_INCLUDE_DIR})
file(COPY "${assimp_BINARY_DIR}/include/assimp/config.h" DESTINATION ${ASSIMP_INCLUDE_DIR})

# Set the binary directory for Assimp and cache it for use in external includes
set(assimp_BINARY_DIR "${assimp_BINARY_DIR}" CACHE PATH "External Includes")

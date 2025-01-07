cmake_minimum_required(VERSION 3.26.0)

project(FireGL
    VERSION 1.0.0
)

# CMake settings
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "") # Suppress warnings for deprecated functionality
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define an option for enabling the example project
option(BUILD_EXAMPLE "Build the example project" OFF)

# Add External Dependencies from `extlibs` folder
add_subdirectory(extlibs)

# Add Project Source and Header Files
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "${CMAKE_SOURCE_DIR}/include/*.h")

# Define the static library for the project (instead of executable)
add_library(FireGL STATIC
    ${SOURCE_FILES} ${HEADER_FILES}
)

# Ensure assimp builds first
add_dependencies(FireGL assimp)

# Apply GLM-specific compile definitions
target_compile_definitions(FireGL PRIVATE
    GLM_FORCE_CXX2A
    GLM_FORCE_RADIANS
    GLM_FORCE_DEPTH_ZERO_TO_ONE
    GLM_FORCE_RIGHT_HANDED
    GLM_FORCE_SILENT_WARNINGS
    GLM_FORCE_SWIZZLE
    GLM_ENABLE_EXPERIMENTAL
)

# Copy the GLFW library (glfw3.lib) to the appropriate build output directory (lib/Debug or lib/Release)
# This ensures that the GLFW library is available in the build folder for easy linking and usage
add_custom_command(
    TARGET FireGL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory # Making sure the directory exist
        ${CMAKE_BINARY_DIR}/lib/$<CONFIG>
    COMMAND ${CMAKE_COMMAND} -E copy
        ${GLFW_LIB_DIR}/${GLFW_LIBRARY}
        ${CMAKE_BINARY_DIR}/lib/$<CONFIG>
)

# Copy Assimp library files (static/dynamic libraries and binaries like DLLs) from the binary directory to the lib/$<CONFIG>/ folder.
if(WIN32)
    add_custom_command(
        TARGET FireGL POST_BUILD
        COMMAND ${CMAKE_COMMAND}
            -Dsource_dir=${assimp_BINARY_DIR}/lib/$<CONFIG>
            -Ddestination_dir=${CMAKE_BINARY_DIR}/lib/$<CONFIG>
            -P ${CMAKE_SOURCE_DIR}/extlibs/CopyLibAssimpHelper.cmake
    )
endif()
add_custom_command(
    TARGET FireGL POST_BUILD
    COMMAND ${CMAKE_COMMAND}
        -Dsource_dir=${assimp_BINARY_DIR}/bin/$<CONFIG>
        -Ddestination_dir=${CMAKE_BINARY_DIR}/lib/$<CONFIG>
        -P ${CMAKE_SOURCE_DIR}/extlibs/CopyLibAssimpHelper.cmake
)

# Ensure the extlibs/lib/assimp directory exists, then copy the Assimp libraries to the root extlibs folder
# This step is for placing the Assimp libraries in a centralized external library directory
if(WIN32)
    add_custom_command(
        TARGET FireGL POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory # Making sure the directory exist
            ${CMAKE_SOURCE_DIR}/extlibs/lib/assimp
        COMMAND ${CMAKE_COMMAND}
            -Dsource_dir=${assimp_BINARY_DIR}/lib/$<CONFIG>
            -Ddestination_dir=${CMAKE_SOURCE_DIR}/extlibs/lib/assimp
            -P ${CMAKE_SOURCE_DIR}/extlibs/CopyLibAssimpHelper.cmake
    )
else()
    add_custom_command(
        TARGET FireGL POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory # Making sure the directory exist
            ${CMAKE_SOURCE_DIR}/extlibs/lib/assimp
    )
endif()
add_custom_command(
    TARGET FireGL POST_BUILD
    COMMAND ${CMAKE_COMMAND}
        -Dsource_dir=${assimp_BINARY_DIR}/bin/$<CONFIG>
        -Ddestination_dir=${CMAKE_SOURCE_DIR}/extlibs/lib/assimp
        -P ${CMAKE_SOURCE_DIR}/extlibs/CopyLibAssimpHelper.cmake
)

# Link External Libraries
target_link_libraries(FireGL PRIVATE
    GLFW                               			 # GLFW header-only library
    "${CMAKE_BINARY_DIR}/lib/$<CONFIG>/${GLFW_LIBRARY}"  # Platform-specific static library
    glm                                			 # GLM (header-only)
    stb                                			 # STB (static library)
    glad                               			 # Glad (static library)
    assimp                                               # Assimp (dynamic library)
)

if(MACOS_ARCHITECTURE)
    target_link_libraries(FireGL PRIVATE 
        "-framework Cocoa" "-framework OpenGL" "-framework IOKit"
    )
endif()

# Organize dependencies and main project for the IDE
set_target_properties(glm PROPERTIES 
    FOLDER "Dependencies"
)
set_target_properties(glad PROPERTIES 
    FOLDER "Dependencies"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>"
)
set_target_properties(stb PROPERTIES 
    FOLDER "Dependencies"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>"
)
if(WIN32)
    set_target_properties(UpdateAssimpLibsDebugSymbolsAndDLLs PROPERTIES 
        FOLDER "Dependencies"
    )
    set_target_properties(zlibstatic PROPERTIES 
        FOLDER "Dependencies"
    )
endif()
set_target_properties(assimp PROPERTIES 
    FOLDER "Dependencies"
)
set_target_properties(FireGL PROPERTIES 
    FOLDER ""
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>"
)

# Include Directories
target_include_directories(FireGL PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
    "${DEPS_INCLUDES}"
)

# Precompiled Headers
target_precompile_headers(FireGL PRIVATE
    "${PROJECT_SOURCE_DIR}/include/FireGL/fglpch.h"
)

# Include the example project if ENABLE_EXAMPLE is ON
if(BUILD_EXAMPLE)
    message(STATUS "Building example project...")

    # GLOB_RECURSE sucks...
    file(GLOB_RECURSE EXAMPLE_SOURCE_FILES "${CMAKE_SOURCE_DIR}/Example/*.cpp")
    file(GLOB_RECURSE EXAMPLE_HEADER_FILES "${CMAKE_SOURCE_DIR}/Example/*.h")

    # Add the example target
    add_executable(ExampleProject 
        ${EXAMPLE_SOURCE_FILES} ${EXAMPLE_HEADER_FILES}    
    )

    # Define the base content directory for shaders (only for ExampleProject)
    set(PROJECT_ROOT "${CMAKE_SOURCE_DIR}/Example/")

    # Pass the ASSETS_BASE_DIR definition to ExampleProject only
    target_compile_definitions(ExampleProject PRIVATE PROJECT_ROOT="${PROJECT_ROOT}")

    # Include Directories
    target_include_directories(ExampleProject PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        "${DEPS_INCLUDES}"
    )

    # Link libraries
    target_link_libraries(ExampleProject PRIVATE FireGL)
    
    # Placing .dll file with the .exe file
    add_custom_command(
        TARGET ExampleProject POST_BUILD
        COMMAND ${CMAKE_COMMAND}
            -Dsource_dir=${assimp_BINARY_DIR}/bin/$<CONFIG>
            -Ddestination_dir=${CMAKE_BINARY_DIR}/$<CONFIG>
            -P ${CMAKE_SOURCE_DIR}/extlibs/CopyLibAssimpHelper.cmake
    )

    # Mark the example as the startup project for IDEs
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ExampleProject)
else()
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT FireGL)
endif()

# Compiler-Specific Warnings
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	add_compile_options(/W4 /wd4100)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	add_compile_options(-Wall -Wextra -Wno-unused-parameter)
endif()

#### Install rules ####

set(CMAKE_INSTALL_PREFIX "${PROJECT_NAME}-${FireGL_VERSION}")

# Install static libraries into the specified destination directory
install(TARGETS glad DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(TARGETS stb DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(FILES "${GLFW_LIB_DIR}/${GLFW_LIBRARY}" DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(TARGETS assimp DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(TARGETS FireGL DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

# Install header files from external libraries to the include directory
install(DIRECTORY ${CMAKE_SOURCE_DIR}/extlibs/include/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

# Install project-specific headers into the include directory
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

# Install Licence and README
install(FILES "${CMAKE_SOURCE_DIR}/License.txt" "${CMAKE_SOURCE_DIR}/README.md" DESTINATION ${CMAKE_INSTALL_PREFIX})

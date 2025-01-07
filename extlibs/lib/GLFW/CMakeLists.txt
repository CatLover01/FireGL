# Root path for GLFW library
set(GLFW_ROOT "${CMAKE_SOURCE_DIR}/extlibs/lib/GLFW")

# Set GLFW include directory
set(GLFW_INCLUDE_DIR "${GLFW_ROOT}/include")

# Header only lib
add_library(GLFW INTERFACE
)
target_include_directories(GLFW INTERFACE 
    "${GLFW_INCLUDE_DIR}"
)

# Configure GLFW library paths for linking with FireGL
if(MSVC)
    # Determine architecture (x86 or x64)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        # 64-bit system (x64)
        if(MSVC_VERSION GREATER_EQUAL 1930)  # MSVC 2022
            set(GLFW_LIB_DIR "${GLFW_ROOT}/Windows_x64/lib-vc2022" CACHE PATH "GLFW Lib Directory")
        elseif(MSVC_VERSION GREATER_EQUAL 1920)  # MSVC 2019
            set(GLFW_LIB_DIR "${GLFW_ROOT}/Windows_x64/lib-vc2019" CACHE PATH "GLFW Lib Directory")
        elseif(MSVC_VERSION GREATER_EQUAL 1910)  # MSVC 2017
            set(GLFW_LIB_DIR "${GLFW_ROOT}/Windows_x64/lib-vc2017" CACHE PATH "GLFW Lib Directory")
        elseif(MSVC_VERSION GREATER_EQUAL 1900)  # MSVC 2015
            set(GLFW_LIB_DIR "${GLFW_ROOT}/Windows_x64/lib-vc2015" CACHE PATH "GLFW Lib Directory")
        elseif(MSVC_VERSION GREATER_EQUAL 1800)  # MSVC 2013
            set(GLFW_LIB_DIR "${GLFW_ROOT}/Windows_x64/lib-vc2013" CACHE PATH "GLFW Lib Directory")
        endif()
    else()
        # 32-bit system (x86)
        if(MSVC_VERSION GREATER_EQUAL 1930)  # MSVC 2022
            set(GLFW_LIB_DIR "${GLFW_ROOT}/Windows_x86/lib-vc2022" CACHE PATH "GLFW Lib Directory")
        elseif(MSVC_VERSION GREATER_EQUAL 1920)  # MSVC 2019
            set(GLFW_LIB_DIR "${GLFW_ROOT}/Windows_x86/lib-vc2019" CACHE PATH "GLFW Lib Directory")
        elseif(MSVC_VERSION GREATER_EQUAL 1910)  # MSVC 2017
            set(GLFW_LIB_DIR "${GLFW_ROOT}/Windows_x86/lib-vc2017" CACHE PATH "GLFW Lib Directory")
        elseif(MSVC_VERSION GREATER_EQUAL 1900)  # MSVC 2015
            set(GLFW_LIB_DIR "${GLFW_ROOT}/Windows_x86/lib-vc2015" CACHE PATH "GLFW Lib Directory")
        elseif(MSVC_VERSION GREATER_EQUAL 1800)  # MSVC 2013
            set(GLFW_LIB_DIR "${GLFW_ROOT}/Windows_x86/lib-vc2013" CACHE PATH "GLFW Lib Directory")
        endif()
    endif()

    # Set GLFW library name for linking
    set(GLFW_LIBRARY "glfw3.lib" CACHE PATH "GLFW Lib Name")

elseif(MINGW)
    # MinGW-w64 configuration
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        # 64-bit MinGW
        set(GLFW_LIB_DIR "${GLFW_ROOT}/Windows_x64/lib-mingw-w64" CACHE PATH "GLFW Lib Directory")
    else()
        # 32-bit MinGW
        set(GLFW_LIB_DIR "${GLFW_ROOT}/Windows_x86/lib-mingw-w64" CACHE PATH "GLFW Lib Directory")
    endif()

    # Set GLFW library name for linking
    set(GLFW_LIBRARY "libglfw3.lib" CACHE PATH "GLFW Lib Name")
elseif(UCRT)
    # UCRT configuration (Universal CRT)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        # 64-bit UCRT
        set(GLFW_LIB_DIR "${GLFW_ROOT}/Windows_x64/lib-static-ucrt" CACHE PATH "GLFW Lib Directory")
    else()
        # 32-bit UCRT
        set(GLFW_LIB_DIR "${GLFW_ROOT}/Windows_x86/lib-static-ucrt" CACHE PATH "GLFW Lib Directory")
    endif()

    # Set GLFW library name for linking
    set(GLFW_LIBRARY "glfw3dll.lib" CACHE PATH "GLFW Lib Name")
elseif(APPLE)

    # Get the macOS architecture automatically
    execute_process(
        COMMAND uname -m
        OUTPUT_VARIABLE MACOS_ARCHITECTURE
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    # Cache the value to allow macOS frameworks to be included in the root CMake configuration
    set(MACOS_ARCHITECTURE ${MACOS_ARCHITECTURE} CACHE STRING "Architecture of macOS")

    # macOS architecture detection
    if(MACOS_ARCHITECTURE STREQUAL "arm64")
        set(GLFW_LIB_DIR "${GLFW_ROOT}/macOS/lib-arm64" CACHE PATH "GLFW Lib Directory")
    elseif(MACOS_ARCHITECTURE STREQUAL "x86_64")
        set(GLFW_LIB_DIR "${GLFW_ROOT}/macOS/lib-x86_64" CACHE PATH "GLFW Lib Directory")
    elseif(MACOS_ARCHITECTURE STREQUAL "universal")
        set(GLFW_LIB_DIR "${GLFW_ROOT}/macOS/lib-universal" CACHE PATH "GLFW Lib Directory")
    endif()

    # Set GLFW library name for linking
    set(GLFW_LIBRARY "libglfw3.a" CACHE PATH "GLFW Lib Name")
elseif(UNIX)
    # GLFW is not pre-built for Linux; you need to build it from source
endif()

# Check if GLFW_LIB_DIR exists and contains the required library
if(NOT EXISTS "${GLFW_LIB_DIR}/${GLFW_LIBRARY}")
    message(STATUS "GLFW library not found at ${GLFW_LIB_DIR}/${GLFW_LIBRARY}")
endif()

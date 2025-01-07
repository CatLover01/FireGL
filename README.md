
# OpenGL Renderer - FireGL

**FireGL** is a C++ RAII library designed for rendering, providing essential functionality for graphics programming. It is easy to use, extendable, and cross-platform, allowing for seamless integration into projects while offering flexibility for custom extensions.

---

## Features

- **RAII (Resource Acquisition Is Initialization)**: Automatic resource management.
- **Extensible**: Easily extendable for custom rendering requirements.
- **Cross-platform**: Tested on Windows and macOS. Linux support should work as expected, but hasn't been fully tested yet.

---

## Installation

### Example Application

To build and run the example application, enable the `BUILD_EXAMPLE` option during configuration:

```bash
-DBUILD_EXAMPLE=ON  # Default is OFF
```

The example application demonstrates the basic usage of the library and provides an example of how to implement an app using it.

### Building the Example Application

1. Download and extract the source code.
2. Initialize **CMake**:
   - In the root directory of your project, create a build directory: 
     ```bash
     mkdir build && cd build
     ```
   - Run CMake to configure the project:  
     ```bash
     cmake ../ -DBUILD_EXAMPLE=ON
     cmake --build .
     ```

3. If you want to use the built library for your own project, you can build it without the `-DBUILD_EXAMPLE` option:
   - Libraries will be located in `<Project_Root>/build/lib/<Config>/`. Ensure these libraries are correctly linked in your project.
   - Include headers by adding `<Project_Root>/include` to your project's include directories.
   - Access external dependencies by adding `<Project_Root>/extlibs/include` to your include paths.

### Installing to a System Directory (Optional)

To install the library to a system directory (e.g., `/usr/local` or `C:\Program Files`):

1. Run the following CMake command:
   ```bash
   cmake --install . --config Debug --prefix "C:\Program Files (x86)"
   ```

2. Ensure the build configuration (Debug/Release) matches your installation configuration to avoid issues with missing or incorrect files.

---

## External libraries

- **GLFW**: Window management and input handling.
- **GLAD**: OpenGL extension loading.
- **STB**: Image loading.
- **GLM**: Mathematics library for graphics.
- **KHR**: Khronos headers.
- **Assimp**: Asset import library.

---

## System Requirements

- **C++ Version**: C++20 or later.
- **Platform Support**: 
  - **Windows**: Fully supported.
  - **macOS**: Fully supported with OpenGL 3.2 to 4.1. Newer versions are not supported due to macOS's deprecation of OpenGL in favor of Metal.
  - **Linux**: Support should work, but hasn't been fully tested.

---

## Documentation

All documentation is provided within the source code files. Classes and functions are fully documented with detailed descriptions and examples directly in the code.

---

## License

This project is licensed under the MIT License, see the [LICENSE](License.txt) file for details.

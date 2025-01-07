# The script expects following parameters:
#
# - source_dir - directory from which copy libraries
# - destination_dir - directory to which copy libraries

if(WIN32)
    # Windows
    file(GLOB LIB "${source_dir}/assimp-vc*-mtd.*")
else()
    # macOS + Linux
    file(GLOB LIB "${source_dir}/libassimp.*")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${LIB}
        ${destination_dir}
        COMMAND_ERROR_IS_FATAL ANY # Fail the CMake script if the subprocess fails
)

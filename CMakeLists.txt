cmake_minimum_required(VERSION 3.15)
project(LazyData)



###############################################################
# Include the FetchContent module to handle external dependencies
include(FetchContent)

# Include the dependencies from the dependencies folder
add_subdirectory(dependencies)
###############################################################



###############################################################
# Copy resources directory to the build/resources directory
###############################################################
# Copy the directory
file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR}/bin)
###############################################################



###############################################################
# Collect all source files from the src directory
file(GLOB SOURCES "src/*.cpp")

# Add the executable for the LazyData project using the collected source files
add_executable(LazyData ${SOURCES} ${DEPENDENCY_SOURCES})

# Define IMGUI_DEFINE_MATH_OPERATORS before compiling sources that include imgui.h
target_compile_definitions(LazyData PRIVATE IMGUI_DEFINE_MATH_OPERATORS)

# Add the includes of the dependencies
target_include_directories(LazyData PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
    ${DEPENDENCIES_INCLUDE_PATHS}
)
###############################################################



###############################################################
# Linker options
###############################################################

# Link executable to all the dependencies libraries
target_link_libraries(LazyData PRIVATE 
    ${DEPENDENCY_LIBS_TO_LINK_TO}
)



###############################################################
# Tell cmake where to place binaries and libraries
###############################################################

set_target_properties(LazyData PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)



###############################################################
# Ensuring the executable knows where to find its dependencies
###############################################################

set(CMAKE_INSTALL_RPATH "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

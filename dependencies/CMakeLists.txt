# dependencies/CMakeLists.txt

# FetchContent is used to download external dependencies
include(FetchContent)



###############################################################
# SFML library
###############################################################

# Declare SFML dependency with a specific Git commit hash
FetchContent_Declare(
    sfml
    GIT_REPOSITORY "https://github.com/SFML/SFML"
    GIT_TAG 2.6.1
)

# Set SFML to build static libraries
set(BUILD_SHARED_LIBS OFF)

# Make SFML content available for use
FetchContent_MakeAvailable(sfml)

# SFML libraries to link to depending on whether SFML is
# to be linked statically or dynamically
set(SFML_LIBRARIES
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    sfml-network
)
###############################################################



###############################################################
# ImGui library
###############################################################
# Declare ImGui dependency with a specific commit hash
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG master
)

# Make ImGui content available for use
FetchContent_MakeAvailable(imgui)
###############################################################



###############################################################
# ImGui-SFML library
###############################################################
# Declare ImGui-SFML dependency with a specific commit hash
FetchContent_Declare(
    imgui_sfml
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml
    GIT_TAG 2.6.x
)

# Specify ImGui directory and disable ImGui-SFML's SFML finder
set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_SFML_IMGUI_DEMO ON)

# Make ImGui-SFML content available for use
FetchContent_MakeAvailable(imgui_sfml)
###############################################################



###############################################################
# Copy the contents of imconfig-SFML.h to your imconfig.h file.
# - To be able to cast ImVec2 to sf::Vector2f and vice versa
###############################################################
# Define the paths
set(SOURCE_FILE "${CMAKE_BINARY_DIR}/_deps/imgui_sfml-src/imconfig-SFML.h")
set(DEST_FILE "${CMAKE_BINARY_DIR}/_deps/imgui-src/imconfig.h")

# Append an extra newline to the destination file
file(APPEND ${DEST_FILE} "\n")

# Append the contents of the source file to the destination file
file(READ ${SOURCE_FILE} SOURCE_CONTENTS)
file(APPEND ${DEST_FILE} "${SOURCE_CONTENTS}")
###############################################################




###############################################################
# imnodes libary
# - This is used to create/visualize computational node graphs
#   in the application
###############################################################
# Declare imnodes library dependency with a specific commit hash
FetchContent_Declare(
    imnodes
    GIT_REPOSITORY https://github.com/Nelarius/imnodes.git
    GIT_TAG master
)

# Download (but do not configure) imnodes
# NOTE: We are adding imnodes source files to the
#       project via this cmake file and not imnodes's cmake
#       file since it expects imgui to be installed on
#       the system
FetchContent_GetProperties(imnodes)
if(NOT imnodes_POPULATED)
    FetchContent_Populate(imnodes)

    # Add the imnodes library
    add_library(imnodes STATIC 
        ${imnodes_SOURCE_DIR}/imnodes.cpp
        # Add other source files as needed
    )

    # Define IMGUI_DEFINE_MATH_OPERATORS before compiling imnodes
    target_compile_definitions(imnodes PRIVATE IMGUI_DEFINE_MATH_OPERATORS)

    # Set the include directories for imnodes
    target_include_directories(imnodes PRIVATE 
        ${imgui_SOURCE_DIR}
        ${imnodes_SOURCE_DIR}
        "${CMAKE_BINARY_DIR}/_deps/sfml-src/include"  # Add SFML include directories
        "${CMAKE_BINARY_DIR}/_deps/imgui_sfml-src"    # Add imgui-sfml generated directory
    )
endif()
###############################################################



###############################################################
# implot library
# - This is used to visualize data with plots in imgui
###############################################################
# Declare implot dependency with a specific commit hash
FetchContent_Declare(
    implot
    GIT_REPOSITORY https://github.com/epezent/implot.git
    GIT_TAG master
)

# Make ImGui-SFML content available for use
FetchContent_MakeAvailable(implot)
###############################################################



###############################################################
# imgui-filebrowser library
# - This is used to open/save files within the application
###############################################################
# Declare imgui_filebrowser dependency with a specific commit hash
FetchContent_Declare(
    imgui_filebrowser
    GIT_REPOSITORY https://github.com/AirGuanZ/imgui-filebrowser.git
    GIT_TAG master
)

# Make ImGui-SFML content available for use
FetchContent_MakeAvailable(imgui_filebrowser)
###############################################################



###############################################################
# LazyMatrix library
###############################################################

# Declare LazyMatrix dependency with a specific commit hash
FetchContent_Declare(
    LazyMatrix
    GIT_REPOSITORY https://github.com/navyenzo/LazyMatrix.git
    GIT_TAG main
)

# Make LazyMatrix content available for use
FetchContent_MakeAvailable(LazyMatrix)
###############################################################



###############################################################
# nlohmann json library
###############################################################

# Declare json dependency with a specific commit hash
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG develop
)

# Make LazyMatrix content available for use
FetchContent_MakeAvailable(json)
###############################################################



###############################################################
# Boost library
###############################################################

set(BOOST_ENABLE_CMAKE ON)

# Declare boost dependency with a specific commit hash
FetchContent_Declare(
    boost
    GIT_REPOSITORY https://github.com/steinwurf/boost.git
    GIT_TAG master
)

# Make boost content available for use
FetchContent_MakeAvailable(boost)

# Get a list of all subdirectories in the libs directory
# file(GLOB BOOST_LIB_SUBDIRS LIST_DIRECTORIES true "${CMAKE_BINARY_DIR}/_deps/boost-src/libs/*")

# Include the top-level Boost directory
set(BOOST_INCLUDE_DIR "${boost_SOURCE_DIR}/boost")
message(STATUS "Including Boost directory: ${BOOST_INCLUDE_DIR}")
###############################################################



###############################################################
# Collect all source files from all libraries
###############################################################
file(GLOB IMGUI_SOURCES ${CMAKE_BINARY_DIR}/_deps/imgui-src/*.cpp)
file(GLOB IMGUI_SFML_SOURCES ${CMAKE_BINARY_DIR}/_deps/imgui_sfml-src/*.cpp)
#file(GLOB IMNODES_SOURCES ${CMAKE_BINARY_DIR}/_deps/imnodes-src/*.cpp)
file(GLOB IMPLOT_SOURCES ${CMAKE_BINARY_DIR}/_deps/implot-src/*.cpp)

set(DEPENDENCY_SOURCES
    "${IMGUI_SOURCES}"
    "${IMGUI_SFML_SOURCES}"
    #"${IMNODES_SOURCES}"
    "${IMPLOT_SOURCES}"
    PARENT_SCOPE
)
###############################################################



###############################################################
# Create path that includes all include paths needed for the
# dependencies
###############################################################
find_package(OpenGL REQUIRED)

set(DEPENDENCIES_INCLUDE_PATHS
    ${OPENGL_INCLUDE_DIR}
    "${CMAKE_BINARY_DIR}/_deps/sfml-src/include"
    "${CMAKE_BINARY_DIR}/_deps/imgui-src"
    "${CMAKE_BINARY_DIR}/_deps/imgui_sfml-src"
    "${CMAKE_BINARY_DIR}/_deps/imgui_node_editor-src"
    "${CMAKE_BINARY_DIR}/_deps/imnodes-src"
    "${CMAKE_BINARY_DIR}/_deps/implot-src"
    "${CMAKE_BINARY_DIR}/_deps/imgui_filebrowser-src"
    "${CMAKE_BINARY_DIR}/_deps/nfd-src/src/include"
    "${CMAKE_BINARY_DIR}/_deps/nfd-src/src"
    "${CMAKE_BINARY_DIR}/_deps/lazymatrix-src/include"
    "${CMAKE_BINARY_DIR}/_deps/json-src/include"
    ${boost_SOURCE_DIR}
    PARENT_SCOPE
)
###############################################################



###############################################################
# Create the variable holding all the libraries to link the
# executable to
###############################################################

set(DEPENDENCY_LIBS_TO_LINK_TO
    ${SFML_LIBRARIES}
    ${OPENGL_gl_LIBRARY}
    imnodes
    LazyMatrix
    # Add Boost libraries (if any specific Boost libraries are used in the project)
    PARENT_SCOPE
)
###############################################################

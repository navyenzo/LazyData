# Catch2 library used for testing
include(FetchContent)
FetchContent_Declare(
  catch
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG devel
)
FetchContent_MakeAvailable(catch)

# Collect all source files defined in this directory
file(GLOB TEST_SOURCES "*.cpp")

# Output the names of the collected test source files
message("Test source files found: ${TEST_SOURCES}")

# Add the source files to the tests executable
add_executable(lazydata_tests ${TEST_SOURCES})

# Link the Catch2 library
target_link_libraries(lazydata_tests PRIVATE Catch2::Catch2WithMain)

# Add the include directories
target_include_directories(lazydata_tests PUBLIC
    "${CMAKE_SOURCE_DIR}/include"
    ${CMAKE_BINARY_DIR}/_deps/catch-src/src
)

# Set the output directory for the tests executable
set_target_properties(lazydata_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

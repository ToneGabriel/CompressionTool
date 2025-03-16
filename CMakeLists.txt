cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
project(
    CompressionTool
    VERSION 1.0.0
    LANGUAGES C CXX
)

# Languages
set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

# Library destinations
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Names
set(PROJECT_LIBRARY_NAME    "_compression_tool")
set(PROJECT_EXECUTABLE_NAME "compression_tool")

# Create project library (link executable and other cpp files to this)
add_library(${PROJECT_LIBRARY_NAME} STATIC)

# CMakeLists subdirectory config
add_subdirectory(configuration)
add_subdirectory(code)
add_subdirectory(app)

# Install library
install(TARGETS ${PROJECT_LIBRARY_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Install the executable
install(TARGETS ${PROJECT_EXECUTABLE_NAME} DESTINATION bin)

# CPack settings
set(CPACK_GENERATOR "NSIS")
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
include(CPack)

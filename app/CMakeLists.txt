# Crete executable
add_executable(${PROJECT_EXECUTABLE_NAME} main.cpp)

# Link it to the library
target_link_libraries(${PROJECT_EXECUTABLE_NAME} PUBLIC ${PROJECT_LIBRARY_NAME})

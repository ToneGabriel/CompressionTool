# Add sources and include files from current directory to project library
target_include_directories(${LIBRARY_NAME} PUBLIC ${CURRENT_DIRECTORY})
target_sources(${LIBRARY_NAME} PUBLIC

)

add_subdirectory(helpers)
add_subdirectory(encode)
add_subdirectory(archive)
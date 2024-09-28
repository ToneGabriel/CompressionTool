#pragma once

#include <filesystem>
#include <fstream>

#include "_Core.h"

#define FILES_BEGIN namespace files {
#define FILES_END }



FILES_BEGIN

std::string create_empty_file_from(const std::string& original, const std::string& newExtension);
void read_file_and_process_buffer(const std::string& filename, std::function<void(char*, std::streamsize)> processFunc);

FILES_END
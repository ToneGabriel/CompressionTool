#pragma once

#include <filesystem>
#include <fstream>

#include "_Core.h"

#define FILES_BEGIN namespace files {
#define FILES_END }



FILES_BEGIN

namespace fs = std::filesystem;

std::string create_empty_file_from(const std::string& original, const std::string& newExtension);
std::unordered_map<symbol_t, size_t> get_symbol_frequencies(const std::string& filename);


FILES_END
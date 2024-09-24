#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "_Core.h"
#include "Thread_Pool.h"
#include "Files.h"

#define HUFFMAN_BEGIN namespace huffman {
#define HUFFMAN_END }


HUFFMAN_BEGIN

namespace fs = std::filesystem;

void compress(const std::string& filename);
void decompress(const std::string& filename);

HUFFMAN_END
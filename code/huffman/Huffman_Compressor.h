#pragma once

#include <string>
#include <bitset>
#include <unordered_map>
#include <queue>

#include <iostream>
#include <fstream>
#include <filesystem>

#include "_Core.h"

#define HUFFMAN_BEGIN namespace huffman {
#define HUFFMAN_END }


HUFFMAN_BEGIN

void compress(const std::string& filename);
void decompress(const std::string& filename);

HUFFMAN_END
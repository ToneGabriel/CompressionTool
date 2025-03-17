#pragma once

#include <vector>
#include <fstream>
#include <filesystem>

#include "_Core.h"

namespace fs = std::filesystem;

// TODO: implement

class Archive
{
public:

    Archive();
    ~Archive();
};

// void Create_Archive(const std::string& dir_path)
// {
//     std::vector<fs::path> file_paths;

//     for (const auto& entry : fs::recursive_directory_iterator(dir_path))
//         if (fs::is_regular_file(entry))
//             file_paths.push_back(entry.path());

//     std::ofstream archive_stream(fs::path(dir_path).parent_path() / "archive.ar", std::ios::binary);

//     for (auto& path : file_paths)
//     {
//         std::ifstream path_stream(path.string(), std::ios::binary);

//         archive_stream << path.string().size();
//         archive_stream << path.string();
//         archive_stream << fs::file_size(path);
//         archive_stream << path_stream.rdbuf();
//     }
// }

// void Restore_Archive(const std::string& arc_path)
// {
//     std::ifstream archive_stream(fs::path(arc_path), std::ios::binary);

//     while (!archive_stream.eof())
//     {
//         size_t path_size;
//         archive_stream.read(reinterpret_cast<char*>(&path_size), sizeof(size_t));
        
//         std::string path;
//         path.reserve(path_size);
//         archive_stream.read(path.data(), path_size);
        
//         size_t file_size;
//         archive_stream.read(reinterpret_cast<char*>(&file_size), sizeof(size_t));

//         std::ofstream path_stream(path, std::ios::binary);
//         path_stream << archive_stream.rdbuf();
//     }
// }
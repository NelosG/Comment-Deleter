//
// Created by NelosG.
//

#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <filesystem>
#include <fstream>

namespace file_utils {
    std::fstream open_file(const std::string &filename, bool add);

    std::string create_temp_file();

    void reset(std::fstream &fstr);

    void to_end(std::fstream &fstr);

    bool delete_file(const std::string &filename, int count_parent_dirs_for_delete);

    bool delete_file(const std::string &filename);

    bool delete_temp_files(const std::string &dirname);
}// namespace file_utils

#endif//FILE_UTILS_H

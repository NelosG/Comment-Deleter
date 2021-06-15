//
// Created by NelosG.
//
#include <file_utils.h>
#include <iostream>
#include <random>

std::fstream file_utils::open_file(const std::string &filename, bool add) {
    std::filesystem::path p(filename);
    p = proximate(p);
    auto pp = p.parent_path();
    if(!pp.empty()) {
        std::filesystem::create_directories(pp);
    }

    std::fstream fstr;
    if (std::filesystem::exists(p)) {
        fstr.open(p, std::ios_base::out | std::ios_base::in | (add ? std::ios_base::app : 0));
    } else {
        fstr.open(p, std::ios_base::out | std::ios_base::in | std::ios_base::trunc);
    }
    return fstr;
}

std::string file_utils::create_temp_file() {
        std::string letters = "abcdefghijklmnopqrstuvwxyz0123456789";

        std::random_device                 rand_dev;
        std::mt19937                       generator(rand_dev());
        std::uniform_int_distribution<int> distr(0, letters.size() - 1);

        std::string result;
        for (int i = 0; i < 9; ++i) {
            result += letters[distr(generator)];
        }

        return result + ".tmp";
}

bool file_utils::delete_temp_files(const std::string &dirname) {
    std::filesystem::path p(dirname);
    if (!is_directory(p)) {
        return false;
    }
    std::string extension = ".tmp";
    bool res = true;
    for (auto &file : std::filesystem::directory_iterator(p, std::filesystem::directory_options::skip_permission_denied)) {
        try {
            if (std::filesystem::is_regular_file(file.status()) && !file.path().extension().compare(extension)) {
                std::filesystem::remove(file);
            }
        } catch (const std::filesystem::filesystem_error &e) {
            res = false;
        }
    }
    return res;
}


void file_utils::reset(std::fstream &fstr) {
    fstr.seekg(0);
}

void file_utils::to_end(std::fstream &fstr) {
    fstr.seekg(fstr.width() - 1);
}

bool file_utils::delete_file(const std::string &filename) {
    std::filesystem::path p(filename);
    p = proximate(p);
    return std::filesystem::remove(p);
}

bool file_utils::delete_file(const std::string &filename, int count_parent_dirs_for_delete) {
    std::filesystem::path p(filename);
    p = proximate(p);
    std::filesystem::path prev("../");
    for (int i = 0; i < count_parent_dirs_for_delete; ++i) {
        if (p.parent_path().empty() || equivalent(p.parent_path(), prev)) {
            break;
        }
        p = p.parent_path();
    }
    return std::filesystem::remove_all(p);
}


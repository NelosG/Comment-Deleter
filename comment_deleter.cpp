//
// Created by NelosG.
//

#include "comment_deleter.h"
#include <file_utils.h>
#include <fstream>

std::string find_end(std::istream &in) {
    std::string s;

    while (!in.eof()) {
        getline(in, s);
        size_t ind = s.find("*/");
        if (ind != std::string::npos) {
            return s.substr(ind + 2);
        }
    }
    return "";
}

size_t count(const std::string &s1, const std::string &s2, size_t start, size_t end) {
    size_t cnt = 0;
    bool flag;
    for (size_t i = start; i < end; ++i) {
        flag = true;
        if (i != start) {
            flag = s1[i - 1] != '\\';
        }
        if (flag && s1[i] == s2[0]) {
            if (s1.substr(i, s2.length()) == s2) {
                ++cnt;
                i += s2.length() - 1;
            }
        }
    }

    return cnt;
}

size_t check(const std::string &s, const std::string &what) {
    size_t ind = s.find(what);
    if (ind == std::string::npos) {
        return ind;
    }
    size_t cnt = count(s, "\"", 0, ind);
    if (cnt % 2 == 0) {
        return ind;
    }
    size_t add_ind = s.substr(ind + what.size()).find('\"') + 1;
    size_t temp = check(s.substr(ind + what.size() + add_ind), what);
    if (temp == std::string::npos) {
        return temp;
    }
    return add_ind + temp + ind + what.size();
}

std::string del_one(const std::string &s, std::istream &in, size_t max, bool start) {
    size_t ind = check(s, "/*");
    if (ind != std::string::npos && ind < max) {
        std::string res = s.substr(0, ind);
        std::string temp;
        temp = s.substr(ind);
        ind = temp.find("*/");
        if (ind != std::string::npos) {
            temp = temp.substr(ind + 2);
        } else {
            temp = find_end(in);
        }
        res += del_one(temp, in, check(temp, "//"), false);
        if (!res.empty() && res.find_first_not_of(" \t") != std::string::npos) {
            return res + (start ? "\n" : "");
        }
    } else {
        std::string temp = s.substr(0, max);
        if (!temp.empty() && temp.find_first_not_of(" \t") != std::string::npos) {
            return temp + (start ? "\n" : "");
        }
    }
    return "";
}

void filtered_write(std::ostream &out, std::istream &in) {
    std::string s;
    while (!in.eof()) {
        getline(in, s);
        if (s.empty() || s.find_first_not_of(" \t") == std::string::npos) {
            out << std::endl;
            continue;
        }
        if (count(s, "\"", 0, s.size()) % 2 != 0) {
            out << s << std::endl;
            getline(in, s);
            while (count(s, "\"", 0, s.size()) % 2 == 0) {
                out << s << std::endl;
                getline(in, s);
            }
            size_t ind = s.find('\"');
            std::string temp = s.substr(ind + 1);
            out << s.substr(0, ind + 1) << del_one(temp, in, check(temp, "//"), true);
        } else {
            out << del_one(s, in, check(s, "//"), true);
        }
    }
}

 void comment_deleter::run(const std::string& name) {
    std::string temp_name = file_utils::create_temp_file();
    std::filesystem::copy(name, temp_name);
    file_utils::delete_file(name);

    std::fstream in = file_utils::open_file(name, false);
    std::fstream temp = file_utils::open_file(temp_name, false);

    filtered_write(in, temp);

    in.close();

    temp.close();
    file_utils::delete_temp_files("./");
}
//
// Created by dumpling on 15.04.19.
//
#ifndef FIND_COMMON_UTILS_H
#define FIND_COMMON_UTILS_H

#include <iostream>
#include <cstring>

static void print_err(const std::string &message) {
    std::cerr << "\033[31m" << message;
    if (errno) {
        std::cerr << ": " << std::strerror(errno);
    }
    std::cerr << "\033[0m" << std::endl;
}

static std::string get_filename(const std::string &path) {
    int last = path.find_last_of('/');
    return path.substr(last + 1);
}

#endif //FIND_COMMON_UTILS_H

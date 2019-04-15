//
// Created by dumpling on 15.04.19.
//

#ifndef FIND_FILE_FILTER_H
#define FIND_FILE_FILTER_H

#include <vector>
#include "linux_dirent64.h"
#include "common_utils.h"
#include <sys/stat.h>

struct File_filter {

    bool init(int argc, char **argv);

    void apply(const std::string &file_path, const struct stat64 &st);


    ~File_filter();

private:
    std::vector<std::string> results;

    struct Inode_filter {
        ino64_t num;

        bool check(const struct stat64 &st);
    };

    Inode_filter *inode_filter = nullptr;


    struct Name_filter {
        std::string name;

        bool check(const std::string &file_name);
    };

    Name_filter *name_filter = nullptr;

    struct Size_filter {
        off64_t lower_bound = -1;
        off64_t upper_bound = -1;
        off64_t extract_bound = -1;

        bool check(const struct stat64 &st);

    };

    Size_filter *size_filter = nullptr;

    struct Links_filter {
        __nlink_t cnt;

        bool check(const struct stat64 &st);

    };

    Links_filter *links_filter = nullptr;

    struct Executer {
        std::string file_path;

        void run(const std::string &args);

    };

    Executer *executer = nullptr;


};


#endif //FIND_FILE_FILTER_H

//
// Created by dumpling on 15.04.19.
//

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstring>
#include "linux_dirent64.h"
#include "File_filter.h"
#include "common_utils.h"

#include <sys/syscall.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>


static const int BUF_SIZE = 4096;

void file_processing(int dir_fd, const std::string &file_path, File_filter &filter, bool dir = false) {
    struct stat64 sb{};

    if (fstatat64(dir_fd, dir ? "." : get_filename(file_path).c_str(), &sb, AT_SYMLINK_NOFOLLOW) == -1) {
        //print_err("Can't stat file/dir");
    } else {
        filter.apply(file_path, sb);
    }

}


void dir_processing(int fd, const std::string &real_path, File_filter &filter) {

    int nread;

    char buf[BUF_SIZE];
    struct linux_dirent64 *d;

    while (true) {
        nread = syscall(SYS_getdents64, fd, buf, BUF_SIZE);
        if (nread == -1) {
            print_err("Fatal error");
        }

        if (nread == 0) {
            break;
        }

        for (int cur_pos = 0; cur_pos < nread;) {
            d = (struct linux_dirent64 *) (buf + cur_pos);
            char d_type = d->d_type;
            std::string name = d->d_name;
            cur_pos += d->d_reclen;

            if (name == "..") {
                continue;
            }

            if (name == ".") {
                file_processing(fd, real_path, filter, true);
                continue;
            }


            std::string new_path = real_path;
            new_path += "/" + name;

            if (d_type == DT_DIR) {
                int child_fd = openat(fd, name.c_str(), O_RDONLY | O_DIRECTORY);

                if (child_fd == -1) {
                    print_err("Can't open directory");
                    continue;
                }

                dir_processing(child_fd, new_path, filter);
            } else {
                file_processing(fd, new_path, filter);
            }
        }

    }
}


int main(int argc, char **argv) {
    if (argc <= 1) {
        print_err("Directory expected");
        return EXIT_FAILURE;
    }

    int fd;

    fd = open(argv[1], O_RDONLY | O_DIRECTORY);
    if (fd == -1) {
        print_err(std::string("Cant't open ") + std::string(argv[1]));
    }

    File_filter filter;
    if (!filter.init(argc, argv)) {
        return 0;
    }

    dir_processing(fd, std::string(argv[1]), filter);

    filter.out();
}
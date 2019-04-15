//
// Created by dumpling on 15.04.19.
//

#include "File_filter.h"

bool File_filter::Inode_filter::check(const struct stat64 &st) {
    return st.st_ino == this->num;
}

bool File_filter::Name_filter::check(const std::string &file_name) {
    return file_name == this->name;
}

bool File_filter::Size_filter::check(const struct stat64 &st) {

    if (this->extract_bound != -1) {
        return st.st_size == extract_bound;
    } else {
        if (st.st_size <= lower_bound) {
            return false;
        } else {
            return st.st_size < upper_bound;
        }
    }
}

bool File_filter::Links_filter::check(const struct stat64 &st) {
    return st.st_nlink == this->cnt;
}

bool File_filter::Executer::run(const std::vector<std::string> &args) {
    for (auto &e : args) {
        file_path += e;
    }
    return (std::system(this->file_path.c_str()));
}

bool File_filter::init(int argc, char **argv) {
    for (int i = 2; i < argc; ++i) {
        std::string key = argv[i];
        ++i;

        if (i == argc) {
            print_err("There is no argument for " + key);
            return false;
        }
        std::string arg = argv[i];

        if (key == "-inum") {
            try {
                if (inode_filter == nullptr) {
                    inode_filter = new Inode_filter;
                }
                inode_filter->num = std::stoull(arg);
            } catch (...) {
                print_err(arg + " is not a positive integer number");
                return false;
            }
        } else if (key == "-name") {
            if (name_filter == nullptr) {
                name_filter = new Name_filter;
            }
            name_filter->name = arg;
        } else if (key == "-size") {
            try {
                if (size_filter == nullptr) {
                    size_filter = new Size_filter;
                }

                std::string real_arg = arg.substr(1);
                if (arg[0] == '=') {
                    size_filter->extract_bound = std::stoull(real_arg);
                } else if (arg[0] == '+') {
                    size_filter->lower_bound = std::stoull(real_arg);
                } else if (arg[0] == '-') {
                    size_filter->upper_bound = std::stoull(real_arg);
                } else {
                    print_err(std::string({arg[0]}) + " is wrong size parameter");
                    return false;
                }
            } catch (...) {
                print_err(arg.substr(1) + " is not a positive integer number");
                return false;
            }
        } else if (key == "-nlinks") {
            try {
                if (links_filter == nullptr) {
                    links_filter = new Links_filter;
                }

                links_filter->cnt = std::stoull(arg);
            } catch (...) {
                print_err(arg + " is not a positive integer number");
                return false;
            }
        } else if (key == "-exec") {
            if (executer == nullptr) {
                executer = new Executer;
            }

            executer->file_path = arg;
        } else {
            print_err(key + " is not a valid argument key");
            return false;
        }
    }

    return true;
}

File_filter::~File_filter() {
    delete inode_filter;
    delete name_filter;
    delete size_filter;
    delete links_filter;
    delete executer;
}

void File_filter::apply(const std::string &file_path, const struct stat64 &st) {

    bool result = true;

    if (inode_filter != nullptr) {
        result &= inode_filter->check(st);
    }

    if (result && name_filter != nullptr) {
        result &= name_filter->check(get_filename(file_path));
    }

    if (result && size_filter != nullptr) {
        result &= size_filter->check(st);
    }

    if (result && links_filter != nullptr) {
        result &= links_filter->check(st);
    }

    if (result) {
        this->results.push_back(file_path);
    }
}

void File_filter::out() {
    if (executer == nullptr) {
        for (auto &e : results) {
            std::cout << e << std::endl;
        }
    } else {
        executer->run(results);
    }
}

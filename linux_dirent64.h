//
// Created by dumpling on 15.04.19.
//

#ifndef FIND_LINUX_DIRENT64_H
#define FIND_LINUX_DIRENT64_H


#include <sys/param.h>

struct linux_dirent64 {
    ino64_t d_ino;    /* 64-bit inode number */
    off64_t d_off;    /* 64-bit offset to next structure */
    unsigned short d_reclen; /* Size of this dirent */
    unsigned char d_type;   /* File type */
    char d_name[]; /* Filename (null-terminated) */
};


#endif //FIND_LINUX_DIRENT64_H

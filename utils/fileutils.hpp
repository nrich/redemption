/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   Product name: redemption, a FLOSS RDP proxy
   Copyright (C) Wallix 2013
   Author(s): Christophe Grosjean

   File related utility functions

*/

#ifndef _REDEMPTION_UTILS_FILEUTILS_HPP_
#define _REDEMPTION_UTILS_FILEUTILS_HPP_


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stddef.h>
#include "log.hpp"

static inline int filesize(const char * path)
{
    struct stat sb;
    int status = stat(path, &sb);
    if (status >= 0){
        return sb.st_size;
    }
    return -1;
}

static inline void canonical_path(const char * fullpath, char * path, size_t path_len, char * basename, size_t basename_len, char * extension, size_t extension_len)
{
    TODO("add parameters values for default path and basename. From inifile ?")
    TODO("add extraction of extension")
    TODO("add overflow checking of path and basename len")
    const char * end_of_path = strrchr(fullpath, '/');
    if (end_of_path){
        memcpy(path, fullpath, end_of_path + 1 - fullpath);
        path[end_of_path + 1 - fullpath] = 0;
        const char * start_of_extension = strrchr(end_of_path + 1, '.');
        if (start_of_extension){
            strcpy(extension, start_of_extension); 
            memcpy(basename, end_of_path + 1, start_of_extension - end_of_path - 1);
            basename[start_of_extension - end_of_path - 1] = 0;
        }
        else {
            if (end_of_path[0]){
                strcpy(basename, end_of_path + 1);
                strcpy(extension, "no extension");
            }
            else {
              strcpy(basename, "no_name");
              strcpy(extension, "no extension");
            }
        }
    }
    else {
        strcpy(path, "./");
        const char * start_of_extension = strrchr(fullpath, '.');
        if (start_of_extension){
            strcpy(extension, start_of_extension); 
            memcpy(basename, fullpath, start_of_extension - fullpath);
            basename[start_of_extension - fullpath] = 0;
        }
        else {
            if (fullpath[0]){
                strcpy(basename, fullpath);
                strcpy(extension, "no extension");
            }
            else {
                strcpy(basename, "no_name");
                strcpy(extension, "no extension");
            }
        }
    }
    LOG(LOG_INFO, "canonical_path : %s%s%s\n", path, basename, extension);
}

void clear_files_flv_meta_png(const char * path, const char * prefix)
{
    DIR * d = opendir(path);
    if (d){
        size_t path_len = strlen(path);
        size_t prefix_len = strlen(prefix);
        size_t file_len = pathconf(path, _PC_NAME_MAX) + 1;
        char * buffer = (char*)malloc(file_len + path_len + 1);
        strcpy(buffer, path);
        if (buffer[path_len] != '/'){
            buffer[path_len] = '/'; path_len++; buffer[path_len] = 0;
        }

        size_t len = offsetof(struct dirent, d_name) + file_len;
        struct dirent * entryp = (struct dirent *)malloc(len);
        struct dirent * result;
        for (readdir_r(d, entryp, &result) ; result ; readdir_r(d, entryp, &result)) {
            if ((0 == strcmp(entryp->d_name, ".")) || (0 == strcmp(entryp->d_name, ".."))){
                continue;
            }

            if (strncmp(entryp->d_name, prefix, prefix_len)){
                continue;
            }

            strcpy(buffer + path_len, entryp->d_name);
            const char * eob = buffer + path_len + strlen(entryp->d_name);
            const bool extension = ((eob[-4] == '.') && (eob[-3] == 'f') && (eob[-2] == 'l') && (eob[-1] == 'v'))
                          || ((eob[-4] == '.') && (eob[-3] == 'p') && (eob[-2] == 'n') && (eob[-1] == 'g'))
                          || ((eob[-5] == '.') && (eob[-4] == 'm') && (eob[-3] == 'e') && (eob[-2] == 't') && (eob[-1] == 'a'))
                          ;

            if (!extension){
                continue;
            }

            struct stat st;
            if (stat(buffer, &st) < 0){
                LOG(LOG_WARNING, "Failed to read file %s [%u: %s]\n", buffer, errno, strerror(errno));
                continue;
            }
            if (unlink(buffer) < 0){
                LOG(LOG_WARNING, "Failed to remove file %s", buffer, errno, strerror(errno));
            }
        }
        closedir(d);
        free(entryp);
        free(buffer);
    }
    else {
        LOG(LOG_WARNING, "Failed to open directory %s [%u: %s]", path, errno, strerror(errno));
    }
}


#endif
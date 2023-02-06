
#include "common.h"

FileBuf* openFile(const char* name, const char* mode) {

    FILE* fp = fopen(name, mode);
    if(fp == NULL) {
        fprintf(stderr, "fatal error, cannot open file \"%s\": %s\n",
            name, strerror(errno));
        exit(1);
    }

    FileBuf* fb = _alloc_ds(FileBuf);
    fb->name = _copy_str(name);
    fb->fp = fp;

    return fb;
}

FileBuf* openReadFile(const char* name) {

    return openFile(name, "r");
}

FileBuf* openWriteFile(const char* name) {

    return openFile(name, "w");
}

void closeFile(FileBuf* buf) {

    if(buf != NULL) {
        if(fclose(buf->fp) != 0) {
            fprintf(stderr, "fatal error, cannot close file \"%s\": %s\n",
                buf->name, strerror(errno));
            exit(1);
        }

        _free(buf->name);
        _free(buf);
    }
    else {
        fprintf(stderr, "fatal error, invalid file buffer to close\n");
        exit(1);
    }
}

void readFile(FileBuf* buf, void* data, size_t sz) {

    if(buf != NULL) {
        size_t rv = fread(data, sizeof(char), sz, buf->fp);
        if(rv != sz) {
            fprintf(stderr, "fatal error, error reading file \"%s\": %s\n",
                buf->name, strerror(errno));
            closeFile(buf);
            exit(1);
        }
    }
    else {
        fprintf(stderr, "fatal error, invalid file buffer to read\n");
        exit(1);
    }
}

void writeFile(FileBuf* buf, void* data, size_t sz) {

    if(buf != NULL) {
        size_t rv = fwrite(data, sizeof(char), sz, buf->fp);
        if(rv != sz) {
            fprintf(stderr, "fatal error, error writing file \"%s\": %s\n",
                buf->name, strerror(errno));
            closeFile(buf);
            exit(1);
        }
    }
    else {
        fprintf(stderr, "fatal error, invalid file buffer to write\n");
        exit(1);
    }
}



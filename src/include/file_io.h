#ifndef _FILE_IO_H_
#define _FILE_IO_H_

#include <stdio.h>
struct _file_buffer_ {
    const char* name;
    FILE* fp;
};

FileBuf* openFile(const char* name, const char* mode);
FileBuf* openReadFile(const char* name);
FileBuf* openWriteFile(const char* name);
void closeFile(FileBuf* buf);
void readFile(FileBuf* buf, void* data, size_t sz);
void writeFile(FileBuf* buf, void* data, size_t sz);

#endif

#ifndef _MODULE_H_
#define _MODULE_H_

typedef struct _module_ Module;

struct _module_ {
    ValueTab* valTab;
    InstrBuf* instrBuf;
};

Module* createModule();
void destroyModule(Module* ptr);

void saveModule(const char* fname, Module* ptr);
Module* loadModule(const char* fname);

#endif


#include "common.h"

Module* createModule() {

    Module* ptr = _alloc_ds(Module);

    ptr->valTab = createValueTab();
    ptr->instrBuf = createInstrBuf();

    return ptr;
}

void destroyModule(Module* ptr) {

    destroyValueTab(ptr->valTab);
    destroyInstrBuf(ptr->instrBuf);

    _free(ptr);
}

void saveModule(const char* fname, Module* ptr) {

    FileBuf* fp = openWriteFile(fname);

    saveValueTab(ptr->valTab, fp);
    saveInstrBuf(ptr->instrBuf, fp);

    closeFile(fp);
}

Module* loadModule(const char* fname) {

    FileBuf* fp = openReadFile(fname);

    Module* ptr = _alloc_ds(Module);

    ptr->valTab = loadValueTab(fp);
    ptr->instrBuf = loadInstrBuf(fp);

    closeFile(fp);
    return ptr;
}


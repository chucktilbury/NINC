
#include <assert.h>
#include "common.h"

InstrBuf* createInstrBuf() {

    InstrBuf* buf = _alloc_ds(InstrBuf);
    buf->cap = 1 << 3;
    buf->idx = 0;
    buf->len = 0;
    buf->buffer = _alloc(buf->cap);

    return buf;
}

void destroyInstrBuf(InstrBuf* ptr) {

    assert(ptr != NULL);
    _free(ptr->buffer);
    _free(ptr);
}

// write to the end of the instruction buffer.
InstrBufResult writeInstrBuf(InstrBuf* ptr, void* data, size_t size) {

    assert(ptr != NULL);
    assert(data != NULL);

    if(ptr->len+size+1 > ptr->cap) {
        while(ptr->len+size+1 > ptr->cap)
            ptr->cap <<= 1;
        ptr->buffer = _realloc(ptr->buffer, ptr->cap);
    }

    memcpy(&ptr->buffer[ptr->len], data, size);
    ptr->len += size;

    return INSTR_BUF_OK;
}

// write to the end of the instruction buffer.
InstrBufResult updateInstrBuf(InstrBuf* ptr, size_t idx, void* data, size_t size) {

    assert(ptr != NULL);
    assert(data != NULL);

    if(idx+size < ptr->len) {
        memcpy(&ptr->buffer[idx], data, size);
        return INSTR_BUF_OK;
    }

    return INSTR_BUF_ERROR;
}

InstrBufResult readInstrBuf(InstrBuf* ptr, void* data, size_t size) {

    assert(ptr != NULL);
    assert(data != NULL);

    if(ptr->idx+size <= ptr->len) {
        memcpy(data, &ptr->buffer[ptr->idx], size);
        ptr->idx += size;
        return INSTR_BUF_OK;
    }

    return INSTR_BUF_ERROR;
}

size_t getInstrBufIdx(InstrBuf* ptr) {

    assert(ptr != NULL);
    return ptr->idx;
}

// This returns the write location for setting the location of labels.
size_t getInstrBufLoc(InstrBuf* ptr) {

    assert(ptr != NULL);
    return ptr->len;
}

// This sets the current execution location for implementing jump instructions.
InstrBufResult setInstrBufLoc(InstrBuf* ptr, size_t loc) {

    assert(ptr != NULL);

    if(loc <= ptr->len) {
        ptr->idx = loc;
        return INSTR_BUF_OK;
    }

    return INSTR_BUF_ERROR;
}

InstrBuf* loadInstrBuf(FileBuf* fp) {

    InstrBuf* ptr = createInstrBuf();
    size_t len;

    readFile(fp, &len, sizeof(len));

    if(len > ptr->cap) {
        while(len > ptr->cap)
            ptr->cap <<= 1;
        ptr->buffer = _realloc(ptr->buffer, ptr->cap);
    }

    readFile(fp, ptr->buffer, len);
    ptr->len = len;

    return ptr;
}

InstrBufResult saveInstrBuf(InstrBuf* ptr, FileBuf* fp) {

    assert(ptr != NULL);

    writeFile(fp, &ptr->len, sizeof(ptr->len));
    writeFile(fp, ptr->buffer, ptr->len);

    return INSTR_BUF_OK;
}


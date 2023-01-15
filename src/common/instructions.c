
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

InstrBufResult loadInstrBuf(InstrBuf* ptr, FILE* fp) {

    assert(ptr != NULL);
    size_t len;
    size_t result;

    result = fread(&len, sizeof(len), 1, fp);
    if(result != 1)
        return INSTR_BUF_ERROR;

    if(len > ptr->cap) {
        while(len > ptr->cap)
            ptr->cap <<= 1;
        ptr->buffer = _realloc(ptr->buffer, ptr->cap);
    }

    result = fread(ptr->buffer, sizeof(char), len, fp);
    if(result != len)
        return INSTR_BUF_ERROR;
    ptr->len = len;

    return INSTR_BUF_OK;
}

InstrBufResult saveInstrBuf(InstrBuf* ptr, FILE* fp) {

    assert(ptr != NULL);
    size_t result;

    result = fwrite(&ptr->len, sizeof(ptr->len), 1, fp);
    if(result != 1)
        return INSTR_BUF_ERROR;

    result = fwrite(ptr->buffer, sizeof(char), ptr->len, fp);
    if(result != ptr->len)
        return INSTR_BUF_ERROR;

    return INSTR_BUF_OK;
}


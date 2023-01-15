#ifndef _INSTRUCTIONS_H_
#define _INSTRUCTIONS_H_

typedef enum {
    INSTR_BUF_OK,
    INSTR_BUF_ERROR,
} InstrBufResult;


#include <stddef.h>
struct _instr_buf_ {
    uint8_t* buffer;
    size_t cap;
    size_t len;
    size_t idx;
};

InstrBuf* createInstrBuf();
void destroyInstrBuf(InstrBuf* ptr);
InstrBufResult writeInstrBuf(InstrBuf* ptr, void* data, size_t size);
InstrBufResult updateInstrBuf(InstrBuf* ptr, size_t idx, void* data, size_t size);
InstrBufResult readInstrBuf(InstrBuf* ptr, void* data, size_t size);
size_t getInstrBufLoc(InstrBuf* ptr);
size_t getInstrBufIdx(InstrBuf* ptr);
InstrBufResult setInstrBufLoc(InstrBuf* ptr, size_t loc);

#include <stdio.h>
InstrBufResult loadInstrBuf(InstrBuf* ptr, FILE* fp);
InstrBufResult saveInstrBuf(InstrBuf* ptr, FILE* fp);

#endif

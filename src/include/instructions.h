#ifndef _INSTRUCTIONS_H_
#define _INSTRUCTIONS_H_

typedef enum {
    INSTR_BUF_OK,
    INSTR_BUF_ERROR,
} InstrBufResult;

#include <stddef.h>
typedef struct {
    char* buffer;
    size_t cap;
    size_t len;
    size_t idx;
} InstrBuf;

InstrBuf* createInstrBuf();
void destroyInstrBuf(InstrBuf* ptr);
InstrBufResult writeInstrBuf(InstrBuf* ptr, void* data, size_t size);
InstrBufResult readInstrBuf(InstrBuf* ptr, void* data, size_t size);
size_t getInstrBufLoc(InstrBuf* ptr);
InstrBufResult setInstrBufLoc(InstrBuf* ptr, size_t loc);

#include <stdio.h>
InstrBufResult loadInstrBuf(InstrBuf* ptr, FILE* fp);
InstrBufResult saveInstrBuf(InstrBuf* ptr, FILE* fp);

#endif

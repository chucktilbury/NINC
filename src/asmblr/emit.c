
#include "common.h"

void emit_instr(InstrBuf* buf, uint8_t val) {

    writeInstrBuf(buf, &val, sizeof(uint8_t));
}

void emit_1_reg(InstrBuf* buf, uint8_t reg) {

    writeInstrBuf(buf, &reg, sizeof(uint8_t));
}

void emit_2_reg(InstrBuf* buf, uint8_t r1, uint8_t r2) {

    uint8_t r = (r1 & 0xF) | ((r2 & 0xF)<< 4);
    writeInstrBuf(buf, &r, sizeof(uint8_t));
}

void emit_3_reg(InstrBuf* buf, uint8_t r1, uint8_t r2, uint8_t r3) {

    uint16_t val = (r1&0x0F)|((r2&0x0F)<<4)|((r3&0x0F)<<8);
    writeInstrBuf(buf, &val, sizeof(uint16_t));
}

void emit_addr(InstrBuf* buf, uint32_t val) {

    writeInstrBuf(buf, &val, sizeof(uint32_t));
}

void emit_val(InstrBuf* buf, uint16_t val) {

    writeInstrBuf(buf, &val, sizeof(uint16_t));
}


#ifndef _EMIT_H_
#define _EMIT_H_

void emit_instr(InstrBuf* buf, uint8_t val);
void emit_1_reg(InstrBuf* buf, uint8_t reg);
void emit_2_reg(InstrBuf* buf, uint8_t r1, uint8_t r2);
void emit_3_reg(InstrBuf* buf, uint8_t r1, uint8_t r2, uint8_t r3);
void emit_addr(InstrBuf* buf, uint32_t val);
void emit_val(InstrBuf* buf, uint16_t val);

#endif

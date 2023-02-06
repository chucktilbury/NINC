
#include "common.h"
#include "asmblr.h"
#include "parser.h"
#include "scanner.h"
#include "address.h"

void disasm(Assembler* ptr) {

    InstrBuf* ibuf = ptr->instrBuf;

    uint8_t op;
    size_t idx = 0;
    int line = 1;
    const char* name = NULL;

    do {
        idx = getInstrBufIdx(ibuf);
        name = lookupAddrRefName(ptr->addrDefs, idx);
        if(name != NULL)
            printf("%d.\t\t%s:\t; defined at address %04lu\n", line++, name, idx);
        readInstrBuf(ibuf, &op, sizeof(op));
        printf("%d.\t%04u\t%s ", line++, (uint32_t)idx, opToStr(op));
        switch(op) {
            // no args
            case OP_EXIT:
            case OP_NOP:
            case OP_BREAK:
            case OP_RETURN:
                printf("\n");
                break;
            // one register
            case OP_PUSH:
            case OP_POP:
            case OP_SIDX: {
                    uint8_t val;
                    readInstrBuf(ibuf, &val, sizeof(val));
                    printf("%s\n", regToStr(val&0xF));
                }
                break;
            // two registers
            case OP_NOT:
            case OP_EQ:
            case OP_NEQ:
            case OP_LTE:
            case OP_GTE:
            case OP_LT:
            case OP_GT:
            case OP_NEG: {
                    uint8_t val;
                    readInstrBuf(ibuf, &val, sizeof(val));
                    printf("%s, %s\n", regToStr(val&0x0F), regToStr((val&0xF0)>>4));
                }
                break;
            // three registers
            case OP_ADD:
            case OP_SUB:
            case OP_MUL:
            case OP_DIV:
            case OP_MOD: {
                    uint16_t val;
                    readInstrBuf(ibuf, &val, sizeof(val));
                    printf("%s, %s, %s\n",
                           regToStr(val&0x0F),
                           regToStr((val&0xF0)>>4),
                           regToStr((val&0xF00)>>8));
                }
                break;
            // one 16 bit word
            case OP_TRAP: {
                    uint16_t val;
                    readInstrBuf(ibuf, &val, sizeof(val));
                    printf("%u\n", val);
                }
                break;
            // value index
            case OP_PUSHS: {
                    uint16_t val;
                    readInstrBuf(ibuf, &val, sizeof(val));
                    Value* vptr = getValueByIndex(ptr->valTab, val);
                    printf("%s\t; index number %d\n", vptr->name, val);
                }
                break;
            // two registers and one 16 bit word
            case OP_PEEK: {
                    uint8_t regs;
                    uint16_t num;
                    readInstrBuf(ibuf, &regs, sizeof(regs));
                    readInstrBuf(ibuf, &num, sizeof(num));
                    printf("%s, %s, %u\n",
                           regToStr(regs&0x0F), regToStr((regs&0xF0)>>4), num);
                }
                break;
            // one register, one var
            case OP_LOAD: {
                    uint8_t regs;
                    uint16_t num;
                    readInstrBuf(ibuf, &regs, sizeof(regs));
                    readInstrBuf(ibuf, &num, sizeof(num));
                    Value* vptr = getValueByIndex(ptr->valTab, num);
                    printf("%s, %s\t; index number %d\n",
                           regToStr(regs&0x0F), vptr->name, num);
                }
                break;
            case OP_LOADL: {
                    uint8_t regs;
                    uint16_t num;
                    readInstrBuf(ibuf, &regs, sizeof(regs));
                    readInstrBuf(ibuf, &num, sizeof(num));
                    Value* vptr = getValueByIndex(ptr->valTab, num);
                    printf("%s, anonymous\t; index number ", regToStr(regs&0x0F));
                    dumpValue(num, vptr);
                }
                break;
            case OP_LOADR: {
                    uint8_t regs;
                    readInstrBuf(ibuf, &regs, sizeof(regs));
                    printf("%s, %s\n", regToStr(regs&0x0F), regToStr((regs&0xF0)>>4));
                }
                break;
            // one var, one register
            case OP_STORE: {
                    uint8_t regs;
                    uint16_t num;
                    readInstrBuf(ibuf, &num, sizeof(num));
                    readInstrBuf(ibuf, &regs, sizeof(regs));
                    Value* vptr = getValueByIndex(ptr->valTab, num);
                    printf("%s, %s\t; index number %d\n",
                           vptr->name, regToStr(regs&0x0F), num);
                }
                break;
            // one 32 bit word
            case OP_JMP:
            case OP_BR:
            case OP_CALL: {
                    uint32_t val;
                    readInstrBuf(ibuf, &val, sizeof(val));
                    name = lookupAddrRefName(ptr->addrDefs, val);
                    printf("%s\t; refers to address %04u\n", name, val);
                }
                break;
            default:
                fatal_error("fatal error: unknown opcode: 0x%02X\n", op);
        }
    } while(ibuf->idx < ibuf->len);

}



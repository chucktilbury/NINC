
#include "common.h"
#include "asmblr.h"
#include "parser.h"
#include "scanner.h"
#include "address.h"

void disasm(Assembler* ptr) {

    InstrBuf* ibuf = ptr->instrBuf;

    int line = 1;
    do {
        uint8_t op;
        size_t idx = getInstrBufIdx(ibuf);
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
                    printf("\n");
                }
                break;
            // two registers and one 16 bit word
            case OP_PEEK: {
                    printf("\n");
                }
                break;
            // one register, one var
            case OP_LOAD: {
                    printf("\n");
                }
                break;
            // one var, one register
            case OP_STORE: {
                    printf("\n");
                }
                break;
            // one 32 bit word
            case OP_JMP:
            case OP_BR:
            case OP_CALL: {
                    uint32_t val;
                    readInstrBuf(ibuf, &val, sizeof(val));
                    const char* name = lookupAddrName(ptr->addrDefs, val);
                    printf("%s\t;  %04u\n", name, val);
                }
                break;
            default:
                fprintf(stderr, "fatal error: unknown opcode: 0x%02X\n", op);
                exit(1);
        }
    } while(ibuf->idx < ibuf->len);

}



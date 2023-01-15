
#include <stdint.h>
#include "opcodes.h"

const char* opToStr(OpCode op)
{
    return (op == OP_EXIT)? "EXIT":
            (op == OP_NOP)? "NOP":
            (op == OP_BREAK)? "BREAK":
            (op == OP_CALL)? "CALL":
            (op == OP_TRAP)? "TRAP":
            (op == OP_RETURN)? "RETURN":
            (op == OP_JMP)? "JMP":
            (op == OP_BR)? "BR":
            (op == OP_PUSH)? "PUSH":
            (op == OP_PUSHS)? "PUSH":
            (op == OP_POP)? "POP":
            (op == OP_PEEK)? "PEEK":
            (op == OP_SIDX)? "SIDX":
            (op == OP_LOAD)? "LOAD":
            (op == OP_LOADL)? "LOAD":
            (op == OP_LOADR)? "LOAD":
            (op == OP_STORE)? "STORE":
            (op == OP_NOT)? "NOT":
            (op == OP_EQ)? "EQ":
            (op == OP_NEQ)? "NEQ":
            (op == OP_LTE)? "LTE":
            (op == OP_GTE)? "GTE":
            (op == OP_LT)? "LT":
            (op == OP_GT)? "GT":
            (op == OP_NEG)? "NEG":
            (op == OP_ADD)? "ADD":
            (op == OP_SUB)? "SUB":
            (op == OP_MUL)? "MUL":
            (op == OP_DIV)? "DIV":
            (op == OP_MOD)? "MOD": "UNKNOWN";
}

#include <stdio.h>
int instrSize(OpCode op) {

    int retv = 0;

    switch(op) {
        case OP_EXIT:   { retv = OP_INST_SZ; } break;
        case OP_NOP:    { retv = OP_INST_SZ; } break;
        case OP_BREAK:  { retv = OP_INST_SZ; } break;
        case OP_RETURN: { retv = OP_INST_SZ; } break;

        case OP_CALL:   { retv = OP_INST_SZ+OP_ADDR_SZ; } break;
        case OP_JMP:    { retv = OP_INST_SZ+OP_ADDR_SZ; } break;
        case OP_BR:     { retv = OP_INST_SZ+OP_ADDR_SZ; } break;

        case OP_PUSH:   { retv = OP_INST_SZ+OP_1REG_SZ; } break;
        case OP_POP:    { retv = OP_INST_SZ+OP_1REG_SZ; } break;
        case OP_SIDX:   { retv = OP_INST_SZ+OP_1REG_SZ; } break;

        case OP_NOT:    { retv = OP_INST_SZ+OP_2REG_SZ; } break;
        case OP_EQ:     { retv = OP_INST_SZ+OP_2REG_SZ; } break;
        case OP_NEQ:    { retv = OP_INST_SZ+OP_2REG_SZ; } break;
        case OP_LTE:    { retv = OP_INST_SZ+OP_2REG_SZ; } break;
        case OP_GTE:    { retv = OP_INST_SZ+OP_2REG_SZ; } break;
        case OP_LT:     { retv = OP_INST_SZ+OP_2REG_SZ; } break;
        case OP_GT:     { retv = OP_INST_SZ+OP_2REG_SZ; } break;
        case OP_NEG:    { retv = OP_INST_SZ+OP_2REG_SZ; } break;

        case OP_ADD:    { retv = OP_INST_SZ+OP_3REG_SZ; } break;
        case OP_SUB:    { retv = OP_INST_SZ+OP_3REG_SZ; } break;
        case OP_MUL:    { retv = OP_INST_SZ+OP_3REG_SZ; } break;
        case OP_DIV:    { retv = OP_INST_SZ+OP_3REG_SZ; } break;
        case OP_MOD:    { retv = OP_INST_SZ+OP_3REG_SZ; } break;

        case OP_TRAP:   { retv = OP_INST_SZ+OP_VAL_SZ; } break;
        case OP_PEEK:   { retv = OP_INST_SZ+OP_2REG_SZ+OP_VAL_SZ; } break;
        case OP_LOAD:   { retv = OP_INST_SZ+OP_1REG_SZ+OP_VAL_SZ; } break;
        case OP_LOADL:  { retv = OP_INST_SZ+OP_1REG_SZ+OP_VAL_SZ; } break;
        case OP_LOADR:  { retv = OP_INST_SZ+OP_2REG_SZ; } break;
        case OP_STORE:  { retv = OP_INST_SZ+OP_VAL_SZ+OP_1REG_SZ; } break;

        default:
            fprintf(stderr, "fatal error: unknown opcode: 0x%02X\n", op);
            retv = -1;
    }

    return retv;
}


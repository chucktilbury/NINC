#ifndef _ASMBLR_H_
#define _ASMBLR_H_

#include "common.h"

typedef struct _assembler_context_ Assembler;
typedef struct _addr_ref_tab_ AddrRefTab;
typedef struct _addr_ref_ AddrRef;

#include "address.h"
#include "emit.h"

struct _assembler_context_ {
    Module* module;
    HashTab* symbols;
    AddrRefTab* addrRefs;
    AddrDefTab* addrDefs;
};

void syntax_error(const char* fmt, ...);
#define fatal_error(f, ...) __ferror(__func__, __LINE__, (f), ## __VA_ARGS__)
void __ferror(const char* func, int line, const char* fmt, ...);
#endif

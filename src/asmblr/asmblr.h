#ifndef _ASMBLR_H_
#define _ASMBLR_H_

#include "common.h"

typedef struct _assembler_ Assembler;
typedef struct _addr_ref_tab_ AddrRefTab;
typedef struct _addr_ref_ AddrRef;

#include "address.h"
#include "emit.h"

struct _assembler_ {
    ValueStore* valStore;
    InstrBuf* instrBuf;
    HashTab* symbols;
    AddrRefTab* addrRefs;
    AddrDefTab* addrDefs;
};

void syntax_error(const char* fmt, ...);

#endif

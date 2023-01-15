#ifndef _COMMON_H_
#define _COMMON_H_

#include "system.h"

typedef struct _value_ Value;
typedef struct _value_store_ ValueStore;
typedef struct _ptr_lst_ PtrLst;
typedef struct _instr_buf_ InstrBuf;
typedef struct _hash_tab_ HashTab;

#include "memory.h"
#include "ptrlst.h"
#include "hash_table.h"
#include "registers.h"
#include "opcodes.h"
#include "instructions.h"
#include "values.h"

#endif

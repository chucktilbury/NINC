#ifndef _VALUES_H_
#define _VALUES_H_

typedef enum {
    VAL_STORE_OK,
    VAL_STORE_ERROR,
} ValStoreResult;

typedef enum {
    VAL_NUM,
    VAL_STR,
    VAL_BVAL,
    VAL_ADDR,
    //VAL_USER,
} ValueType;

struct _value_ {
    ValueType type;
    const char* name;
    union {
        double num;
        char* str;
        bool bval;
        uint32_t addr;
        //void* user;
    } data;
    bool is_assigned;
    bool is_const;
    int idx;
};

#include <stddef.h>
struct _value_table_ {
    HashTab* index;
    PtrLst* list;
//     Value** lst;
//     int cap;
//     int len;
};


Value* createValue(ValueType type, const char* name);

void assignNumVal(Value* value, double val);
void assignStrVal(Value* value, const char* val);
void assignBoolVal(Value* value, bool val);
void assignAddrVal(Value* value, uint32_t val);
void assignValName(Value* val, const char* name);

void destroyValue(Value* val);
ValueType getValType(Value* val);

ValueTab* createValueTab();
void destroyValueTab(ValueTab* vs);

Value* getValueByIndex(ValueTab* tab, int idx);
Value* getValueByName(ValueTab* tab, const char* name);
int addValue(ValueTab* tab, Value* val);

void dumpValueTab(ValueTab* tab);
void dumpValue(Value* val);

#include <stdio.h>
ValueTab* loadValueTab(FileBuf* fp);
void saveValueTab(ValueTab* tab, FileBuf* fp);

#endif

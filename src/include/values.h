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
} ValueType;

struct _value_ {
    ValueType type;
    union {
        double num;
        char* str;
        bool bval;
    } data;
    bool is_assigned;
    bool is_const;
};

#include <stddef.h>
struct _value_store_ {
    Value** lst;
    size_t cap;
    size_t len;
};

Value* createValue(ValueType type);
void assignNumVal(Value* value, double val);
void assignStrVal(Value* value, const char* val);
void assignBoolVal(Value* value, bool val);
void destroyValue(Value* val);
ValueType getValType(Value* val);

ValueStore* createValueStore();
void destroyValueStore(ValueStore* vs);

Value* getValue(ValueStore* store, size_t idx);
size_t addValue(ValueStore* store, Value* val);

//void createNames();
//Value* getValueName(ValueStore* store, const char* name);
//ValStoreResult addValueName(ValueStore* store, const char* name, Value* val);

void dumpValueStore(ValueStore* store);

#include <stdio.h>
void loadValueStore(ValueStore* store, FILE* fp);
void saveValueStore(ValueStore* store, FILE* fp);

#endif

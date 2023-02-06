
#include <assert.h>

#include "common.h"

Value* createValue(ValueType type, const char* name) {

    Value* val = _alloc_ds(Value);
    val->type = type;
    val->is_assigned = false;
    val->is_const = false;
    if(name != NULL)
        val->name = _copy_str(name);
    else
        val->name = NULL;

    return val;
}

void assignNumVal(Value* value, double val) {

    value->data.num = val;
    value->is_assigned = true;
}

void assignStrVal(Value* value, const char* val) {

    value->data.str = (char*)val;
    value->is_assigned = true;
}

void assignBoolVal(Value* value, bool val) {

    value->data.bval = val;
    value->is_assigned = true;
}

void assignAddrVal(Value* value, uint32_t val) {

    value->data.addr = val;
    value->is_assigned = true;
}

void assignValName(Value* val, const char* name) {

    val->name = _copy_str(name);
}

void destroyValue(Value* val) {

    assert(val != NULL);
    if(val->name != NULL)
        _free(val->name);
    _free(val);
}

ValueType getValType(Value* val) {

    assert(val != NULL);
    return val->type;
}


ValueTab* createValueTab() {

    ValueTab* tab = _alloc_ds(ValueTab);
    tab->list = createPtrLst();
    tab->index = createHashTab();

    return tab;
}

void destroyValueTab(ValueTab* vs) {

    assert(vs != NULL);
    for(Value* val = resetPtrLst(vs->list); val != NULL; val = iteratePtrLst(vs->list))
        destroyValue(val);
    destroyPtrLst(vs->list);
    _free(vs);
}

Value* getValueByIndex(ValueTab* tab, int idx) {

    assert(tab != NULL);
    return (Value*)getPtrLst(tab->list, idx);
}

Value* getValueByName(ValueTab* tab, const char* name) {

    assert(tab != NULL);

    int idx;
    if(findHashTab(tab->index, name, &idx, sizeof(idx)) == HASH_OK)
        return getValueByIndex(tab, idx);
    else
        return NULL;
}

int addValue(ValueTab* tab, Value* val) {

    assert(tab != NULL);

    int idx = appendPtrLst(tab->list, val);
    val->idx = idx;

    if(val->name != NULL)
        insertHashTab(tab->index, val->name, &idx, sizeof(idx));
    appendPtrLst(tab->list, val);

    return idx;
}

static const char* val_type_to_str(ValueType type) {

    return (type == VAL_NUM )? "NUMBER":
        (type == VAL_STR )? "STRING":
        (type == VAL_BVAL)? "BOOLEAN":
        //(type == VAL_USER)? "USER":
        (type == VAL_ADDR)? "ADDRESS": "UNKNOWN";
}

void dumpValue(Value* val) {

    printf("%d:%s:%c:%c:\t\"%s\"\t", val->idx, val_type_to_str(val->type),
            val->is_assigned? 't': 'f', val->is_const? 't':'f',
            val->name? val->name: "");

    switch(val->type) {
        case VAL_NUM: printf("%f ", val->data.num); break;
        case VAL_ADDR: printf("%u ", val->data.addr); break;
        case VAL_STR: printf("\"%s \"", val->data.str); break;
        case VAL_BVAL: printf("%s ", val->data.bval? "true": "false"); break;
        default: printf("UNKNOWN"); break;
    }
}

void dumpValueTab(ValueTab* tab) {

    assert(tab != NULL);

    printf("Value Table:\n");
    for(Value* val = resetPtrLst(tab->list); val != NULL;
                val = iteratePtrLst(tab->list))
        dumpValue(val);
}

static Value* load_value(FileBuf* fp) {

    Value* val = _alloc_ds(Value);

    readFile(fp, &val->type, sizeof(val->type));
    readFile(fp, &val->is_assigned, sizeof(val->is_assigned));
    readFile(fp, &val->is_const, sizeof(val->is_const));
    readFile(fp, &val->idx, sizeof(val->idx));

    size_t sz = 0;
    readFile(fp, &sz, sizeof(sz));
    val->name = _alloc(sz);
    readFile(fp, (void*)val->name, sz);
    if(val->is_assigned) {
        switch(val->type) {
            case VAL_NUM:
                readFile(fp, &val->data.num, sizeof(val->data.num));
                break;
            case VAL_ADDR:
                readFile(fp, &val->data.addr, sizeof(val->data.addr));
                break;
            case VAL_BVAL:
                readFile(fp, &val->data.bval, sizeof(val->data.bval));
                break;
            case VAL_STR: {
                    size_t slen;
                    readFile(fp, &slen, sizeof(slen));
                    val->data.str = _alloc(slen+1);
                    readFile(fp, val->data.str, slen);
                }
                break;
        }
    }

    return val;
}

ValueTab* loadValueTab(FileBuf* fp) {

    int len;
    ValueTab* tab = createValueTab();

    readFile(fp, &len, sizeof(len));

    for(int idx = 0; idx < len; idx++) {
        Value* val = load_value(fp);
        addValue(tab, val);
    }

    tab->index = loadHashTab(fp);

    return tab;
}

static void save_value(Value* val, FileBuf* fp) {

    writeFile(fp, &val->type, sizeof(val->type));
    writeFile(fp, &val->is_assigned, sizeof(val->is_assigned));
    writeFile(fp, &val->is_const, sizeof(val->is_const));
    writeFile(fp, &val->idx, sizeof(val->idx));

    if(val->name != NULL) {
        size_t sz = strlen(val->name) + 1;
        writeFile(fp, &sz, sizeof(sz));
        writeFile(fp, (void*)val->name, sz);
    }
    else {
        size_t sz = 1;
        writeFile(fp, &sz, sizeof(sz));
        char* buf[1] = {0};
        writeFile(fp, buf, sz);
    }

    if(val->is_assigned) {
        switch(val->type) {
            case VAL_NUM:
                writeFile(fp, &val->data.num, sizeof(val->data.num));
                break;
            case VAL_ADDR:
                writeFile(fp, &val->data.addr, sizeof(val->data.addr));
                break;
            case VAL_BVAL:
                writeFile(fp, &val->data.bval, sizeof(val->data.bval));
                break;
            case VAL_STR: {
                    size_t slen = strlen(val->data.str)+1;
                    writeFile(fp, &slen, sizeof(slen));
                    writeFile(fp, val->data.str, slen);
                }
                break;
        }
    }
}

void saveValueTab(ValueTab* tab, FileBuf* fp) {

    Value** vlst = (Value**)getRawPtrLst(tab->list);
    int len = getLenPtrLst(tab->list);

    writeFile(fp, &len, sizeof(len));
    for(int idx = 0; idx < len; idx++)
        save_value(vlst[idx], fp);

    saveHashTab(tab->index, fp);
}


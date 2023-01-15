
#include <assert.h>
#include "common.h"
#include "asmblr.h"

static struct _addr_ref_* create_addr_ref(const char* name, uint32_t idx) {

    AddrRef* addr = _alloc_ds(AddrRef);
    addr->loc = idx;
    addr->name = _copy_str(name);

    return addr;
}

static void destroy_addr_ref(AddrRef* ptr) {

    assert(ptr != NULL);
    _free(ptr->name);
    _free(ptr);
}

static int add_addr_ref(AddrRefTab* ptr, AddrRef* addr) {

    assert(ptr != NULL);
    assert(addr != NULL);

    if(ptr->len+1 > ptr->cap) {
        ptr->cap <<= 1;
        ptr->lst = _realloc_ds_array(ptr->lst, AddrRef*, ptr->cap);
    }

    int retv = ptr->len;
    ptr->lst[ptr->len] = addr;
    ptr->len ++;

    return retv;
}

/*
 * Create the address store.
 */
AddrRefTab* createAddrRefTab() {

    AddrRefTab* ptr = _alloc_ds(AddrRefTab);
    ptr->cap = 1 << 3;
    ptr->len = 0;
    ptr->lst = _alloc_ds_array(struct _addr_ref_*, ptr->cap);

    return ptr;
}

/*
 * Destroy the address store.
 */
void destroyAddrRefTab(AddrRefTab* ptr) {

    assert(ptr != NULL);

    for(size_t i = 0; i < ptr->len; i++)
        destroy_addr_ref(ptr->lst[i]);
    _free(ptr->lst);
    _free(ptr);
}

/*
 * Create a label definition.
 */
void addAddrDef(Assembler* ptr, const char* name) {

    // get the location of the label in the instruction stream
    uint32_t idx = getInstrBufLoc(ptr->instrBuf);

    if(HASH_OK == get_addr_def(ptr->addrDefs, name, &idx))
        syntax_error("label \"%s\" is already defined", name);
    else {
        add_addr_def(ptr->addrDefs, name, &idx);
    }
}

/*
 * Create a label reference and emit the opcode and the location.
 */
void addAddrRef(Assembler* ptr, const char* name) {

    uint32_t idx = getInstrBufLoc(ptr->instrBuf);
    add_addr_ref(ptr->addrRefs, create_addr_ref(name, idx));
}

/*
 * Read the instruction stream and edit the flow control instructions to have
 * the address that was defined by source code. This is done this way to handle
 * forward references of labels in the input.
 */
void fixupAddrs(Assembler* ptr) {

    AddrRefTab* rtab = ptr->addrRefs;
    AddrDefTab* dtab = ptr->addrDefs;

    for(size_t idx = 0; idx < rtab->len; idx++) {
        AddrRef* ref = rtab->lst[idx];
        uint32_t loc;
        if(get_addr_def(dtab, ref->name, &loc) != HASH_OK)
            syntax_error("label \"%s\" has not been defined", ref->name);
        updateInstrBuf(ptr->instrBuf, ref->loc, &loc, sizeof(uint32_t));
    }
}

/*
 * Look up the name of a label from the label reference hash table.
 *
 * VERY INEFFICIENT.
 *
 */
const char* lookupAddrRefName(AddrDefTab* tab, uint32_t val) {

    for(int i = 0; i < tab->cap; i++) {
        if(tab->table[i] != NULL) {
            if(tab->table[i]->key != NULL) {
                if(val == *(size_t*)tab->table[i]->data)
                    return tab->table[i]->key;
            }
        }
    }

    return NULL;
}

void dumpAddrs(Assembler* ptr) {

    printf("\nAddress Table:\n\tRefs:\n");

    AddrRefTab* refs = ptr->addrRefs;
    for(size_t idx  = 0; idx < refs->len; idx++) {
        printf("\t%s\t%u\n",
               refs->lst[idx]->name,
               refs->lst[idx]->loc);
    }

    printf("--------------------\n\tDefs:\n");
    HashTab* tab = ptr->addrDefs;
    for(int i = 0; i < tab->cap; i++) {
        if(tab->table[i] != NULL) {
            if(tab->table[i]->key != NULL) {
                size_t idx = *(size_t*)tab->table[i]->data;
                printf("\t%s\t%lu\n", tab->table[i]->key, idx);
            }
        }
    }
}

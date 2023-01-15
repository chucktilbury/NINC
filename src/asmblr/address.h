#ifndef _ADDRESS_H_
#define _ADDRESS_H_

/*
 * This data structure is stored in the address definition hash table.
 */
typedef HashTab AddrDefTab;
#define createAddrDefTab() createHashTab()
#define destroyAddrDefTab(t) destroyHashTab(t)
#define add_addr_def(t, n, v) insertHashTab(t, n, v, sizeof(uint32_t))
#define get_addr_def(t, n, v) findHashTab(t, n, v, sizeof(uint32_t))

struct _addr_ref_ {
    const char* name;
    uint32_t loc;   // location to edit in the instruction stream.
};

struct _addr_ref_tab_ {
    AddrRef** lst;
    size_t cap;
    size_t len;
};

AddrRefTab* createAddrRefTab();
void destroyAddrRefTab(AddrRefTab* ptr);

void addAddrDef(Assembler* ptr, const char* name);
void addAddrRef(Assembler* ptr, const char* name);
void fixupAddrs(Assembler* ptr);
void dumpAddrs(Assembler* ptr);
const char* lookupAddrRefName(AddrDefTab* tab, uint32_t val);

#endif

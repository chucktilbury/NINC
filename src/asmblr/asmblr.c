
#include "common.h"
#include "asmblr.h"
#include "parser.h"
#include "scanner.h"
#include "address.h"

Assembler* asmblr = NULL;
int errors = 0;

void dump_hash(HashTab* tab) {

    for(int i = 0; i < tab->cap; i++) {
        if(tab->table[i] != NULL) {
            if(tab->table[i]->key != NULL) {
                size_t idx = *(size_t*)tab->table[i]->data;
                printf("\t%s\t%lu\n", tab->table[i]->key, idx);
            }
        }
    }
}

void disasm(Assembler*);

void dump_tables() {

    disasm(asmblr);
    printf("\n");
    dumpValueStore(asmblr->valStore);
    printf("\nSymbol Table:\n");
    dump_hash(asmblr->symbols);
    dumpAddrs(asmblr);
    printf("\n");
}

#include <stdarg.h>
void syntax_error(const char* fmt, ...) {

    fprintf(stderr, "%s:%d:%d syntax error, ",
            get_file_name(), get_line_no(), get_col_no());

    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");

    errors++;
}

Assembler* createAsmblr(const char* fname) {

    Assembler* ptr = _alloc_ds(Assembler);

    ptr->valStore = createValueStore();
    ptr->instrBuf = createInstrBuf();
    ptr->symbols = createHashTab();
    ptr->addrDefs = createAddrDefTab();
    ptr->addrRefs = createAddrRefTab();

    open_file(fname);

    return ptr;
}

void destroyAsmblr(Assembler* ptr) {

    destroyValueStore(ptr->valStore);
    destroyInstrBuf(ptr->instrBuf);
    destroyHashTab(ptr->symbols);
    destroyAddrDefTab(ptr->addrDefs);
    destroyAddrRefTab(ptr->addrRefs);

    _free(ptr);
}

int main(int argc, char** argv) {

    if(argc < 2) {
        printf("need a file name\n");
        exit(1);
    }

    asmblr = createAsmblr(argv[1]);

    if(asm_parse()) {
        printf("parse fail\n");
        return 1;
    }
    fixupAddrs(asmblr);

    if(errors == 0)
        dump_tables();

    destroyAsmblr(asmblr);

    return 0;
}

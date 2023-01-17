
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

void dump_tables(Assembler* ptr) {

    printf("\n");
    disasm(ptr);
    printf("\n");
    dumpValueStore(ptr->valStore);
    dumpAddrs(ptr);
    printf("\n");
}

#include <stdarg.h>
void syntax_error(const char* fmt, ...) {

    if(get_line_no() > 0)
        fprintf(stderr, "%s:%d:%d syntax error, ",
                get_file_name(), get_line_no(), get_col_no());
    else
        fprintf(stderr, "syntax error, ");

    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");

    errors++;
}

Assembler* createAsmblr() {

    Assembler* ptr = _alloc_ds(Assembler);

    ptr->valStore = createValueStore();
    ptr->instrBuf = createInstrBuf();
    ptr->symbols = createHashTab();
    ptr->addrDefs = createAddrDefTab();
    ptr->addrRefs = createAddrRefTab();

    return ptr;
}

void destroyAsmblr(Assembler* ptr) {

    destroyValueStore(ptr->valStore);
    destroyInstrBuf(ptr->instrBuf);
    destroyHashTab(ptr->symbols);
    destroyAddrDefTab(ptr->addrDefs);
    destroyAddrRefTab(ptr->addrRefs);
    destroyCmdLine();

    _free(ptr);
}

void save_module(const char* fname, Assembler* ptr) {

    FILE* fp = fopen(fname, "w+");
    if(fp == NULL) {
        fprintf(stderr,
                "fatal error: cannot open output file: %s: %s\n",
                fname, strerror(errno));
        exit(1);
    }
    saveInstrBuf(ptr->instrBuf, fp);
    saveValueStore(ptr->valStore, fp);

    HashTab* tab = ptr->addrDefs;
    fwrite(&tab->count, sizeof(tab->count), 1, fp);
    for(int i = 0; i < tab->cap; i++) {
        if(tab->table[i] != NULL) {
            if(tab->table[i]->key != NULL) {
                size_t idx = *(size_t*)tab->table[i]->data;
                fwrite(&idx, sizeof(idx), 1, fp);
                idx = strlen(tab->table[i]->key) + 1;
                fwrite(&idx, sizeof(idx), 1, fp);
                fwrite(tab->table[i]->key, sizeof(char), idx, fp);
            }
        }
    }

    fclose(fp);
}

Assembler* load_module(const char* fname) {

    FILE* fp = fopen(fname, "r+");
    if(fp == NULL) {
        fprintf(stderr,
                "fatal error: cannot open input file: %s: %s\n",
                fname, strerror(errno));
        exit(1);
    }

    Assembler* ptr = createAsmblr();

    loadInstrBuf(ptr->instrBuf, fp);
    loadValueStore(ptr->valStore, fp);

    HashTab* tab = ptr->addrDefs;
    int count;
    fread(&count, sizeof(count), 1, fp);

    for(int i = 0; i < count; i++) {
        size_t sz, idx;
        fread(&idx, sizeof(idx), 1, fp);
        fread(&sz, sizeof(sz), 1, fp);
        char* str = _alloc(sz);
        fread(str, sizeof(char), sz, fp);
        insertHashTab(tab, str, &idx, sizeof(idx));
    }

    fclose(fp);
    return ptr;
}

int main(int argc, char** argv) {

    initCmdLine(CL_FL_ONE,
                "This is the assembler. It reads the assembly language input\n"
                "and converts it to a binary for use by the virtual machine.\n");
    addStrParam("-o", "ofile", "output file name", "output.bin", CL_REQD);
    addNumParam("-v", "verbose", "verbosity number from 0 to 10", 0, CL_NONE);
    addCBwoParam("-h", "show the help information", showUseCmdLine, CL_NONE);
    parseCmdLine(argc, argv);

    asmblr = createAsmblr();

    resetCLFileList();
    const char* name = iterateCLFileList();
    if(name != NULL && strlen(name) > 0)
        open_file(name);
    else
        showUseCmdLine();

    if(asm_parse()) {
        printf("parse fail: %d error(s)\n", errors);
        return 1;
    }
    fixupAddrs(asmblr);

    if(errors == 0 && getNumParam("verbose") >= 1)
        dump_tables(asmblr);

    save_module(getStrParam("ofile"), asmblr);

    destroyAsmblr(asmblr);

    Assembler* a = load_module("test.bin");
    dump_tables(a);

    return 0;
}


#include "common.h"
#include "asmblr.h"
#include "parser.h"
#include "scanner.h"
#include "address.h"

Module* module = NULL;
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

void disasm(Module*);

void dump_tables(Assembler* ptr) {

    printf("\n");
    disasm(ptr);
    printf("\n");
    dumpValueTab(ptr->valTab);
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

void __ferror(const char* func, int line, const char* fmt, ...) {

    fprintf(stderr, "%s:%d: fatal error, ", func, line);

    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");

    errors++;
    exit(1);
}

int main(int argc, char** argv) {

    initCmdLine(CL_FL_ONE,
                "This is the assembler. It reads the assembly language input\n"
                "and converts it to a binary for use by the virtual machine.\n");
    addStrParam("-o", "ofile", "output file name", "output.bin", CL_REQD);
    addNumParam("-v", "verbose", "verbosity number from 0 to 10", 0, CL_NONE);
    addCBwoParam("-h", "show the help information", showUseCmdLine, CL_NONE);
    parseCmdLine(argc, argv);

    module = createModule();

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
    fixupAddrs(module);

    if(errors == 0 && getNumParam("verbose") >= 1)
        dump_tables(module);

    saveModule(getStrParam("ofile"), module);

    destroyModule(module);
    destroyCmdLine();

    Assembler* a = loadModule("test.bin");
    dump_tables(a);

    return 0;
}

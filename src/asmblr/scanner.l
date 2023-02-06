%{
#include "common.h"
#include "parser.h"
#include "asmblr.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"

void yyerror(const char* s);

typedef struct _file_stack_ {
    const char* fname;
    int line;
    int col;
    FILE* fptr;
    YY_BUFFER_STATE buffer;
    struct _file_stack_* next;
} FileStack;

#define MAX_INCL 15
FileStack* fstack = NULL;
int incl_depth = 0;

int bidx = 0;
char buffer[1024 * 64];
const char* file_name;

static void __append_char(char ch) {
    // TODO: bounds checking
    buffer[bidx] = ch;
    bidx++;
}

static void __append_str(const char* str) {
    // TODO: bounds checking
    strcpy(&buffer[bidx], str);
    bidx += strlen(str);
}

int yycolumn = 1;

/* This is executed before every action. */
#define YY_USER_ACTION                                                   \
  fstack->line = prev_yylineno; fstack->col = yycolumn;                  \
  if (yylineno == prev_yylineno) yycolumn += yyleng;                     \
  else {                                                                 \
    for (yycolumn = 1; asm_text[yyleng - yycolumn] != '\n'; ++yycolumn) {} \
    prev_yylineno = yylineno;                                            \
  }

%}

/* options */
%option noinput nounput
%option yylineno
%option noyywrap

/* state definitions */
%x SQUOTES
%x DQUOTES
%x COMMENT

%%

   /* Any indented text before the first rule goes at the top of the lexer.  */
   int prev_yylineno = asm_lineno;

\n      {}
[ \t\r] {}

    /* Start unique rules */
"number"|"num"      { return NUMBER; }
"string"|"strg"     { return STRING; }
"boolean"|"bool"    { return BOOLEAN; }
"import"            { return IMPORT; }

"true"          { asm_lval.bool_val = true; return TRUE; }
"false"         { asm_lval.bool_val = false; return FALSE; }

"EXIT"          { asm_lval.opcode = OP_EXIT; return EXIT; }
"NOP"           { asm_lval.opcode = OP_NOP; return NOP; }
"BREAK"         { asm_lval.opcode = OP_BREAK; return BREAK; }
"CALL"          { asm_lval.opcode = OP_CALL; return CALL; }
"TRAP"          { asm_lval.opcode = OP_TRAP; return TRAP; }
"RETURN"        { asm_lval.opcode = OP_RETURN; return RETURN; }
"JMP"           { asm_lval.opcode = OP_JMP; return JMP; }
"BR"            { asm_lval.opcode = OP_BR; return BR; }
"PUSH"          { asm_lval.opcode = OP_PUSH; return PUSH; }
"POP"           { asm_lval.opcode = OP_POP; return POP; }
"PEEK"          { asm_lval.opcode = OP_PEEK; return PEEK; }
"SIDX"          { asm_lval.opcode = OP_SIDX; return SIDX; }
"LOAD"          { asm_lval.opcode = OP_LOAD; return LOAD; }
"STORE"         { asm_lval.opcode = OP_STORE; return STORE; }
"NOT"           { asm_lval.opcode = OP_NOT; return NOT; }
"EQ"            { asm_lval.opcode = OP_EQ; return EQ; }
"NEQ"           { asm_lval.opcode = OP_NEQ; return NEQ; }
"LTE"           { asm_lval.opcode = OP_LTE; return LTE; }
"GTE"           { asm_lval.opcode = OP_GTE; return GTE; }
"LT"            { asm_lval.opcode = OP_LT; return LT; }
"GT"            { asm_lval.opcode = OP_GT; return GT; }
"NEG"           { asm_lval.opcode = OP_NEG; return NEG; }
"ADD"           { asm_lval.opcode = OP_ADD; return ADD; }
"SUB"           { asm_lval.opcode = OP_SUB; return SUB; }
"MUL"           { asm_lval.opcode = OP_MUL; return MUL; }
"DIV"           { asm_lval.opcode = OP_DIV; return DIV; }
"MOD"           { asm_lval.opcode = OP_MOD; return MOD; }

"R0"            { asm_lval.reg = REG_0; return R0; }
"R1"            { asm_lval.reg = REG_1; return R1; }
"R2"            { asm_lval.reg = REG_2; return R2; }
"R3"            { asm_lval.reg = REG_3; return R3; }
"R4"            { asm_lval.reg = REG_4; return R4; }
"R5"            { asm_lval.reg = REG_5; return R5; }
"R6"            { asm_lval.reg = REG_6; return R6; }
"R7"            { asm_lval.reg = REG_7; return R7; }
"R8"            { asm_lval.reg = REG_8; return R8; }
"R9"            { asm_lval.reg = REG_9; return R9; }
"R10"           { asm_lval.reg = REG_10; return R10; }
"R11"           { asm_lval.reg = REG_11; return R11; }
"R12"           { asm_lval.reg = REG_12; return R12; }
"R13"           { asm_lval.reg = REG_13; return R13; }
"R14"           { asm_lval.reg = REG_14; return R14; }
"R15"           { asm_lval.reg = REG_15; return R15; }

"CONST"         { return CONST; }
    /* End unique rules */

[a-zA-Z_$][a-zA-Z_0-9$\.#%&@!]* {
        asm_lval.symbol = (char*)strdup(asm_text);
        return SYMBOL;
    }

-?[0-9]+ {
        asm_lval.num = (double)strtol(asm_text, NULL, 10);
        return NUM;
    }

    /* recognize a float */
-?([0-9]*\.)?[0-9]+([Ee][-+]?[0-9]+)? {
        asm_lval.num = strtod(asm_text, NULL);
        return NUM;
    }

0[Xx][0-9a-fA-F]+ {
        asm_lval.num = (double)strtol(asm_text, NULL, 16);
        return NUM;
    }

    /* double quoted strings have escapes managed */
\"  {
        bidx = 0;
        memset(buffer, 0, sizeof(buffer));
        BEGIN(DQUOTES);
    }

<DQUOTES>\" {
        asm_lval.str = _copy_str(buffer);
        BEGIN(INITIAL);
        return QSTRG;
    }

    /* the short rule matches before the long one does */
<DQUOTES>\\n { __append_char('\n'); }
<DQUOTES>\\r { __append_char('\r'); }
<DQUOTES>\\e { __append_char('\x1b'); }
<DQUOTES>\\t { __append_char('\t'); }
<DQUOTES>\\b { __append_char('\b'); }
<DQUOTES>\\f { __append_char('\f'); }
<DQUOTES>\\v { __append_char('\v'); }
<DQUOTES>\\\\ { __append_char('\\'); }
<DQUOTES>\\\" { __append_char('\"'); }
<DQUOTES>\\\' { __append_char('\''); }
<DQUOTES>\\\? { __append_char('\?'); }
<DQUOTES>\\.  { __append_char(asm_text[1]); }
<DQUOTES>\\[0-7]{1,3} { __append_char((char)strtol(asm_text + 1, 0, 8)); }
<DQUOTES>\\[xX][0-9a-fA-F]{1,3} { __append_char((char)strtol(asm_text + 2, 0, 16)); }
<DQUOTES>[^\\\"\n]*  { __append_str(asm_text); }
<DQUOTES>\n     { asm_lineno++; } /* track line numbers, but strip new line */

    /* single quoted strings are absolute literals */
\'  {
        bidx = 0;
        memset(buffer, 0, sizeof(buffer));
        BEGIN(SQUOTES);
    }

<SQUOTES>\' {
        asm_lval.str = _copy_str(buffer);
        BEGIN(INITIAL);
        return QSTRG;
    }

<SQUOTES>[^\\'\n]*  { __append_str(asm_text); }
<SQUOTES>\\.    { __append_str(asm_text); }
<SQUOTES>\n     {
        __append_str(asm_text);
        asm_lineno++;
    } /* don't strip new lines */

    /* All other character are sent up to the parser */
.   { return asm_text[0]; }

    /* comments */
";".*\n {}

<<EOF>> {

        incl_depth--;

        FileStack* tmp = fstack;
        _free(tmp->fname);
        fclose(tmp->fptr);
        _free(tmp);
        fstack = fstack->next;

        if(fstack == NULL) {
            yyterminate();
        }
        else {
            yy_delete_buffer(YY_CURRENT_BUFFER);
            yy_switch_to_buffer(fstack->buffer);
        }
    }


%%

int open_file(const char *fname) {

    if(incl_depth > MAX_INCL) {
        fprintf(stderr, "FATAL ERROR: Maximum include depth exceeded\n");
        exit(1);
    }
    incl_depth++;

    FileStack* fs = _alloc_ds(FileStack);
    fs->fname = _copy_str(fname);
    fs->line = 1;
    fs->col = 1;
    fs->next = NULL;

    // file_name = _copy_str(fname);
    yyin = fopen(fname, "r");
    if(yyin == NULL) {
        fatal_error("cannot open input file: %s: %s\n", fname, strerror(errno));
    }
    fs->fptr = yyin;
    fs->buffer = yy_create_buffer(yyin, YY_BUF_SIZE);
    yy_switch_to_buffer(fs->buffer);

    if(fstack != NULL) {
        fs->next = fstack;
        fstack = fs;
    }
    else
        fstack = fs;

    BEGIN(INITIAL);
    return 0;
}

int get_line_no() {

    if(fstack != NULL)
        return fstack->line;
    else
        return -1;
}

int get_col_no() {

    if(fstack != NULL)
        return fstack->col;
    else
        return -1;
}

const char* get_file_name() {

    if(fstack != NULL)
        return fstack->fname;
    else
        return "no file open";
}



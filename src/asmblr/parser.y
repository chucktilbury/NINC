%debug
%defines
%locations

%{

#include "common.h"
#include "asmblr.h"
#include "scanner.h"
#include "emit.h"
#include "address.h"

extern Assembler* asmblr;

%}

%union {
    char* str;
    char* symbol;
    double num;
    int type;
    bool bool_val;
    int opcode;
    int reg;

    Value* val;

};


%token<opcode> EXIT NOP CALL TRAP RETURN JMP BR PUSH POP PEEK SIDX LOAD
%token<opcode> STORE NOT EQ NEQ LTE GTE LT GT NEG ADD SUB MUL DIV MOD BREAK

%token<reg> R0 R1 R2 R3 R4 R5 R6 R7 R8
%token<reg> R9 R10 R11 R12 R13 R14 R15
%token CONST IMPORT

%token<bool_val> TRUE FALSE
%token<type> NUMBER STRING BOOLEAN NOTHING
%token<symbol> SYMBOL
%token<num> NUM
%token<str> QSTRG

%type<reg> register
%type<type> type_name
%type<val> type_preamble
%type<bool_val> bool_value

    /* token list automatically generated by boilerplate */

%define parse.error verbose
%locations

%left '+' '-'
%left '*' '/' '%'
%left NEGATE

%%

module
    : {
            addAddrDef(asmblr, "begin_instructions");
        }module_list {
            // make sure that the last thing is a NOP
            emit_instr(asmblr->instrBuf, OP_NOP);
        }
    ;

module_item
    : data_definition
    | label_definition
    | instruction_definition
    | IMPORT QSTRG { open_file($2); }
    ;

module_list
    : module_item
    | module_list module_item
    ;

type_name
    : NUMBER { $$ = VAL_NUM; }
    | STRING { $$ = VAL_STR; }
    | BOOLEAN { $$ = VAL_BVAL; }
    ;

type_preamble
    : type_name {
            $$ = createValue($1, NULL);
        }
    | CONST type_name {
            $$ = createValue($2, NULL);
            $$->is_const = true;
        }
    ;

bool_value
    : TRUE { $$ = $1; }
    | FALSE { $$ = $1; }
    ;

data_definition
    : type_preamble SYMBOL {
            size_t idx = addValue(asmblr->valStore, $1, $2);
            if(insertHashTab(asmblr->symbols, $2, &idx, sizeof(idx)))
                syntax_error("symbol \"%s\" is already defined", $2);
        }
    | type_preamble SYMBOL '=' NUM {
            size_t idx = addValue(asmblr->valStore, $1, $2);
            if(insertHashTab(asmblr->symbols, $2, &idx, sizeof(idx)))
                syntax_error("symbol \"%s\" is already defined", $2);
            else
                assignNumVal($1, $4);
        }
    | type_preamble SYMBOL '=' QSTRG {
            size_t idx = addValue(asmblr->valStore, $1, $2);
            if(insertHashTab(asmblr->symbols, $2, &idx, sizeof(idx)))
                syntax_error("symbol \"%s\" is already defined", $2);
            else
                assignStrVal($1, $4);
        }
    | type_preamble SYMBOL '=' bool_value {
            size_t idx = addValue(asmblr->valStore, $1, $2);
            if(insertHashTab(asmblr->symbols, $2, &idx, sizeof(idx)))
                syntax_error("symbol \"%s\" is already defined", $2);
            else
                assignBoolVal($1, $4);
        }
    ;

label_definition
    : SYMBOL ':' {
            addAddrDef(asmblr, $1);
        }
    ;

    /* Flow control instructions accept the name of a label. */
flow_instruction
    : CALL SYMBOL {
            emit_instr(asmblr->instrBuf, OP_CALL);
            addAddrRef(asmblr, $2);
            emit_addr(asmblr->instrBuf, 0);
        }
    | JMP SYMBOL {
            emit_instr(asmblr->instrBuf, OP_JMP);
            addAddrRef(asmblr, $2);
            emit_addr(asmblr->instrBuf, 0);
        }
    | BR SYMBOL {
            emit_instr(asmblr->instrBuf, OP_BR);
            addAddrRef(asmblr, $2);
            emit_addr(asmblr->instrBuf, 0);
        }
    ;

instruction_definition
    : flow_instruction
    | EXIT {
            emit_instr(asmblr->instrBuf, OP_EXIT);
        }
    | RETURN {
            emit_instr(asmblr->instrBuf, OP_RETURN);
        }
    | NOP {
            emit_instr(asmblr->instrBuf, OP_NOP);
        }
    | BREAK {
            emit_instr(asmblr->instrBuf, OP_BREAK);
        }
    /* One register operand */
    | NOT register ',' register {
            emit_instr(asmblr->instrBuf, OP_NOT);
            emit_2_reg(asmblr->instrBuf, $2, $4);
        }
    /* Two register operands */
    | NEG register ',' register {
            emit_instr(asmblr->instrBuf, OP_NEG);
            emit_2_reg(asmblr->instrBuf, $2, $4);
        }
    | EQ register ',' register {
            emit_instr(asmblr->instrBuf, OP_EQ);
            emit_2_reg(asmblr->instrBuf, $2, $4);
        }
    | NEQ register ',' register {
            emit_instr(asmblr->instrBuf, OP_NEQ);
            emit_2_reg(asmblr->instrBuf, $2, $4);
        }
    | LTE register ',' register {
            emit_instr(asmblr->instrBuf, OP_LTE);
            emit_2_reg(asmblr->instrBuf, $2, $4);
        }
    | GTE register ',' register {
            emit_instr(asmblr->instrBuf, OP_GTE);
            emit_2_reg(asmblr->instrBuf, $2, $4);
        }
    | LT register ',' register {
            emit_instr(asmblr->instrBuf, OP_LT);
            emit_2_reg(asmblr->instrBuf, $2, $4);
        }
    | GT register ',' register {
            emit_instr(asmblr->instrBuf, OP_GT);
            emit_2_reg(asmblr->instrBuf, $2, $4);
        }
    | LOAD register ',' register {
            emit_instr(asmblr->instrBuf, OP_LOAD);
            emit_2_reg(asmblr->instrBuf, $2, $4);
        }
    /* Three register operands */
    | ADD register ',' register ',' register {
            emit_instr(asmblr->instrBuf, OP_ADD);
            emit_3_reg(asmblr->instrBuf, $2, $4, $6);
        }
    | SUB register ',' register ',' register {
            emit_instr(asmblr->instrBuf, OP_SUB);
            emit_3_reg(asmblr->instrBuf, $2, $4, $6);
        }
    | MUL register ',' register ',' register {
            emit_instr(asmblr->instrBuf, OP_MUL);
            emit_3_reg(asmblr->instrBuf, $2, $4, $6);
        }
    | DIV register ',' register ',' register {
            emit_instr(asmblr->instrBuf, OP_DIV);
            emit_3_reg(asmblr->instrBuf, $2, $4, $6);
        }
    | MOD register ',' register ',' register {
            emit_instr(asmblr->instrBuf, OP_MOD);
            emit_3_reg(asmblr->instrBuf, $2, $4, $6);
        }
    /* Trap instruction. Convert index to int. */
    | TRAP NUM {
            emit_instr(asmblr->instrBuf, OP_TRAP);
            emit_val(asmblr->instrBuf, ((uint16_t)$2)&0xFFFF);
        }
    /* Load the value of a var into a register */
    | LOAD register ',' SYMBOL {
            size_t idx;
            if(findHashTab(asmblr->symbols, $4, &idx, sizeof(idx)))
                syntax_error("symbol \"%s\" not defined", $4);
            else {
                emit_instr(asmblr->instrBuf, OP_LOAD);
                emit_1_reg(asmblr->instrBuf, $2);
                emit_val(asmblr->instrBuf, ((uint16_t)idx)&0xFFFF);
            }
        }
    /* Load a literal value into a register */
    | LOAD register ',' NUM {
            Value* val = createValue(VAL_NUM, NULL);
            val->data.num = $4;
            val->is_const = true;
            val->is_assigned = true;
            emit_instr(asmblr->instrBuf, OP_LOADL);
            emit_1_reg(asmblr->instrBuf, $2);
            uint16_t idx = addValue(asmblr->valStore, val, NULL);
            emit_val(asmblr->instrBuf, idx);
        }
    | LOAD register ',' QSTRG {
            Value* val = createValue(VAL_STR, NULL);
            val->data.str = $4;
            val->is_const = true;
            val->is_assigned = true;
            emit_instr(asmblr->instrBuf, OP_LOADL);
            emit_1_reg(asmblr->instrBuf, $2);
            uint16_t idx = addValue(asmblr->valStore, val, NULL);
            emit_val(asmblr->instrBuf, idx);
        }
    | LOAD register ',' bool_value {
            Value* val = createValue(VAL_BVAL, NULL);
            val->data.bval = $4;
            val->is_const = true;
            val->is_assigned = true;
            emit_instr(asmblr->instrBuf, OP_LOADL);
            emit_1_reg(asmblr->instrBuf, $2);
            uint16_t idx = addValue(asmblr->valStore, val, NULL);
            emit_val(asmblr->instrBuf, idx);
        }
    /* Store the value of a register into a var */
    | STORE SYMBOL ',' register {
            size_t idx;
            if(findHashTab(asmblr->symbols, $2, &idx, sizeof(idx)))
                syntax_error("symbol \"%s\" not defined", $4);
            else {
                emit_instr(asmblr->instrBuf, OP_STORE);
                emit_val(asmblr->instrBuf, ((uint16_t)idx)&0xFFFF);
                emit_1_reg(asmblr->instrBuf, $4);
            }
        }
    /* Peek at the stack with a given register and index. */
    | PEEK register ',' register ',' NUM {
            emit_instr(asmblr->instrBuf, OP_PEEK);
            emit_2_reg(asmblr->instrBuf, $2, $4);
            emit_val(asmblr->instrBuf, ((uint16_t)$6)&0xFFFF);
        }
    | POP register {
            emit_instr(asmblr->instrBuf, OP_POP);
            emit_1_reg(asmblr->instrBuf, $2);
        }
    /* Push a register onto the stack */
    | PUSH register {
            emit_instr(asmblr->instrBuf, OP_PUSH);
            emit_1_reg(asmblr->instrBuf, $2);
        }
    /* Push the value of the var onto the stack. */
    | PUSH SYMBOL {
            size_t idx;
            if(findHashTab(asmblr->symbols, $2, &idx, sizeof(idx)))
                syntax_error("symbol \"%s\" not defined", $2);
            else {
                emit_instr(asmblr->instrBuf, OP_PUSHS);
                emit_val(asmblr->instrBuf, ((uint16_t)idx)&0xFFFF);
            }
        }
    /* Store the current stack index into the specified register */
    | SIDX register {
            emit_instr(asmblr->instrBuf, OP_SIDX);
            emit_1_reg(asmblr->instrBuf, $2);
        }
    ;

register
    : R0 { $$ = $1; }
    | R1 { $$ = $1; }
    | R2 { $$ = $1; }
    | R3 { $$ = $1; }
    | R4 { $$ = $1; }
    | R5 { $$ = $1; }
    | R6 { $$ = $1; }
    | R7 { $$ = $1; }
    | R8 { $$ = $1; }
    | R9 { $$ = $1; }
    | R10 { $$ = $1; }
    | R11 { $$ = $1; }
    | R12 { $$ = $1; }
    | R13 { $$ = $1; }
    | R14 { $$ = $1; }
    | R15 { $$ = $1; }
    ;

%%

extern int errors;

void yyerror(const char* s) {

    fprintf(stderr, "%s:%d:%d %s\n", get_file_name(), get_line_no(), get_col_no(), s);
    errors++;
    //error("%s", s);
}

const char* tokenToStr(int tok) {

    return yysymbol_name(YYTRANSLATE(tok));
}

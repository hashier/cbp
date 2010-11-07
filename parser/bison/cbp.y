/* Compilerbau Praktikum */
/* cbp.y */

%{
#include "ExprNodes.h"
#include "ProgramNodes.h"

int yyerror(char *s);
int yylex(void);
%}

%union{
	Function*               func_val;
	Statement*              statement;
	Expression*             expr_val;
	Variable*               var_val;
	Block*                  block_val;
	std::list<Variable*>*   var_list;
	std::list<Expression*>* expr_list;
	TypeDecl*               typeDecl_val;
	Type*                   type_val;
	SimpleTypeEnum          simpletype_val;
	Func_abi                abi_val;
	float                   float_val;
	int                     int_val;
	std::list<Variable*>*   struct_members_val;
	//TypeStruct*             struct_val;
	std::string*            string_val;
}

%start input

%token LETTER DIGIT WHITESPACE LINE_COMMENT EOL
%token KEY_FUNC KEY_CALL KEY_TYPE KEY_WHILE KEY_IF KEY_ELSE KEY_STRUCT KEY_VAR KEY_LOCAL
%token CURLY_LEFT CURLY_RIGHT PAR_LEFT PAR_RIGHT COLON SEMICOLON
%token TYPE ABI AT DOLLAR

%token <int_val>	INTEGER_CONSTANT
%token <float_val>	FLOAT_CONSTANT
%token <string_val>	IDENTIFIER

%type <type_val>     type
%type <simpletype_val> TYPE
%type <func_val>     func_decl
%type <expr_val>     exp
%type <statement>    statement
%type <statement>    elseish
%type <block_val>    st_block
%type <typeDecl_val> type_decl
%type <abi_val>      abi ABI
%type <var_val>      var
%type <var_val>      var_decl

%type <var_list>     var_list
%type <expr_list>    exp_list

%type <struct_members_val> struct_members

%left PLUS
%left MULT

%%

input:   /* empty */
       | input type_decl { $2->dump(); }
       | input var_decl  { $2->dump(); }
       | input func_decl { $2->dump(); }
         ;

func_decl: KEY_FUNC abi IDENTIFIER PAR_LEFT var_list PAR_RIGHT COLON type statement { $$ = new Function($3, $2, $5, $9); }
           ;

var: IDENTIFIER COLON type { $$ = new Variable($1, $3); }
   ;

var_list:   /* empty */ { $$ = new std::list<Variable*>(); }
          | var { $$->push_back($1); }
          | var_list var { $$->push_back($2); }
            ;

type_decl: KEY_TYPE IDENTIFIER COLON type { $$ = new TypeDecl($2, $4); }
           ;

type: TYPE
    | KEY_STRUCT CURLY_LEFT struct_members CURLY_RIGHT { $$ = new TypeStruct($3) }
      ;

struct_members: /* empty */ { $$ = new std::list<Variable*>(); }
              | struct_members var_decl { $1->push_back($2); }
              | struct_members var_decl AT INTEGER_CONSTANT { $1->push_back($2); }
                ;

var_decl: KEY_VAR IDENTIFIER COLON type { /* printf("test\n");*/ $$ = new Variable($2, $4); }

statement:   CURLY_LEFT st_block CURLY_RIGHT { $$ = $2 }
           | KEY_WHILE exp statement { $$ = new WhileLoop($2, $3); }
             /* this is a shift-reduce, danglig else problem. not sure what to do about that.. */
           | KEY_IF exp statement elseish { $$ = new IfElse($2, $3, $4); }
           | exp { $$ = $1 }
           | KEY_LOCAL var_decl { $$ = new Local($2); } // TODO
             ;

elseish: /* empty */ { $$ = NULL; }
       | KEY_ELSE statement { $$ = $2 }
         ;

st_block: /* empty */ { $$ = new Block(); }
        | st_block statement { $$->add($2); }
          ;

abi:   /* empty */ { $$ = Abi_default; }
     | ABI
       ;

exp_list:   exp { $$ = new std::list<Expression*>(); $$->push_back($1); }
          | exp_list exp { $$->push_back($2); }
            ;

exp:   INTEGER_CONSTANT	{ $$ = new ConstInt($1); }
     | exp PLUS exp	{ $$ = new Expr_Add($1, $3); }
     | exp MULT exp	{ $$ = new Expr_Mul($1, $3); }
     | KEY_CALL IDENTIFIER PAR_LEFT exp_list PAR_RIGHT { $$ = new FuncCall($2, $4);  }
       ;

%%

int yyerror(std::string s)
{
	extern int yylineno;   // defined and maintained in lex.c
	extern char yytext[];  // defined and maintained in lex.c

	std::cerr << "ERROR: " << s << " at symbol \"" << yytext;
	std::cerr << "\" on line " << yylineno << std::endl;
	exit(1);
}

int yyerror(char *s)
{
	return yyerror(std::string(s));
}



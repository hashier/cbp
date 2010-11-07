/* Compilerbau Praktikum */
/* cbp.y */

%{
#include "ExprNodes.h"
#include "ProgramNodes.h"
int yyerror(char *s);
int yylex(void);
%}

%union{
	Function*		func_val;
	Statement*	statement;
	Expression*	expr_val;
	Variable*	var_val;
	Block*	block_val;
	list<Variable*>*	var_list;
	list<Expression*>*	expr_list;
	TypeDecl*		typeDecl_val;
	Type		type_val;
	Func_abi	abi_val;
	float		float_val;
	int			int_val;
	string*		string_val;
}

%start	input

%token	LETTER DIGIT WHITESPACE LINE_COMMENT EOL
%token KEY_FUNC KEY_CALL KEY_TYPE KEY_WHILE KEY_IF KEY_ELSE
%token CURLY_LEFT CURLY_RIGHT PAR_LEFT PAR_RIGHT COLON SEMICOLON
%token TYPE ABI

%token	<int_val>	INTEGER_CONSTANT
%token	<float_val>	FLOAT_CONSTANT
%token	<string_val>	IDENTIFIER
%type <type_val> TYPE

%type	<func_val>	func_decl
%type	<expr_val>	exp
%type	<statement>	statement
%type	<statement>	elseish
%type	<block_val>	st_block
%type	<typeDecl_val> type_decl
%type   <abi_val> abi ABI
%type   <var_val> var

%type   <var_list> var_list
%type	<expr_list>	exp_list

%left	PLUS
%left	MULT

%%

input:	/* empty */
		| type_decl { $1->dump(); }
		| func_decl { $1->dump(); }
		;

func_decl:	KEY_FUNC abi IDENTIFIER PAR_LEFT var_list PAR_RIGHT COLON TYPE statement { $$ = new Function($3, $2, $5, $9); }
		 ;

var: IDENTIFIER COLON TYPE { $$ = new Variable($1, $3); }
   ;

var_list: /* empty */ { $$ = new list<Variable*>(); }
        | var { $$->push_back($1); }
		| var_list var { $$->push_back($2); }
        ;
	
type_decl: KEY_TYPE IDENTIFIER COLON TYPE { $$ = new TypeDecl($2, $4); }
		; 	

statement: CURLY_LEFT st_block CURLY_RIGHT { $$ = $2 }
         | KEY_WHILE exp statement { $$ = new WhileLoop($2, $3); }
         /* this is a shift-reduce, danglig else problem. not sure what to do about that.. */
         | KEY_IF exp statement elseish { $$ = new IfElse($2, $3, $4); }
         | exp { $$ = $1 }
         ;

elseish: /* empty */ { $$ = NULL; }
       | KEY_ELSE statement { $$ = $2 }
       ;

st_block: /* empty */ { $$ = new Block(); }
        | st_block statement { $$->add($2); }
        ;

abi:	/* empty */ { $$ = Abi_default; }
		| ABI
		;

exp_list: exp { $$ = new list<Expression*>(); $$->push_back($1); }
        | exp_list exp { $$->push_back($2); }
        ;

exp:	INTEGER_CONSTANT	{ $$ = new ConstInt($1); }
		| exp PLUS exp	{ $$ = new Expr_Add($1, $3); }
		| exp MULT exp	{ $$ = new Expr_Mul($1, $3); }
        | KEY_CALL IDENTIFIER PAR_LEFT exp_list PAR_RIGHT { $$ = new FuncCall($2, $4);  }
		;

%%

int yyerror(string s)
{
	extern int yylineno;	// defined and maintained in lex.c
	extern char *yytext;	// defined and maintained in lex.c

	cerr << "ERROR: " << s << " at symbol \"" << yytext;
	cerr << "\" on line " << yylineno << endl;
	exit(1);
}

int yyerror(char *s)
{
	return yyerror(string(s));
}


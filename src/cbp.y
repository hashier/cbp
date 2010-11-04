/* Compilerbau Praktikum */
/* cbp.y */

%{
#include "main.h"
#include "ExprNodes.h"
#include "ProgramNodes.h"
int yyerror(char *s);
int yylex(void);
%}

%token	LETTER DIGIT WHITESPACE LINE_COMMENT EOL
%token KEY_FUNC KEY_ABI_C KEY_ABI_DEFAULT
%token PAR_LEFT PAR_RIGHT COLON
%token UINT8 INT8 UINT16 INT16 UINT32 INT32 FLOAT32 FLOAT64

%union{
	class Function*		func_val;
	class Statement*	statement;
	class Expression*	expr_val;
	float		float_val;
	int			int_val;
	string*		string_val;
}

%start	input

%token	<int_val>	INTEGER_CONSTANT
%token	<float_val>	FLOAT_CONSTANT
%token	<string_val>	IDENTIFIER

%type	<func_val>	func_decl
%type	<expr_val>	exp
%type	<statement>	statement

%left	PLUS
%left	MULT

%%

input:	/* empty */
		| func_decl { $1->dump(0); }
		;


/*
		| exp	{ $1->dump(); }
func_args ::= '(' ( IDENTIFIER ':' type )* ')' ;                                   --Christopher
*/

func_decl:	KEY_FUNC abi IDENTIFIER func_args COLON type statement { $$ = new Function($3, NULL, $7); }
		 ;

func_args:	PAR_LEFT var_list PAR_RIGHT

var_list:	/* empty */
		| IDENTIFIER COLON type;

type: UINT8
	| INT8
	| UINT16
	| INT16
	| UINT32
	| INT32
	| FLOAT32
	| FLOAT64
	;

statement:	/* empty */ { $$ = NULL }
		 | exp;

abi:	/* empty */
		| KEY_ABI_C { ; }
		| KEY_ABI_DEFAULT { ; }
		;

exp:	INTEGER_CONSTANT	{ $$ = new ConstInt($1); }
		| exp PLUS exp	{ $$ = new Expr_Add($1, $3); }
		| exp MULT exp	{ $$ = new Expr_Mul($1, $3); }
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



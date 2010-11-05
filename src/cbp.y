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
%token KEY_FUNC KEY_ABI_C KEY_ABI_DEFAULT KEY_TYPE
%token PAR_LEFT PAR_RIGHT COLON SEMICOLON
%token UINT8 INT8 UINT16 INT16 UINT32 INT32 FLOAT32 FLOAT64 VOID

%union{
	class Function*		func_val;
	class Statement*	statement;
	class Expression*	expr_val;
	class TypeDecl*		typeDecl_val;
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
%type	<typeDecl_val> type_decl;
%type	<string_val> type;

%left	PLUS
%left	MULT

%%

input:	/* empty */
		| type_decl { $1->dump(0); }
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

type: UINT8 { $$ = new string("uint8"); }
	| INT8 { $$ = new string("int8"); }
	| UINT16 { $$ = new string("uint16"); }
	| INT16 { $$ = new string("int16"); }
	| UINT32 { $$ = new string("uint32"); }
	| INT32 { $$ = new string("int32"); }
	| FLOAT32 { $$ = new string("float32"); }
	| FLOAT64 { $$ = new string("float64"); }
	| VOID { $$ = new string("void"); }
	;
	
type_decl : KEY_TYPE IDENTIFIER COLON type { $$ = new TypeDecl($2, $4); }
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



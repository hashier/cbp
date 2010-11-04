/* Compilerbau Praktikum */
/* cbp.y */

%{
#include "main.h"
#include "ExprNodes.h"
int yyerror(char *s);
int yylex(void);
%}

%token	LETTER DIGIT WHITESPACE LINE_COMMENT EOL

%union{
	class Atom*		atom_val;
	class Expression*	expr_val;
	float		float_val;
	int			int_val;
	string*		string_val;
}

%start	input

%token	<int_val>	INTEGER_CONSTANT
%token	<float_val>	FLOAT_CONSTANT
%token	<string>	IDENTIFIER

%type	<expr_val>	exp

%left	PLUS
%left	MULT

%%

input:	/* empty */
		| exp	{ $1->dump(); }
		;

exp:	INTEGER_CONSTANT	{ $$ = new ConstInt($1); }
		| exp PLUS exp	{ $$ = new Expr_Add($1, $3); }
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



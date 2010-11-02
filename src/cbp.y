/* Compilerbau Praktikum */
/* cbp.y */

%{
#include "main.h"
int yyerror(char *s);
int yylex(void);
%}

%token	LETTER DIGIT WHITESPACE LINE_COMMENT

%union{
	int			int_val;
	float		float_val;
	string*		string_val;
}

%start	input

%token	<int_val>	INTEGER_CONSTANT
%token	<float_val>	FLOAT_CONSTANT
%token	<string>	IDENTIFIER

%type	<int_val>	exp
%left	PLUS
%left	MULT

%%

input:	/* empty */
		| exp	{ cout << "Result: " << $1 << endl; }
		;

exp:	INTEGER_CONSTANT	{ $$ = $1; }
		| exp PLUS exp	{ $$ = $1 + $3; }
		| exp MULT exp	{ $$ = $1 * $3; }
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



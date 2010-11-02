/* Compilerbau Praktikum 2010 */
/* cbp.lex */

%{
#include "main.h"
#include "tok.h"

int yyerror(char *s);

%}

letter		[a-zA-Z]
digit		[0-9]
word		[a-zA-Z][a-zA-Z0-9_]*
identifier	{letter}{word}*
int_const	-?{digit}+
float_const	-?{digit}+\.{digit}+

%%

{identifier}	{ yylval.string_val = new std::string(yytext); return IDENTIFIER; }
{int_const}		{ yylval.int_val = atoi(yytext); return INTEGER_CONSTANT; }
{float_const}	{ yylval.float_val = atof(yytext); return FLOAT_CONSTANT; }

[ \t]*			{ return WHITESPACE; }
[\r]?[\n]		{ yylineno++; }
[#]+.*			{ return LINE_COMMENT; }

[+]		{ yylval.string_val = new std::string(yytext); return PLUS; }
"*"		{ yylval.string_val = new std::string(yytext); return MULT; }



.				{ std::cerr << "SCANNER "; yyerror(""); exit(1); }


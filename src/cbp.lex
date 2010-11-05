/* Compilerbau Praktikum 2010 */
/* cbp.lex */

%{
#include "main.h"
//#include "tok.h"
#include "cbp.tab.h"

int yyerror(char *s);

%}

letter		[a-zA-Z]
digit		[0-9]
word		[a-zA-Z][a-zA-Z0-9_]*
identifier	{letter}{word}*
int_const	-?{digit}+
float_const	-?{digit}+\.{digit}+

%%

{int_const}		{ yylval.int_val = atoi(yytext); return INTEGER_CONSTANT; }
{float_const}	{ yylval.float_val = atof(yytext); return FLOAT_CONSTANT; }

"func"			{ return KEY_FUNC; }
"abi_c"			{ return KEY_ABI_C; }
"abi_default"	{ return KEY_ABI_DEFAULT; }
"type"			{ return KEY_TYPE; }

"uint8"			{ return UINT8; }
"int8"			{ return INT8; }
"uint16"		{ return UINT16; }
"int16"			{ return INT16; }
"uint32"		{ return UINT32; }
"int32"			{ return INT32; }
"float32"		{ return FLOAT32; }
"float64"		{ return FLOAT64; }
"void"			{ return VOID; }

"("				{ return PAR_LEFT; }
")"				{ return PAR_RIGHT; }
":"				{ return COLON; }
"+"		{ yylval.string_val = new std::string(yytext); return PLUS; }
"*"		{ yylval.string_val = new std::string(yytext); return MULT; }

[ \t]*			{ /* return WHITESPACE; */ }
[\r]?[\n]		{ yylineno++; /* return EOL; */ }
[#]+.*			{ return LINE_COMMENT; }



{identifier}	{ yylval.string_val = new std::string(yytext); return IDENTIFIER; }


.				{ std::cerr << "SCANNER "; yyerror(""); exit(1); }


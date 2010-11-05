/* Compilerbau Praktikum 2010 */
/* cbp.lex */

%{
#include "main.h"
//#include "tok.h"
#include "ExprNodes.h"
#include "ProgramNodes.h"
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
"type"			{ return KEY_TYPE; }
"abi_c"			{ yylval.abi_val = Abi_c; return ABI; }
"abi_default"	{ yylval.abi_val = Abi_default; return ABI; }

"uint8"			{ yylval.type_val = Type_uint8; return TYPE; }
"int8"			{ yylval.type_val = Type_int8; return TYPE; }
"uint16"		{ yylval.type_val = Type_uint16; return TYPE; }
"int16"			{ yylval.type_val = Type_int16; return TYPE; }
"uint32"		{ yylval.type_val = Type_uint32; return TYPE; }
"int32"			{ yylval.type_val = Type_int32; return TYPE; }
"float32"		{ yylval.type_val = Type_float32; return TYPE; }
"float64"		{ yylval.type_val = Type_float64; return TYPE; }
"void"			{ yylval.type_val = Type_void; return TYPE; }

"("				{ return PAR_LEFT; }
")"				{ return PAR_RIGHT; }
":"				{ return COLON; }
","				{ return COMMA; }
"+"		{ yylval.string_val = new std::string(yytext); return PLUS; }
"*"		{ yylval.string_val = new std::string(yytext); return MULT; }

[ \t]*			{ /* return WHITESPACE; */ }
[\r]?[\n]		{ yylineno++; /* return EOL; */ }
[#]+.*			{ return LINE_COMMENT; }



{identifier}	{ yylval.string_val = new std::string(yytext); return IDENTIFIER; }


.				{ std::cerr << "SCANNER "; yyerror(""); exit(1); }


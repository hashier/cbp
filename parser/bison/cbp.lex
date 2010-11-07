/* Compilerbau Praktikum 2010 */
/* cbp.lex */

%{
#include "ExprNodes.h"
#include "ProgramNodes.h"

// bison generates .hh files in cpp mode
#include "cbp.tab.h"

int yyerror(char *s);
%}

/* Avoid one linker error related to yywrap */
/* http://stackoverflow.com/questions/1811125/undefined-reference-to-yywrap */
%option noyywrap

letter      [a-zA-Z]
digit       [0-9]
word        [a-zA-Z][a-zA-Z0-9_]*
identifier  {letter}{word}*
int_const   -?{digit}+
float_const -?{digit}+\.{digit}+
eol			[\r]?[\n]

%%

{int_const}     { yylval.int_val = atoi(yytext); return INTEGER_CONSTANT; }
{float_const}   { yylval.float_val = (float)atof(yytext); return FLOAT_CONSTANT; }

"func"          { return KEY_FUNC; }
"call"          { return KEY_CALL; }
"type"          { return KEY_TYPE; }
"if"            { return KEY_IF; }
"else"          { return KEY_ELSE; }
"while"         { return KEY_WHILE; }
"for"           { return KEY_FOR; }
"abi_c"         { yylval.abi_val = Abi_c; return ABI; }
"abi_default"   { yylval.abi_val = Abi_default; return ABI; }

"uint8"         { yylval.type_val = Type_uint8;   return TYPE; }
"int8"          { yylval.type_val = Type_int8;    return TYPE; }
"uint16"        { yylval.type_val = Type_uint16;  return TYPE; }
"int16"         { yylval.type_val = Type_int16;   return TYPE; }
"uint32"        { yylval.type_val = Type_uint32;  return TYPE; }
"int32"         { yylval.type_val = Type_int32;   return TYPE; }
"float32"       { yylval.type_val = Type_float32; return TYPE; }
"float64"       { yylval.type_val = Type_float64; return TYPE; }
"void"          { yylval.type_val = Type_void;    return TYPE; }

"{"             { return CURLY_LEFT; }
"}"             { return CURLY_RIGHT; }
"("             { return PAR_LEFT; }
")"             { return PAR_RIGHT; }
":"             { return COLON; }
"+"             { yylval.string_val = new std::string(yytext); return PLUS; }
"*"             { yylval.string_val = new std::string(yytext); return MULT; }

[ \t]*          { /* return WHITESPACE; */ }
{eol}           { yylineno++; /* return EOL; */ }
[#].*{eol}      { /* return LINE_COMMENT; */ }

{identifier}    { yylval.string_val = new std::string(yytext); return IDENTIFIER; }

.               { std::cerr << "SCANNER "; yyerror(""); exit(1); }


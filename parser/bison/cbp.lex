/* Compilerbau Praktikum 2010 */
/* cbp.lex */

%{
#include "ExprNodes.h"
#include "ProgramNodes.h"

// bison generates .hh files in cpp mode
#include "cbp.tab.h"

#if defined (MSVC) || defined (WIN32)
int yylineno;
#endif
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

"func"          { Node::symbolTable->enterNewScope(); return KEY_FUNC; }
"call"          { return KEY_CALL; }
"type"          { return KEY_TYPE; }
"if"            { Node::symbolTable->enterNewScope(); return KEY_IF; }
"else"          { return KEY_ELSE; }
"switch"        { return KEY_SWITCH; }
"case"          { return KEY_CASE; }
"while"         { Node::symbolTable->enterNewScope(); return KEY_WHILE; }
"for"           { Node::symbolTable->leaveCurrentScope(); return KEY_FOR; }
"return"        { return KEY_RETURN; }
"abi_c"         { yylval.abi_val = Abi_c; return ABI; }
"abi_default"   { yylval.abi_val = Abi_default; return ABI; }

"uint8"         { yylval.type_val = new NodeType(Type_uint8);   return TYPE; }
"int8"          { yylval.type_val = new NodeType(Type_int8);    return TYPE; }
"uint16"        { yylval.type_val = new NodeType(Type_uint16);  return TYPE; }
"int16"         { yylval.type_val = new NodeType(Type_int16);   return TYPE; }
"uint32"        { yylval.type_val = new NodeType(Type_uint32);  return TYPE; }
"int32"         { yylval.type_val = new NodeType(Type_int32);   return TYPE; }
"float32"       { yylval.type_val = new NodeType(Type_float32); return TYPE; }
"float64"       { yylval.type_val = new NodeType(Type_float64); return TYPE; }
"void"          { yylval.type_val = new NodeType(Type_void);    return KEY_VOID; }
"local"         { return KEY_LOCAL; }


"struct"        { return KEY_STRUCT; }

"var"           { return KEY_VAR; }

"as"            { return KEY_AS; }

"{"             { Node::symbolTable->enterNewScope(); return CURLY_BRACKET_LEFT; }
"}"             { Node::symbolTable->leaveCurrentScope(); return CURLY_BRACKET_RIGHT; }
"("             { return PAR_LEFT; }
")"             { return PAR_RIGHT; }
"["             { return SQUARE_BRACKET_LEFT; }
"]"             { return SQUARE_BRACKET_RIGHT; }
":"             { return COLON; }
"+"             { yylval.string_val = new std::string(yytext); return PLUS; }
"-"             { yylval.string_val = new std::string(yytext); return SUBT; }
"*"             { yylval.string_val = new std::string(yytext); return MULT; }
"@"             { return AT; }
"$"             { return DOLLAR; }
"="             { return ASSIGN; }
"=="            { return EQ; }
"!="            { return NEQ; }
"<="            { return LE; }
">="            { return GE; }
"<"             { return LT; }
">"             { return GT; }
"||"            { return OR; }
"&&"            { return AND; }
"^^"            { return XOR; }
"<<"            { return BIT_LEFT; }
">>"            { return BIT_RIGHT; }
"|"             { return BIT_OR; }
"&"             { return BIT_AND; }
"^"             { return BIT_XOR; }
".."            { return DOTDOT; }

[ \t]*          { /* return WHITESPACE; */ }
{eol}           { yylineno++; /* return EOL; */ }
[#].*{eol}      { yylineno++; /* return LINE_COMMENT; */ }

{identifier}    { yylval.string_val = new std::string(yytext); return IDENTIFIER; }

.               { std::cerr << "SCANNER "; yyerror(""); exit(1); }


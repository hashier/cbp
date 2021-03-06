/* Compilerbau Praktikum 2010 */
/* cbp.lex */

%{
#include "ExprNodes.h"
#include "Variables.h"
#include "ProgramNodes.h"
#include "Types.h"

// bison generates .hh files in cpp mode
#include "cbp.tab.h"

#if defined (MSVC) || defined (WIN32)
int yylineno = 1; // first line number is 1
#endif
int yyerror(char const* s);

extern YYLTYPE yylloc;
#define YY_USER_INIT yylloc.first_line=1;
%}

/* Avoid one linker error related to yywrap */
/* http://stackoverflow.com/questions/1811125/undefined-reference-to-yywrap */
%option noyywrap

/* Removes warning */
/* lex.yy.c:1497: warning: ‘void yyunput(int, char*)’ defined but not used */
%option nounput

letter      [a-zA-Z]
digit       [0-9]
word        [a-zA-Z0-9_]*
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
"if"            { Node::symbolTable->enterNewScope(); return KEY_IF; }
"else"          { return KEY_ELSE; }
"switch"        { return KEY_SWITCH; }
"case"          { return KEY_CASE; }
"while"         { Node::symbolTable->enterNewScope(); return KEY_WHILE; }
"for"           { Node::symbolTable->enterNewScope(); return KEY_FOR; }
".."            { return KEY_DOTDOT; }
"by"            { return KEY_BY; }
"return"        { return KEY_RETURN; }
"abi_c"         { yylval.abi_val = Abi_c; return ABI; }
"abi_default"   { yylval.abi_val = Abi_default; return ABI; }

"uint8"         { yylval.type_val = new TypeSimple(Type_uint8);   return TYPE; }
"int8"          { yylval.type_val = new TypeSimple(Type_int8);    return TYPE; }
"uint16"        { yylval.type_val = new TypeSimple(Type_uint16);  return TYPE; }
"int16"         { yylval.type_val = new TypeSimple(Type_int16);   return TYPE; }
"uint32"        { yylval.type_val = new TypeSimple(Type_uint32);  return TYPE; }
"int32"         { yylval.type_val = new TypeSimple(Type_int32);   return TYPE; }
"float32"       { yylval.type_val = new TypeSimple(Type_float32); return TYPE; }
"float64"       { yylval.type_val = new TypeSimple(Type_float64); return TYPE; }
"void"          { yylval.type_val = new TypeVoid();               return KEY_VOID; }
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
"/"             { yylval.string_val = new std::string(yytext); return DIV; }
"%"             { yylval.string_val = new std::string(yytext); return MOD; }
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
"."             { return DOT; }

[ \t]*          { /* return WHITESPACE; */ }
{eol}           { yylineno++; yylloc.first_line++; /* return EOL; */ }
[#].*{eol}      { yylineno++; yylloc.first_line++; /* return LINE_COMMENT; */ }

{identifier}    { yylval.string_val = new std::string(yytext); return IDENTIFIER; }

.               { std::cerr << "SCANNER "; yyerror(""); exit(1); }


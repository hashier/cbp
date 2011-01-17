/* Compilerbau Praktikum */
/* cbp.y */

%{
#include "ExprNodes.h"
#include "Variables.h"
#include "ProgramNodes.h"
//#include <cstdio>

int yyerror(char const* s);
int yylex(void);

#define YYPARSE_PARAM parm
%}

%union{
	File*                   file_val;;
	Function*               func_val;
	Statement*              statement;
	Expression*             expr_val;
	LocalVariable*          var_val_loc;
	GlobalVariable*         var_val_glo;
	StructVariable*         var_val_str;
	Variable*               var_val;
	Block*                  block_val;
	SwitchCase::Case*       case_val;
	std::list<Variable*>*   var_list;
	std::list<Expression*>* expr_list;
	std::list<SwitchCase::Case*>* case_list;
	TypeDecl*               typeDecl_val;
	Type*                   type_val;
	BaseType                simpletype_val;
	Func_abi                abi_val;
	float                   float_val;
	int                     int_val;
	std::map<std::string, StructVariable*>*   struct_members_val;
	//TypeStruct*             struct_val;
	std::string*            string_val;
}

%start input

%token LETTER DIGIT WHITESPACE LINE_COMMENT EOL
%token KEY_FUNC KEY_CALL KEY_TYPE KEY_VOID KEY_WHILE KEY_STRUCT KEY_VAR KEY_LOCAL KEY_RETURN
%token CURLY_BRACKET_LEFT CURLY_BRACKET_RIGHT PAR_LEFT PAR_RIGHT COLON SEMICOLON
%token TYPE ABI AT SQUARE_BRACKET_RIGHT
%token KEY_FOR KEY_DOTDOT KEY_BY 

/* expressions (and some statements) need associativity and precedence */
%left KEY_SWITCH
%left KEY_CASE
%left KEY_IF
%left KEY_ELSE
%left ASSIGN
%left KEY_AS
%left EQ NEQ GE LE GT LT
%left OR AND XOR
%left BIT_LEFT BIT_RIGHT BIT_OR BIT_AND BIT_XOR
%left PLUS SUBT
%left MULT DIV MOD
%left DOT
%left DOLLAR

%nonassoc SQUARE_BRACKET_LEFT 

%token <int_val>    INTEGER_CONSTANT
%token <float_val>  FLOAT_CONSTANT
%token <string_val> IDENTIFIER

%type <file_val> input

%type <type_val>     type
%type <type_val>     TYPE
%type <type_val>     KEY_VOID
%type <func_val>     func_decl
%type <expr_val>     exp
%type <statement>    statement
%type <block_val>    st_block
%type <typeDecl_val> type_decl
%type <abi_val>      abi ABI
%type <var_val>      var
%type <var_val_loc>      var_decl_local
%type <var_val_glo>      var_decl_global
%type <var_val_str>      var_decl_struct

%type <var_list>     var_list
%type <expr_list>    exp_list

%type <case_val>    case
%type <case_list>   case_list

%type <struct_members_val> struct_members

%%

input: { $$ = *((File**) parm) = new File(); }
     | input type_decl { $1->add($2); }
     | input var_decl_global { $1->add($2); }
     | input func_decl { $1->add($2); }
     ;



func_decl: func_decl_start abi IDENTIFIER PAR_LEFT var_list PAR_RIGHT COLON type statement { Node::symbolTable->leaveCurrentScope(); $$ = new Function($3, $2, $5, $8, $9); }
         | func_decl_start abi IDENTIFIER PAR_LEFT var_list PAR_RIGHT COLON type { Node::symbolTable->leaveCurrentScope(); $$ = new Function($3, $2, $5, $8); }
         ;

func_decl_start: KEY_FUNC { Node::symbolTable->enterNewScope(); }
         ;

var: IDENTIFIER COLON type { $$ = new LocalVariable($1, $3); }
   ;

var_list:   /* empty */ { $$ = new std::list<Variable*>(); }
        | var_list var { $$->push_back($2); }
        ;

type_decl: KEY_TYPE IDENTIFIER COLON type { $$ = new TypeDecl($2, $4); }
         ;

type: TYPE
    | KEY_VOID
    | KEY_STRUCT CURLY_BRACKET_LEFT struct_members CURLY_BRACKET_RIGHT { $$ = new TypeStruct($3) }
    | IDENTIFIER { $$ = TypeDecl::getDeclaredType($1); }
    | SQUARE_BRACKET_LEFT SQUARE_BRACKET_RIGHT type { $$ = new TypePointer($3); }
    ;

struct_members: /* empty */ { $$ = new std::map<std::string, StructVariable*>(); }
              | struct_members var_decl_struct { (*$1)[$2->getIdentifier()] = $2; }
              | struct_members var_decl_struct AT INTEGER_CONSTANT { $2->setExplicitOffset($4); (*$1)[$2->getIdentifier()] = $2; }
              ;

var_decl_local:  KEY_VAR IDENTIFIER COLON type { $$ = new LocalVariable($2, $4); }
var_decl_struct: KEY_VAR IDENTIFIER COLON type { $$ = new StructVariable($2, $4); }
var_decl_global: KEY_VAR IDENTIFIER COLON type { $$ = new GlobalVariable($2, $4); }

case: KEY_CASE INTEGER_CONSTANT statement {  $$ = new SwitchCase::Case(new ConstInt($2), $3); }

case_list: /* empty */ { $$ = new std::list<SwitchCase::Case*>(); }
         | case_list case { $$->push_back($2); }
         ;

statement:   CURLY_BRACKET_LEFT st_block CURLY_BRACKET_RIGHT { $$ = $2 }
         | KEY_WHILE exp statement { Node::symbolTable->leaveCurrentScope(); $$ = new WhileLoop($2, $3); }
         | KEY_IF exp statement { Node::symbolTable->leaveCurrentScope(); $$ = new IfElse($2, $3, NULL); }
         | KEY_IF exp statement KEY_ELSE statement { Node::symbolTable->leaveCurrentScope(); $$ = new IfElse($2, $3, $5); }
         | exp { $$ = $1 }
         | KEY_LOCAL var_decl_local { $$ = new Local($2); } // TODO
         | KEY_RETURN exp { $$ = new Return($2) }
         | KEY_RETURN KEY_VOID { $$ = new Return(NULL) }
         | KEY_FOR IDENTIFIER ASSIGN exp KEY_DOTDOT exp statement            { Node::symbolTable->leaveCurrentScope(); $$ = new ForLoop($2,$4,$6,$7) }
         | KEY_FOR IDENTIFIER ASSIGN exp KEY_DOTDOT exp KEY_BY exp statement { Node::symbolTable->leaveCurrentScope(); $$ = new ForLoop($2,$4,$6,$8,$9) }
         | KEY_SWITCH exp case_list { $$ = new SwitchCase($2, $3) }
         ;

st_block: /* empty */ { $$ = new Block(); }
        | st_block statement { $1->add($2); }
        ;

abi:   /* empty */ { $$ = Abi_default; }
   | ABI
   ;

exp_list: /* empty */ { $$ = new std::list<Expression*>(); }
        | exp_list exp { $1->push_back($2); }
        ;

exp: INTEGER_CONSTANT	{ $$ = new ConstInt($1); }
   | FLOAT_CONSTANT	{ $$ = new ConstFloat($1); }
   | exp PLUS exp	{ $$ = new Expr_Add($1, $3); }
   | exp SUBT exp { $$ = new Expr_Sub($1, $3); }
   | exp MULT exp	{ $$ = new Expr_Mul($1, $3); }
   | exp DIV exp	{ $$ = new Expr_Div($1, $3); }
   | exp MOD exp	{ $$ = new Expr_Mod($1, $3); }
   | exp EQ exp       { $$ = new Expr_EQ($1, $3); }
   | exp NEQ exp      { $$ = new Expr_NEQ($1, $3); }
   | exp LE exp       { $$ = new Expr_LE($1, $3); }
   | exp GE exp       { $$ = new Expr_GE($1, $3); }
   | exp LT exp       { $$ = new Expr_LT($1, $3); }
   | exp GT exp       { $$ = new Expr_GT($1, $3); }
   | exp OR exp       { $$ = new Expr_BoolOR($1, $3); }
   | exp AND exp      { $$ = new Expr_BoolAND($1, $3); }
   | exp XOR exp      { $$ = new Expr_BoolXOR($1, $3); }
   | exp BIT_LEFT  exp { $$ = new Expr_BitLeft($1, $3); }
   | exp BIT_RIGHT exp { $$ = new Expr_BitRight($1, $3); }
   | exp BIT_OR exp    { $$ = new Expr_BitOR($1, $3); }
   | exp BIT_AND exp   { $$ = new Expr_BitAND($1, $3); }
   | exp BIT_XOR exp   { $$ = new Expr_BitXOR($1, $3); }
   | exp DOLLAR        { $$ = new Expr_Ref($1); }
   | exp DOT IDENTIFIER { $$ = new Expr_Struc($1, $3); }
   | exp SQUARE_BRACKET_LEFT SQUARE_BRACKET_RIGHT { $$ = new Expr_Deref($1); }
   | exp SQUARE_BRACKET_LEFT exp SQUARE_BRACKET_RIGHT { $$ = new Expr_Deref($1, $3); }
   | PAR_LEFT exp PAR_RIGHT { $$ = $2; }
   | IDENTIFIER        { $$ = new Expr_Identifier($1); }
   | KEY_CALL IDENTIFIER PAR_LEFT exp_list PAR_RIGHT { $$ = new FuncCall($2, $4);  }
   | KEY_AS type exp { $$ = new Expr_Cast($2, $3); }
   | exp ASSIGN exp { $$ = new Expr_Assign($1, $3); }
   ;

%%

int yyerror(std::string const& s)
{
	extern int yylineno;   // defined and maintained in lex.c
	extern char* yytext;  // defined and maintained in lex.c

	std::cerr << "ERROR: " << s << " at symbol \"" << yytext;
	std::cerr << "\" on line " << yylineno << std::endl;

    return 0;
}

int yyerror(char const* s)
{
	return yyerror(std::string(s));
}



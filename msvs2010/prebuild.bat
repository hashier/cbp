echo [Execute Bison]

cd bison
cd bin
bison.exe -d -l ..\..\..\parser\bison\cbp.y

echo [Rename] "cbp.tab.c" to "cbp.tab.cpp" 

del ..\..\..\parser\bison\cbp.cpp
move cbp.tab.c ..\..\..\parser\bison\cbp.cpp

del ..\..\..\parser\bison\cbp.tab.h
move cbp.tab.h ..\..\..\parser\bison\cbp.tab.h

cd..
cd..

echo [Execute Flex]

cd flex
cd bin
flex.exe -L -l ..\..\..\parser\bison\cbp.lex

cd..
cd..



echo [extern *yyin]
ssr 0 "FILE *yyin = (FILE *) 0" "extern FILE *yyin; FILE *yyin = (FILE *) 0" .\flex\bin\lex.yy.c 

echo [Correct To POSIX]
ssr 0 "extern int isatty" "extern ''C'' int __cdecl isatty" .\flex\bin\lex.yy.c 
REM ssr 0 "extern int isatty" "extern ''C'' int __cdecl isatty" .\flex\bin\lex.yy.c 
ssr 0 "isatty" "_isatty" .\flex\bin\lex.yy.c 
ssr 0 "fileno" "_fileno" .\flex\bin\lex.yy.c

echo [Correct x64 issues]
ssr 0 "int offset = yy_c_buf_p" "size_t offset = yy_c_buf_p" .\flex\bin\lex.yy.c
ssr 0 "static int yy_n_chars;" "static size_t yy_n_chars;" .\flex\bin\lex.yy.c
ssr 0 "int yy_n_chars;" "size_t yy_n_chars;" .\flex\bin\lex.yy.c
ssr 0 "register int number_to_move = yy_n_chars + 2;" "register size_t number_to_move = yy_n_chars + 2;" .\flex\bin\lex.yy.c


echo [Rename] "lex.yy.c" to "lex.yy.cpp" 

del ..\parser\bison\lex.yy.cpp
move .\flex\bin\lex.yy.c ..\parser\bison\lex.yy.cpp
echo [Execute Bison]
cd tools
cd bin
REM bison.exe -d -l ..\..\..\parser\bison\cbp.y
bison.exe -d ..\..\..\parser\bison\cbp.y

echo [Rename] "cbp.tab.c" to "cbp.tab.cpp" 
del ..\..\..\parser\bison\cbp.cpp
move cbp.tab.c ..\..\..\parser\bison\cbp.cpp
del ..\..\..\parser\bison\cbp.tab.h
move cbp.tab.h ..\..\..\parser\bison\cbp.tab.h

cd..
cd..

echo [Execute Flex]
cd tools
cd bin
REM flex.exe -L -l ..\..\..\parser\bison\cbp.lex
flex.exe ..\..\..\parser\bison\cbp.lex

cd..
cd..

REM echo [extern *yyin]
REM ssr 0 "FILE *yyin = (FILE *) 0" "extern FILE *yyin; FILE *yyin = (FILE *) 0" .\tools\bin\lex.yy.c 

echo [Correct To POSIX]
REM _CRTIMP int __cdecl _isatty(_In_ int _FileHandle)
ssr 0 "extern int isatty" "_CRTIMP int __cdecl isatty" .\tools\bin\lex.yy.c 
REM REM ssr 0 "extern int isatty" "extern ''C'' int __cdecl isatty" .\tools\bin\lex.yy.c 
ssr 0 "isatty" "_isatty" .\tools\bin\lex.yy.c 
ssr 0 "fileno" "_fileno" .\tools\bin\lex.yy.c

REM echo [Correct x64 issues]
ssr 0 "int offset = yy_c_buf_p" "size_t offset = yy_c_buf_p" .\tools\bin\lex.yy.c
ssr 0 "static int yy_n_chars;" "static size_t yy_n_chars;" .\tools\bin\lex.yy.c
ssr 0 "int yy_n_chars;" "size_t yy_n_chars;" .\tools\bin\lex.yy.c
ssr 0 "register int number_to_move = yy_n_chars + 2;" "register size_t number_to_move = yy_n_chars + 2;" .\tools\bin\lex.yy.c

echo [Rename] "lex.yy.c" to "lex.yy.cpp" 
del ..\parser\bison\lex.yy.cpp
move .\tools\bin\lex.yy.c ..\parser\bison\lex.yy.cpp
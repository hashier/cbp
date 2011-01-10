@echo off

echo [Delete old Object Files]
echo del CBP_main.o
echo del out.o
echo del CBP_main.exe
echo.

echo [Compile ASM Source]
cd gcc
cd bin
@echo on
"x86_64-w64-mingw32-gcc.exe" -m64 -c ..\..\out.s -o ..\..\out.o
@echo off
echo.
cd..
cd..

echo [Compile ASM Main]
cd gcc
cd bin
@echo on
"x86_64-w64-mingw32-gcc.exe" -m64 -c ..\..\..\cbp_main\cbp_main.c -o ..\..\CBP_main.o
@echo off
echo.
cd..
cd..

echo [Linking]
cd gcc
cd bin
@echo on
"x86_64-w64-mingw32-gcc.exe" -m64 ..\..\CBP_main.o ..\..\out.o ..\..\SDL\lib\libSDLmain.a ..\..\SDL\lib\libSDL.dll.a -o ..\..\CBP_main.exe
@echo off
echo.
cd..
cd..

PAUSE
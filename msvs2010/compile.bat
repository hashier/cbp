@echo off

echo [Delete old Object Files]
echo del CBP_main.o
echo del out.o
echo.

echo [Compile ASM Source]
cd gcc
cd bin
echo x86_64-w64-mingw32-gcc.exe -c -m64 -o ..\..\out.o ..\..\out.s
echo.
cd..
cd..

echo [Compile ASM Main]
cd gcc
cd bin
echo x86_64-w64-mingw32-gcc.exe -c -m64 -o ..\..\CBP_main.o ..\..\..\cbp_main\cbp_main.c
echo.
cd..
cd..

echo [Linking]

PAUSE
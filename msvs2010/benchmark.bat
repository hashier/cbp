@echo off

echo [Delete old Object Files]
echo del benchmark.o
echo.

echo [Compile benchmark Source]
cd gcc
cd bin
@echo on
echo x86_64-w64-mingw32-gcc.exe -m64 -c -o ..\..\benchmark.cpp ..\..\benchmark.o 
@echo off
echo.
cd..
cd..

echo [Linking]
cd gcc
cd bin
@echo on
"x86_64-w64-mingw32-gcc.exe" -m64 ..\..\..\benchmark\benchmark\BenchmarkUtils.h ..\..\benchmark.o ..\..\CBP_main.o ..\..\out.o ..\..\SDL\lib\libSDLmain.a ..\..\SDL\lib\libSDL.dll.a -o ..\..\benchmark.exe
@echo off
echo.
cd..
cd..

PAUSE
#include "main.h"
#include "ProgramNodes.h"
#include "MsgHandler.h"
#include "Optimizer.h"
#include "Benchmark.h"
#include "Peephole.h"

#include <stdlib.h>

#ifdef MSVC
#include <iostream>
#include <windows.h>
#include <shellapi.h>
#endif

// Hauptprogramm für das Compilerbaupraktikum
// ##########################################
//
// Parameter: 1. Quellcode-Datei zum Compilieren
int main(int argc, char *argv[])
{
    int error;

    MsgHandler::getInstance().printMessage("CompilerBauPraktikum");
    MsgHandler::getInstance().printMessage("--------------------");
    MsgHandler::getInstance().printMessage("");

    std::string fname;
    if(argc<2)
    {
        std::cout << " -warning: missing input file. Using default 'in'." << std::endl;
        fname = "in";
    } else {
        fname = argv[1];
    }

//-----------------------------------------------------------------------------
// Open File

    std::cout << "[Load File]" << std::endl;
    std::cout << " -file: " << fname << std::endl;

    //open file
    FILE *file = fopen(fname.c_str(), "r");
    if(file==NULL)
    {
        std::cout << " -error: file not found" << std::endl;
        return 2;
    }
    std::cout << " -done" << std::endl << std::endl;

    //pass file to flex input
    extern FILE *yyin;
    yyin = file;

//-----------------------------------------------------------------------------
// Create Parse Tree

    //yydebug = 1;
    std::cout << "[Parse File]" << std::endl;
    File* tree;
    error = yyparse((void*) &tree);
    if ( error) {
        std::cout << "Parse error -> Abort" << std::endl;
        exit ( 1);
    }
    std::cout << " -done" << std::endl << std::endl;

//-----------------------------------------------------------------------------
// Optimize Parse Tree

    std::cout << "[Optimize Parse Tree]" << std::endl;
    optimizeTree(tree);
    std::cout << " -done" << std::endl << std::endl;

//-----------------------------------------------------------------------------
// Generate ASM Source

    std::cout << "[Generate ASM-Source]" << std::endl;

    //check for underscore
    int withUnderscore = -1;
    if(argc > 2) {
        std::string arg = argv[2];
        if(!arg.compare("nounderscore")) {
            withUnderscore = 0;
        }
        else if(!arg.compare("underscore")) {
            withUnderscore = 1;
        }
    }
    if(withUnderscore == -1) { //generating default values
#ifdef APPLE
        withUnderscore = 1;
#endif
#ifdef UNIX
        withUnderscore = 0;
#endif
#ifdef WIN32
        withUnderscore = 0;
#endif
    }

    //set filename for asm source file
    std::stringstream ss;
#ifdef MSVC
    ss << "out.s";
#else
    ss << fname.substr(fname.find_last_of("/")+1) << ".s";
#endif
    std::cout << " -outfile: " << ss.str() << std::endl;

    //generate asm source from parse tree
    CodeGen* out = new CodeGen(ss.str().c_str(), fname, withUnderscore);
    tree->gen(out);
    std::cout << " -done" << std::endl << std::endl;

    delete(tree);

//-----------------------------------------------------------------------------
// Optimize assembler output

    std::cout << "[Optimize ASM-Source]" << std::endl;
    optimizePeephole(ss.str());
    std::cout << " -done" << std::endl << std::endl;

#ifdef MSVC
//-----------------------------------------------------------------------------
// Generate ASM Source

    std::cout << "[Compile ASM-Source]" << std::endl;
    ShellExecute(NULL, "open", "compile.bat", NULL, NULL, SW_SHOWNORMAL);
    std::cout << " -done" << std::endl << std::endl;
  
    std::cout << "\nPress Any Key To Continue" << std::endl;
    getchar();
#endif
//-----------------------------------------------------------------------------
// Benchmark
    std::cout << " -benchmark: " << std::endl;
    Benchmark bench("benchmark.cpp");
    bench.writeBenchmarkFiles(20);
	std::cout << " -done" << std::endl << std::endl;
#ifdef MSVC
    ShellExecute(NULL, "open", "benchmark.bat", NULL, NULL, SW_SHOWNORMAL);
	std::cout << "Press Any Key To Continue" << std::endl;
    getchar();
#endif
    return 0;
}

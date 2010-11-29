#include "main.h"
#include "include/parser/ProgramNodes.h"
#include "include/msghandler/MsgHandler.h"

// Hauptprogramm für das Compilerbaupraktikum
// ##########################################
//
// Parameter: 1. Quellcode-Datei zum Compilieren
int main(int argc, char *argv[])
{
    MsgHandler::getInstance().printMessage("CompilerBauPraktikum");
    MsgHandler::getInstance().printMessage("--------------------");

    std::string fname;
    if(argc<2)
    {
        std::cout << " -warning: missing input file. Using default 'in'." << std::endl;
        fname = "in";
    } else {
        fname = argv[1];
	}

//-----------------------------------------------------------------------------
//Open File

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
//Parse File

    //yydebug = 1;
    std::cout << "[Parse File]" << std::endl;
    File* tree;
    yyparse((void*) &tree);
    std::cout << " -done" << std::endl << std::endl;

//-----------------------------------------------------------------------------
//Generate ASM Source

    std::cout << "[Generate ASM-Source]" << std::endl;
    CodeGen* out = new CodeGen("out.s");
    tree->gen(out);
    std::cout << " -done" << std::endl << std::endl;

    delete(tree);

#ifdef MSVC
    std::cout << "Press Any Key To Continue" << std::endl;
    getchar();
#endif

    return 0;
}

#include "main.h"
#include "ProgramNodes.h"
#include "MsgHandler.h"

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
// Parse File

    //yydebug = 1;
    std::cout << "[Parse File]" << std::endl;
    File* tree;
    yyparse((void*) &tree);
    std::cout << " -done" << std::endl << std::endl;

//-----------------------------------------------------------------------------
// Generate ASM Source

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

    std::cout << "[Generate ASM-Source]" << std::endl;
    CodeGen* out = new CodeGen("out.s", fname, withUnderscore);
    tree->gen(out);
    std::cout << " -done" << std::endl << std::endl;

    delete(tree);

#ifdef MSVC
    std::cout << "Press Any Key To Continue" << std::endl;
    getchar();
#endif

    return 0;
}

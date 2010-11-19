#include "main.h"
#include "include/parser/ProgramNodes.h"

// Hauptprogramm für das Compilerbaupraktikum
// ##########################################
//
// Parameter: 1. Quellcode-Datei zum Compilieren
int main(int argc, char *argv[])
{
    std::cout << "CompilerBauPraktikum" << std::endl;
    std::cout << "--------------------" << std::endl << std::endl;

    std::string fname;
    if(argc<2)
    {
        std::cout << " -warning: missing input file. Using default 'in'." << std::endl;
        fname = "in";
    } else
        fname = argv[1];

    std::cout << "[Load File]" << std::endl << std::endl;
    std::cout << " -input file: " << fname << std::endl;

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

    //yydebug = 1;
    //parse file
    std::cout << "[Parse File]" << std::endl;
    File* tree;
    yyparse((void*) &tree);

    CodeGen* out = new CodeGen("out.asm");
    tree->gen(out);

    std::cout << " -done" << std::endl << std::endl;

    delete tree;
    tree = 0;

#if defined (MSVC)
    getchar();
#endif

    return 0;
}

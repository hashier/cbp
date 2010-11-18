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

    if(argc<2)
    {
        std::cout << " -error: missing input file" << std::endl;
        return 1;
    }

    std::cout << "[Load File]" << std::endl << std::endl;
    std::cout << " -input file: " << argv[1] << std::endl;

    //open file
    FILE *file = fopen(argv[1], "r");
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
    tree->dump();
    std::cout << " -done" << std::endl << std::endl;

#if defined (MSVC)
    getchar();
#endif

    return 0;
}

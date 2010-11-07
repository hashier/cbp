#include "main.h"

// Hauptprogramm für das Compilerbaupraktikum
// ##########################################
//
// Parameter: 1. Quellcode-Datei zum Compilieren
int main(int argc, char *argv[])
{
	std::cout << "CompilerBauPraktikum" << std::endl;
	std::cout << "--------------------" << std::endl << std::endl;

	if (argc >= 2)
	{
		std::cout << "-input file: " << argv[1] << std::endl;
	} else
	{
		std::cout << "-error: missing input file" << std::endl;
		return 1;
	}

	//open file
	FILE *file = fopen(argv[1], "r");
	if(file==NULL)
	{
		std::cout << "-error: file not found" << std::endl;
		return 2;
	}

	//pass file to flex input
	extern FILE *yyin;
	yyin = file;

	//parse file
	yyparse();

	return 0;
}

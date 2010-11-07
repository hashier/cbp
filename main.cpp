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
		std::cout << "Parameter1: " << argv[1] << std::endl;
	} else
	{
		std::cout << "Missing input file." << std::endl;
		return 1;
	}

	// Quelldatei parsen
	yyparse();

	return 0;
}

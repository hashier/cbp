#include "main.h"


using namespace std;


// Hauptprogramm für das Compilerbaupraktikum
// ##########################################
//
// Parameter: 1. Quellcode-Datei zum Compilieren
int main(int argc, char *argv[])
{
	cout << "CompilerBauPraktikum" << endl;
	cout << "--------------------" << endl << endl;

	if (argc >= 2)
	{
		cout << "Parameter1: " << argv[1] << endl;
	} else
	{
		cout << "Missing input file." << endl;
		return 1;
	}

	// Quelldatei parsen
	yyparse();

	return 0;
}

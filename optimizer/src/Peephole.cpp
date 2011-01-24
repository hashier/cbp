#include "Peephole.h"

#include <string>
#include <iostream>

void backupFile(FILE *file) {

    char buffer[4096];
    FILE *backupFile;

    // Make sure we have a backup of the original asm source
    backupFile = fopen("peephole_backup", "w+");

    if(backupFile==NULL)
        std::cout << " -error: file not found " << "peephole_backup" << std::endl;

    while(!feof(file))
    {
       fgets (buffer , 4096 , file);
       fputs (buffer , backupFile);
    }

    fclose(backupFile);
}

void optimizePeephole(std::string fname) {

    FILE *file;

    std::cout << " -Peephole" << std::endl;

    // Open generated assembler output file read-write to optimize in-place
    file = fopen(fname.c_str(), "r+");

    if(file==NULL)
        std::cout << " -error: file not found " << fname << std::endl;

    backupFile(file);

    fclose(file);
}

#pragma once

#include "Mnemonics.h"

#include<iostream>
#include<fstream>
#include<string>

class CodeGen {
    std::ofstream output;
    int mark_counter;
    std::string inputFileName;
    int withUnderscore;

    public:
        CodeGen(const char* fname, const std::string inputFileName, int withUnderscore)
            : output(fname), mark_counter(0),
            inputFileName(inputFileName), withUnderscore(withUnderscore) {
        }

        Label newMark(std::string name, bool func = false);

        std::string getInputFileName() {
            return inputFileName;
        }

        int isWithUnderscore() {
            return withUnderscore;
        }

        std::ostream& outputStream(){
            return output;
        }

};

CodeGen& operator<<(CodeGen& cg, Mnemonic const& mnemonic);


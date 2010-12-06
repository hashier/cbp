#pragma once

#include<iostream>
#include<fstream>
#include<string>

typedef std::string Mark;

class CodeGen : public std::ostream {
    int mark_counter;
    std::string inputFileName;
    int withUnderscore;

    public:
        CodeGen(const char* fname, const std::string inputFileName, int withUnderscore) : std::ostream(NULL), mark_counter(0), inputFileName(inputFileName), withUnderscore(withUnderscore) {
            std::filebuf* fbuf = new std::filebuf();
            fbuf->open(fname, std::ios::out);
            rdbuf(fbuf);
        }

        Mark newMark(std::string name);

        std::string getInputFileName() {
            return inputFileName;
        }

        int isWithUnderscore() {
            return withUnderscore;
        }

};


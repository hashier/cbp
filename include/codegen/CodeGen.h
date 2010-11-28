#pragma once

#include<iostream>
#include<fstream>
#include<string>

typedef std::string Mark;

class CodeGen : public std::ostream {
    int mark_counter;

    public:
        CodeGen(const char* fname) : std::ostream(NULL), mark_counter(0) {
            std::filebuf* fbuf = new std::filebuf();
            fbuf->open(fname, std::ios::out);
            rdbuf(fbuf);
        }

        Mark newMark(std::string name);

};


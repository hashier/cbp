#pragma once

#include<sstream>
#include<iostream>
#include<fstream>
#include<map>
#include<string>

class Mark {
    std::string name;

    public:
        static int counter;
        Mark(std::string name) {
            std::stringstream ss(name);
            ss << (counter++);
            this->name = ss.str();
        }
};

class CodeGen : public std::ostream {

    public:
        CodeGen(const char* fname) : std::ostream(NULL) {
            std::filebuf* fbuf = new std::filebuf();
            fbuf->open(fname, std::ios::out);
            rdbuf(fbuf);
        }

        static std::map<std::string, Mark*> marks;
        static Mark mark(std::string name);

};


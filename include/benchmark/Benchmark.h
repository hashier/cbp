#pragma once

#include<iostream>
#include<fstream>
#include<string>
#include"CodeGen.h"

class Benchmark {
    std::string filename;
    public:
        Benchmark(std::string filename);
        void writeBenchmarkFiles(int k);
};


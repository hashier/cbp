#include "Benchmark.h"
#include<sstream>

Benchmark::Benchmark(std::string filename)
{
    this->filename = filename;
}

void Benchmark::writeBenchmarkFiles(int k)
{
    //open file
    std::ofstream output(filename.c_str());

    //write file
    output << "#include \"BenchmarkUtils.h\"" << std::endl;
    output << "#include <iostream>" << std::endl;
    output << std::endl;
    output << "//forward declaration" << std::endl;
    output << "void _cbp_main();" << std::endl;
    output << std::endl;
    output << "int main(int argc, char* argv[])" << std::endl;
    output << "{" << std::endl;
    output << "    " << "int k = " << k << ";" << "//iterations" << std::endl;
    output << "    " << "double times[" << k << "];" << std::endl;
    output << "    " << "double avg = 0;" << "//average" << std::endl;
    output << "    " << "double med = 0;" << "//medium" << std::endl;
    output << "    " << "double min = 0;" << "//minimum" << std::endl;
    output << "    " << "double max = 0;" << "//maximum" << std::endl;
    output << "    " << "double dev = 0;" << "//deviation" << std::endl;
    output << "    " << "double tempTime = 0;" << "//temporary time" << std::endl;
    output << std::endl;
    output << "    " << "for(unsigned int n = 0; n<k; n++)" << std::endl;
    output << "    " << "{" << std::endl;
    output << "    " << "    " <<"tempTime = getTimeInSeconds();" << std::endl;

    output << std::endl;
    output << "    " << "    " <<"//call asm main" << std::endl;
    output << "    " << "    " << "_cbp_main();" << std::endl;
    output << std::endl;

    output << "    " << "    " <<"times[n] = getTimeInSeconds()-tempTime;" << std::endl;
    output << "    " << "    " <<"avg += times[n];" << std::endl;
    output << "    " << "}" << std::endl;
    output << std::endl;
    output << "    " << "quicksort(&times[0],0,k-1);" << std::endl;
    output << "    " << "med = times[k/2];" << std::endl;
    output << "    " << "min = times[0];" << std::endl;
    output << "    " << "max = times[k-1];" << std::endl;
    output << "    " << "avg /= (double)k;" << std::endl;
    output << std::endl;
    output << "    " << "for(unsigned int i = 0; i<k; i++)" << std::endl;
    output << "    " << "{" << std::endl;
    output << "    " << "    " << "dev += ((times[i]-avg)*(times[i]-avg));" << std::endl;
    output << "    " << "}" << std::endl;
    output << std::endl;
    output << "    " << "dev /= (double)(k-1);" << std::endl;
    output << "    " << "dev = sqrt(dev);" << std::endl;
    output << std::endl;
    output << "    " << "std::cout << \"minimum: \" << min << std::endl << \"avarage: \"<< avg << std::endl << \"median: \" << med << std::endl << \"deviation: \" << dev << std::endl << \"maximum: \" << max << std::endl;" << std::endl;
    output << std::endl;
    output << "    " << "std::cout << \"Press Any Key To Continue\" << std::endl;" << std::endl;
    output << "    " << "getchar();" << std::endl;
    output << "    " << "return 0;" << std::endl;
    output << "}" << std::endl;

    //close file
    output.close();
}

//
// Created by esha-naveed on 5/14/26.
//

#ifndef DA2026_PROJ2_T06_G04_OUTPUT_FILE_H
#define DA2026_PROJ2_T06_G04_OUTPUT_FILE_H
#include <string>
using namespace std;
#include "dataStruct.h"
#include "graph.h"

class output_file {
public:
    static void createOutputFile(const string& fileName, Graph<Web>& g, int numReg);
};


#endif //DA2026_PROJ2_T06_G04_OUTPUT_FILE_H

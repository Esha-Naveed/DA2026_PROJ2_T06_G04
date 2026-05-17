//
// Created by esha-naveed on 5/14/26.
//
/**
 * @file output_file.h
 * @brief Declarations for exporting register allocation results to an output file.
 * @details Provides utility structures to stream the structural map of webs,
 * program lines, and physical register bindings directly into text targets.
 */

#ifndef DA2026_PROJ2_T06_G04_OUTPUT_FILE_H
#define DA2026_PROJ2_T06_G04_OUTPUT_FILE_H
#include <string>
using namespace std;
#include "dataStruct.h"
#include "graph.h"

/**
 * @class output_file
 * @brief Class responsible for generating and formatting the final allocation output report.
 */

class output_file {
public:
    /**
     * @brief Generates a structured log file detailing total webs, associated lines, and register/memory mappings.
     * @timecomplexity O(V log V + V * P) where V is the number of vertices (webs) being sorted, and P is the average
     * number of program points printed per web.
     * @spacecomplexity O(V) auxiliary footprint to copy and manipulate the graph's vertex subset vector.
     * @param fileName Destination path where the output file will be created.
     * @param g Reference to the completed web interference graph.
     * @param numReg Total number of physical hardware registers available.
     */
    static void createOutputFile(const string& fileName, Graph<Web>& g, int numReg);
};


#endif //DA2026_PROJ2_T06_G04_OUTPUT_FILE_H

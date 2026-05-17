#include <iostream>
/*this will have arguments for teh batch mode, to open directly the folder with input files and call output_file.cpp to run
 *function with the purpose of placing the output file in its specific location
 * else it cals on the menu function in its own file to show the gui and run that.
 */

#include <iostream>
#include <string>
#include "menu.h"
#include "dataStruct.h"
#include "parser.h"
#include "graph.h"
#include "createGraph.h"
#include "regAlloc.h"
#include "output_file.h"

using namespace std;

void runBatchMode(int argc, char* argv[], ProjData& data) {
    if (argc < 5) {
        cout << "[Error] Missing required arguments." << endl;
        return;
    }

    data.rangesFile = argv[2];
    data.regsFile = argv[3];
    data.outputFile = argv[4];

    cout << "[Batch] Starting process " << endl;

    // 1. parser(rangesFile, regsFile, outputFile);
    if (!parseRegsFile(data) || !parseRangesFile(data)) {
        cout << "[Batch Error] Failed to parse input files." << endl;
        return;
    }

    // 2. buildGraph(data);
    Graph<Web> g = createGraph::buildGraph(data);

    // 3. run algorithm (default basic and chose another if given an error)
    if (data.algorithmType == "basic") regAlloc::baseAllocation(g, data.numReg);
    else if (data.algorithmType == "spilling") regAlloc::spilling(g, data.numReg, data.algoParam);
    else if (data.algorithmType == "splitting") {
        int countSplit = 0;
        regAlloc::splitting(g, data.numReg, data.algoParam, countSplit);
    }

    // 4. outputData (outputFile, data);
    output_file::createOutputFile(data.outputFile, g, data.numReg);
    cout << "[Batch] Done. Results saved to " << data.outputFile << endl;
}


int main(int argc, char* argv[]) {
    ProjData data;
    if (argc >= 5 && string(argv[1]) == "-b") {
        runBatchMode(argc, argv, data);
    }
    else {
        handleInteractiveMode(data);
    }

    return 0;
}
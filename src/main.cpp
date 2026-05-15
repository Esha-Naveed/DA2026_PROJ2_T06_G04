#include <iostream>
/*this will have arguments for teh batch mode, to open directly the folder with input files and call output_file.cpp to run
 *function with the purpose of placing the output file in its specific location
 * else it cals on the menu function in its own file to show the gui and run that.
 */

#include <iostream>
#include <string>
#include "menu.h"
#include "dataStruct.h"

using namespace std;

void runBatchMode(int argc, char* argv[], ProjData& data) {
    data.rangesFile = argv[2];
    data.regsFile = argv[3];
    data.outputFile = argv[4];

    cout << "[Batch] Starting process " << endl;

    // 1. parser(rangesFile, regsFile, outputFile);

    // 2. buildGraph(data);

    // 3. run algorithm (default basic and chose another if given an error)

    // 4. outputData (outputFile, data);

    cout << "[Batch] Done. Results daved to " << data.outputFile << endl;
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
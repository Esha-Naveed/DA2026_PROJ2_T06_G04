//
// Created by esha-naveed on 5/14/26.
//
/*will contain the function to print the gui interface and call specific functions based in the instructions
 */
/**
* @file menu.cpp
* @brief Implementation of the interactive menu interface.
*/

#include "menu.h"
#include "parser.h"
#include <iostream>

#include "regAlloc.h"
#include "dataStruct.h"
#include "graph.h"
#include "createGraph.h"
#include "output_file.h"

using namespace std;

void displayMenu() {
    cout << "\n--- Register Allocation Tool ---" << endl;
    cout << "1. Load Input Files (Ranges & Registers)" << endl;
    cout << "2. Build Interference Graph" << endl;
    cout << "3. Basic Register Allocation" << endl;
    cout << "4. Allocation with Web Spilling" << endl;
    cout << "5. Allocation with Web Splitting" << endl;
    cout << "6. Custom Allocation (Free version)" << endl;
    cout << "0. Exit" << endl;
    cout << "Selection: ";
}

void promptOutputFile(ProjData& data) {
    if (!data.outputFile.empty()) return;

    const string OUTPUT_DIR = "basic/basic/output/";
    string outFilename;

    cout << "Enter output filename: ";
    cin >> outFilename;
    data.outputFile = OUTPUT_DIR + outFilename;
}

void handleInteractiveMode(ProjData& data) {
    int choice = -1;
    Graph<Web> g;
    bool graphBuilt = false;
    while (choice != 0) {
        displayMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        if (choice >= 2 && choice <= 6) {
            if (data.rangesFile.empty() || data.regsFile.empty()) {
                cout << "[Error]: load input files first (1.)" << endl;
                continue;
            }
        }

        switch (choice) {
            case 1: {
                // paths
                const string RANGES_DIR = "basic/basic/ranges/";
                const string REGS_DIR = "basic/basic/registers/";
                string filename;

                cout << "Enter live ranges filename: ";
                cin >> filename;
                data.rangesFile = RANGES_DIR + filename;

                cout << "Enter registers filename: ";
                cin >> filename;
                data.regsFile = REGS_DIR + filename;

                // parser(data);
                if (parseRegsFile(data) && parseRangesFile(data)) {
                    cout << "[Success] Data loaded cleanly. Found " << data.allWebs.size() << " variable webs." << endl;
                    graphBuilt = false;
                } else {
                    cout << "[Warning] Error parsing inputs. Please check file paths." << endl;
                }
                break;
            }
            case 2:
                // build interference graph
                g = createGraph::buildGraph(data);
                graphBuilt = true;
                cout << "[Success] Graph built." << endl;
                break;
            case 3:
                if (!graphBuilt) {
                    cout << "[Error]" << endl;
                    break;
                }

                if (regAlloc::baseAllocation(g, data.numReg)) {
                    cout << "Success!" << endl;
                } else {
                    cout << "Error. Requires Splitting or Spilling" << endl;
                }

                cout << "Enter output file path: ";
                cin >> data.outputFile;
                data.algorithmType = "basic";

                // run basic coloring
                output_file::createOutputFile(data.outputFile, g, data.numReg);
                break;

            case 4: {
                if (!graphBuilt) {
                    cout << "[Error]" << endl;
                    break;
                }
                promptOutputFile(data);

                int k;
                cout << "Enter max number of webs to spill: ";
                cin >> k;
                data.algoParam = k;
                data.algorithmType = "spilling";

                // call spilling function
                regAlloc::spilling(g, data.numReg, data.algoParam);
                output_file::createOutputFile(data.outputFile, g, data.numReg);
                break;
            }
            case 5: {
                if (!graphBuilt) {
                    cout << "[Error]" << endl;
                    break;
                }
                promptOutputFile(data);

                int k;
                cout << "Enter max number of webs to split: ";
                cin >> k;
                data.algoParam = k;
                data.algorithmType = "splitting";
                // call splitting function
                int countSplit = 0;
                regAlloc::splitting(g, data.numReg, data.algoParam, countSplit);
                output_file::createOutputFile(data.outputFile, g, data.numReg);
                break;
            }
            case 6:
                promptOutputFile(data);
                data.algorithmType = "custom";
                // our own approach
                break;
            case 0:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice" << endl;
        }
    }
}

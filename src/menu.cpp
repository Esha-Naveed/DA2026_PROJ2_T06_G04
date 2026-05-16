//
// Created by esha-naveed on 5/14/26.
//
/*will contain the function to print the gui interface and call specific functions based in the instructions
 */
#include "menu.h"
#include "parser.h"
#include <iostream>

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

void handleInteractiveMode(ProjData& data) {
    int choice = -1;
    while (choice != 0) {
        displayMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        switch (choice) {
            case 1:
                cout << "Enter live ranges file path: ";
                cin >> data.rangesFile;
                cout << "Enter registers file path: ";
                cin >> data.regsFile;

                // parser(data);
                if (parseRegsFile(data) && parseRangesFile(data)) {
                    cout << "[Success] Data loaded cleanly. Found " << data.allWebs.size() << " variable webs." << endl;
                } else {
                    cout << "[Warning] Error parsing inputs. Please check file paths." << endl;
                }
                break;
            case 2:
                // build interference graph
                break;
            case 3:
                if (data.rangesFile.empty()) {
                    cout << "Error: Ranges file path is empty." << endl;
                    break;
                }
                else if (data.regsFile.empty()) {
                    cout << "Error: Registers file path is empty." << endl;
                    break;
                }
                cout << "Enter output file path: ";
                cin >> data.outputFile;
                data.algorithmType = "basic";

                // run basic coloring
                break;
            case 4: {
                int k;
                cout << "Enter max number of webs to spill: ";
                cin >> k;
                data.algoParam = k;
                data.algorithmType = "spilling";
                // call spillinh function
                break;
            }
            case 5: {
                int k;
                cout << "Enter max number of webs to split: ";
                cin >> k;
                data.algoParam = k;
                data.algorithmType = "splitting";
                // call splitting function
                break;
            }
            case 6:
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
//
// Created by esha-naveed on 5/14/26.
//
/*to create functions that read the final results and create the output files in their given destination*/
#include "output_file.h"

#include <algorithm>
#include <iostream>
#include <fstream>
using namespace std;

void output_file::createOutputFile(const string &fileName, Graph<Web>& g, int numReg) {
    ofstream outFile(fileName);
    if (!outFile.is_open()) {
        cout<<"Error opening output file: "<< fileName <<endl;
        return;
    }

    auto vertices = g.getVertexSet();

    //sorting vertices by ID, suing lambda
    sort(vertices.begin(), vertices.end(), [](Vertex<Web>* a, Vertex<Web>* b) {
        return a->getInfo().id < b->getInfo().id;
    });

    //check if allocation failed or not
    bool allocFail = false;
    for (auto v : vertices) {
        int reg = v->getInfo().assignedRegister;
        //if not spilled and valid
        if (reg == -1) {
            allocFail = true;
            break;
        }
    }

    if (allocFail) cout << "[Error] No number of registers." << endl;
    cout << "# Total number of webs followed by the listing of the program points of each one" << endl;
    cout << "# program points in each web are sorted in ascending order" << endl;

    //writing list of webs list
    outFile << "webs: " <<vertices.size() << "\n";
    for (auto v : vertices) {
        Web w = v->getInfo();
        outFile << "web" << w.id << ": " ;

        size_t count = 0;
        for (auto point : w.progPoints) {
            outFile << point;
            count++;
            if (count < w.progPoints.size()) outFile << ",";
        }
        outFile << "\n";
    }

    //writing list for registers
    if (allocFail) {
        outFile << "registers: 0\n";
        for (auto v : vertices) outFile << "M: web" << v->getInfo().id << "\n";
    } else {
        int regUsed = 0;
        for (auto v : vertices) {
            if (v->getInfo().assignedRegister > regUsed) regUsed = v->getInfo().assignedRegister;
        }

        //if no reg used cuz of spilling
        outFile << "registers: " << regUsed << "\n";

        for (auto v : vertices) {
            Web w = v->getInfo();
            if (w.assignedRegister == -2 || w.assignedRegister == -1) outFile << "M: web" << w.id << "\n";
            else outFile << "r" << w.assignedRegister-1 << ": web" << w.id << "\n";
        }
    }

    outFile.close();
}
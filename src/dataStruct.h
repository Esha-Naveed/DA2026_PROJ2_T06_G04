//
// Created by Clara Correia on 15/05/2026.
//

#ifndef DA2026_PROJ2_T06_G04_DATASTRUCT_H
#define DA2026_PROJ2_T06_G04_DATASTRUCT_H

#include <string>
#include <vector>
#include <set>
#include <map>

struct Web {
    int id;
    std::string varName;
    std::set<int> progPoints; // all lines where this web is alive
    int assignedRegister = -1;

    // finding specific webs
    bool operator==(const Web& other) const {
        return this->id == other.id;
    }
};

struct ProjData {
    // file paths
    std::string rangesFile;
    std::string regsFile;
    std::string outputFile;

    // settings for algorithms
    int numReg = 0;
    std::string algorithmType; // basic, spilling, splitting, custom
    int algoParam = 0; // k for spilling and splitting

    // parsed data
    std::map<int, Web> allWebs;
};

struct LiveRange {
    std::string varName;
    std::vector<int> lines;
    bool startsWith; // +
    bool endsWith; // -
};

// class InterferenceGraph : public Graph<Web> {};

#endif //DA2026_PROJ2_T06_G04_DATASTRUCT_H
//
// Created by esha-naveed on 5/14/26.
//
/*read input files and parse data*/
#include "parser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

string cleanLine(string line) {
    size_t commentPos = line.find('#');
    if (commentPos != string::npos) {
        return line.substr(0, commentPos);
    }
    return line;
}

bool parseRegsFile(ProjData &data) {
    ifstream file(data.regsFile);
    if (!file.is_open()) {
        cerr << "[Error] Failed to open registers file" << data.regsFile << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        line = cleanLine(line);
        if (line.empty()) continue;

        stringstream ss(line);
        string key;

        if (getline(ss, key, ':')) {
            if (key.find("registers") != string::npos) {
                ss >> data.numReg;
            }
            else if (key.find("algorithm") != string::npos){
                string algo;
                getline(ss, algo, ',');
                algo.erase(remove_if(algo.begin(), algo.end(), ::isspace), algo.end());
                data.algorithmType = algo;

                if (ss) {
                    ss >> data.algoParam;
                }
            }
        }
    }

    return true;
}

bool parseRangesFile(ProjData &data) {
    ifstream file(data.rangesFile);
    if (!file.is_open()) {
        cerr << "[Error] Failed to open live ranges file" << data.rangesFile << endl;
        return false;
    }

    string line;
    string currentVar = "";
    vector<LiveRange> temporaryRanges;

    while (getline(file, line)) {
        line = cleanLine(line);
        if (line.empty()) continue;

        stringstream ss(line);
        string varPart;
        getline(ss, varPart, ':');

        // check valid variable name
        string trimmedVar = varPart;
        trimmedVar.erase(remove_if(trimmedVar.begin(), trimmedVar.end(), ::isspace), trimmedVar.end());

        if (!trimmedVar.empty()) {
            currentVar = trimmedVar;
        }

        string token;
        LiveRange range;
        range.varName = currentVar;
        range.startsWith = false;
        range.endsWith = false;

        while (getline(ss, token, ',')) {
            if (token.empty()) continue;

            // check flags
            if (token.back() == '+') {
                range.startsWith = true;
                token.pop_back();
            } else if (token.back() == '-') {
                range.endsWith = true;
                token.pop_back();
            }

            range.lines.push_back(stoi(token));
        }

        if (!range.lines.empty()) {
            temporaryRanges.push_back(range);
        }
    }

    // process temporary list of individual ranges and group them into Webs
    mergeRangesIntoWebs(temporaryRanges, data);
    return true;
}

void mergeRangesIntoWebs(const std::vector<LiveRange> &rawRanges, ProjData &data) {
    int webIdCounter = 0;

    // group raw ranges by variable name
    map<string, vector<LiveRange>> rangesByVar;
    for (const auto& r : rawRanges) {
        rangesByVar[r.varName].push_back(r);
    }

    // perform greedy merging of overlapping execution intervals
    for (auto& pair : rangesByVar) {
        const string& varName = pair.first;
        vector<LiveRange>& ranges = pair.second;

        vector<set<int>> mergedWebPoints;

        for (const auto& r : ranges) {
            set<int> currentSet(r.lines.begin(), r.lines.end());
            bool merged = false;

            // check for shared lines
            for (auto& existingWeb : mergedWebPoints) {
                bool overlaps = false;
                for (int point : currentSet) {
                    if (existingWeb.count(point)) {
                        overlaps = true;
                        break;
                    }
                }

                if (overlaps) {
                    existingWeb.insert(currentSet.begin(), currentSet.end());
                    merged = true;
                    break;
                }
            }

            if (!merged) {
                mergedWebPoints.push_back(currentSet);
            }
        }

        // turn the sets into web instances inside data.allWebs
        for (const auto& pointSet : mergedWebPoints) {
            Web newWeb;
            newWeb.id = webIdCounter++;
            newWeb.varName = varName;
            newWeb.progPoints = pointSet;
            newWeb.assignedRegister = -1;

            data.allWebs[newWeb.id] = newWeb;
        }
    }
}


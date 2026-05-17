//
// Created by esha-naveed on 5/14/26.
//
/*looks in the LiveRanges and adds data to data.allWebs*/

/**
* @file webProcessing.cpp
* @brief Algorithmic implementation of disjoint interval merging.
*/


#include "webProcessing.h"
#include <algorithm>
#include <map>
#include <set>

using namespace std;

void webProcessing::buildWebs(const std::vector<LiveRange>& ranges, ProjData& data) {
    //first clearing so no errors, starting with a clean slate
    data.allWebs.clear();

    int webCounter = 0;

    //grouping raw ranges by variables
    map<string, vector<LiveRange>> groups;
    //adding variables and their names to groups
    for (const auto& r : ranges) {
        groups[r.varName].push_back(r);
    }

    /*where processing starts*/

    //currently we have groups of names in string part of the map
    for (auto& [varName, varRanges] : groups) {
        //gathering all points for each variable (var during interation)

        /*
        Web newWeb;
        newWeb.id = webCounter++;
        newWeb.varName = varName;
        newWeb.assignedRegister = -1;//unassigned initially

        for (const auto& r : ranges) {
            for (int l : r.lines) {
                newWeb.progPoints.insert(l);
            }
        }

        //saving data
        data.allWebs[newWeb.id] = newWeb;
        */
        vector<set<int>> mergedWebs;
        for (const auto& r : varRanges) {
            set<int> currentWeb(r.lines.begin(), r.lines.end());
            bool merged = false;

            //checking if the current interval has nums
            for (auto& thisWeb : mergedWebs) {
                bool overlaps = false;
                for (int points : currentWeb) {
                    if (thisWeb.count(points)) {
                        overlaps = true;
                        break;
                    }
                }

                if (overlaps) {
                    thisWeb.insert(currentWeb.begin(), currentWeb.end());
                    merged = true;
                    break;
                }
            }

            if (!merged) mergedWebs.push_back(currentWeb);
        }
        for (const auto& pointsSets : mergedWebs) {
            Web newWeb;
            newWeb.id = webCounter++;
            newWeb.varName = varName;
            newWeb.progPoints = pointsSets;
            newWeb.assignedRegister = -1;

            data.allWebs[newWeb.id] = newWeb;
        }
    }

    //at the end we have saved data of all variable names and their 'points'
    //so no multiple variables of the same name

}

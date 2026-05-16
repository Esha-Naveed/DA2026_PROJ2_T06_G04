//
// Created by esha-naveed on 5/14/26.
//
/*looks in the LiveRanges and adds data to data.allWebs*/

#include "webProcessing.h"
#include <algorithm>
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

    //where processing starts
    //currently we have groups of names in string part of the map
    for (auto& [varName, ranges] : groups) {
        //gathering all points for each variable (var during interation)

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
    }
    //at the end we have saved data of all variable names and their 'points'
    //so no multiple variables of the same name
}

//
// Created by esha-naveed on 5/14/26.
//
/**
* @file webProcessing.h
* @brief Class declarations for grouping independent live ranges into variable webs.
*/

#ifndef DA2026_PROJ2_T06_G04_WEB_PROCESSING_H
#define DA2026_PROJ2_T06_G04_WEB_PROCESSING_H
#include <vector>

#include "dataStruct.h"

/**
* @class webProcessing
* @brief Handles structural analysis pipelines that collapse raw execution intervals into distinct Web objects.
*/
class webProcessing {
public:
    /**
    * @brief Traverses sequential live ranges, maps overlaps belonging to identical variables, and compiles webs.
    * @details Clears pre-existing tracking records to avoid mutation artifacts, groups intervals per variable,
    * and performs sub-interval intersection merges to produce isolated variable webs.
    * @timecomplexity O(N * R * L) where N is unique variables, R is raw range instances per variable, * and L is the element lookups/insertions inside the standard library tracking sets.
    * @spacecomplexity O(M) auxiliary space to construct the structural sets and sorting buckets.
    * @param ranges Vector listing sequential raw live ranges extracted from execution logs.
    * @param data Reference to the global project repository where the generated webs are saved.
    */
    static void buildWebs(const std::vector<LiveRange>& ranges, ProjData& projData);
};


#endif //DA2026_PROJ2_T06_G04_WEB_PROCESSING_H

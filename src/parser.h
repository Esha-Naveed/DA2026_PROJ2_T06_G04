//
// Created by esha-naveed on 5/14/26.
//
/**
* @file parser.h
* @brief Function declarations for reading and processing input files.
*/

#ifndef DA2026_PROJ2_T06_G04_PARSER_H
#define DA2026_PROJ2_T06_G04_PARSER_H

#include "dataStruct.h"
/**
* @brief Parses the registers configuration file.
* @param data Reference to the global project data repository.
* @return true if parsing succeeds, false otherwise.
*/
bool parseRegsFile(ProjData &data);

/**
* @brief Parses the live ranges file and initiates Web building.
* @param data Reference to the global project data repository.
* @return true if parsing succeeds, false otherwise.
*/
bool parseRangesFile(ProjData &data);

/**
* @brief Groups raw sequential live ranges into consolidated Web entities based on overlap.
* @param rawRanges The raw list of live ranges parsed from the file.
* @param data Reference to the global project data repository where webs are saved.
*/
void mergeRangesIntoWebs(const std::vector<LiveRange>& rawRanges, ProjData &data);

#endif //DA2026_PROJ2_T06_G04_PARSER_H

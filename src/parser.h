//
// Created by esha-naveed on 5/14/26.
//

#ifndef DA2026_PROJ2_T06_G04_PARSER_H
#define DA2026_PROJ2_T06_G04_PARSER_H

#include "dataStruct.h"

bool parseRegsFile(ProjData &data);
bool parseRangesFile(ProjData &data);
void mergeRangesIntoWebs(const std::vector<LiveRange>& rawRanges, ProjData &data);

#endif //DA2026_PROJ2_T06_G04_PARSER_H

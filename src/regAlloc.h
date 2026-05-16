//
// Created by esha-naveed on 5/16/26.
//

#ifndef DA2026_PROJ2_T06_G04_REGALLOC_H
#define DA2026_PROJ2_T06_G04_REGALLOC_H

#include "graph.h"
#include "dataStruct.h"

class regAlloc {
public:
    static bool baseAllocation (Graph<Web>& g, int numReg);
    //bool so that we could get false if allocNum>numReg, and true if done successfully

    static void spilling(Graph<Web>& g, int numReg, int maxSpilling);

    static void splitting(Graph<Web>& g, int numReg, int maxSplitting);
};


#endif //DA2026_PROJ2_T06_G04_REGALLOC_H

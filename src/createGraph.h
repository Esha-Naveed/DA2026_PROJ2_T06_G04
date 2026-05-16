//
// Created by esha-naveed on 5/15/26.
//

#ifndef DA2026_PROJ2_T06_G04_CREATEGRAPH_H
#define DA2026_PROJ2_T06_G04_CREATEGRAPH_H

#include "graph.h"
#include "dataStruct.h"
using namespace std;

class createGraph {

public:
    static Graph<Web> buildGraph(ProjData& data);

    static bool WebsInterfere(const Web& w1, const Web& w2);

};

#endif //DA2026_PROJ2_T06_G04_CREATEGRAPH_H

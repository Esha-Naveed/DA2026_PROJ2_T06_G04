//
// Created by esha-naveed on 5/15/26.
//
/**
* @file createGraph.h
* @brief Declarations for building the web interference graph.
* @details Provides utility functions to check intersections between web
* program points and construct an undirected graph representing interferences.
*/

#ifndef DA2026_PROJ2_T06_G04_CREATEGRAPH_H
#define DA2026_PROJ2_T06_G04_CREATEGRAPH_H

#include "graph.h"
#include "dataStruct.h"

/**
* @class createGraph
* @brief Factory class responsible for generating the interference graph from parsed Web data.
*/

using namespace std;

class createGraph {

public:
    /**
    * @brief Constructs an interference graph where vertices represent webs and edges represent life overlaps.
    * * @timecomplexity O(V^2 * M * log(N)) where V is the number of webs, M is the size of the smaller
    * web's program points, and N is the size of the larger web's program points.
    * @spacecomplexity O(V + E) to store the generated vertices and mutual interference edges.
    * * @param data Reference to the global project data containing all parsed webs.
    * @return Graph<Web> The completely built undirected interference graph.
    */
    static Graph<Web> buildGraph(ProjData& data);

    /**
    * @brief Determines whether two individual webs interfere (share at least one program point line).
    * * @timecomplexity O(M * log(N)) where M is the size of the smaller program point set * and N is the size of the larger set.
    * @spacecomplexity O(1) Auxiliary space.
    * * @param w1 First web to compare.
    * @param w2 Second web to compare.
    * @return true if they share an execution point line, false otherwise.
    */
    static bool WebsInterfere(const Web& w1, const Web& w2);

};

#endif //DA2026_PROJ2_T06_G04_CREATEGRAPH_H

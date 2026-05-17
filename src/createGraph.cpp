//
// Created by esha-naveed on 5/15/26.
//
/**
* @file createGraph.cpp
* @brief Implementation of interference graph construction algorithms.
*/

#include "createGraph.h"
#include "graph.h"
#include <algorithm>

using namespace std;


bool createGraph::WebsInterfere(const Web &w1, const Web &w2) {
    //optimization, making sure to iterate over the smaller program points
    if (w1.progPoints.size() > w2.progPoints.size()) { return WebsInterfere(w2, w1); }

    //search anything common
    for (int point : w1.progPoints) {
        if (w2.progPoints.count(point) > 0) {
            return true;
        }
    }
    return false;
}

Graph<Web> createGraph::buildGraph(ProjData &data) {
    Graph<Web> g;

    //add all webs as Vertex into the graph
    for (auto const& [id, web] : data.allWebs) g.addVertex(web);

    /*checking if the web overlaps,
     *if yes-> create an edge,
     *if not-> continue
     */
    auto vertices = g.getVertexSet();//getting vertices
    for (size_t i = 0; i < vertices.size(); i++) {
        for (size_t j = i+1; j < vertices.size(); j++) {
            //using pointer so we are access the real thing and not copying it
            const Web &w1 = vertices[i]->getInfo();
            const Web &w2 = vertices[j]->getInfo();

            if (WebsInterfere(w1, w2)) {
                //add edge for both webs
                g.addFlowEdge(w1, w2, 1);
                g.addFlowEdge(w2, w1, 1);
            }
        }
    }
    return g;
}


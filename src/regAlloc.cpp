//
// Created by esha-naveed on 5/16/26.
//
/*allocates registers or 'Colors' them
 * contains function of spilling and splitting
 */
/**
 * @file regAlloc.cpp
 * @brief Implementation of basic greedy register allocation, spilling, web splitting, and custom stack-based coloring algorithm.
 */

#include "regAlloc.h"
#include <algorithm>
#include "createGraph.h"
#include <stack>
using namespace std;

Vertex<Web>* regAlloc::findMaxDegree(Graph<Web>& g) {
    //get vertices
    auto vertices = g.getVertexSet();
    Vertex<Web>* maxNode = nullptr;
    size_t maxEdges = 0;
    bool valid = true;

    for (auto vertex : vertices) {
        //skipping already done webs
        if (vertex->getInfo().assignedRegister == -2) continue;

        ////counting only the non-spilled adj
        size_t activeEdges = 0;
        for (auto edge : vertex->getAdj()) {
            if (edge->getDest()->getInfo().assignedRegister != -2) activeEdges++;
        }

        if (!maxNode || activeEdges > maxEdges) {
            maxEdges = activeEdges;
            maxNode = vertex;
        }
    }
    return maxNode;
}

bool regAlloc::baseAllocation(Graph<Web> &g, int numReg) {
    //getting vertices that we will work with
    auto vertices = g.getVertexSet();

    stack<Vertex<Web>*> colorStack;
    set<Vertex<Web>*> removedV;

    //resetting allocations
    for (auto v : vertices) {
        if (v->getInfo().assignedRegister != -2) removedV.insert(v);//v->getInfo().assignedRegister = -1;
    }
/*
    //Greedy interation over each vertex
    for (auto vertex : vertices) {
        //skipping spilled nodes
        if (vertex->getInfo().assignedRegister == -2) continue;

        set<int> usedReg;//an empty list fr the current vertex

        //every edge connected to the vertex
        for (auto edge : vertex->getAdj()) {
            auto connectedEdge = edge->getDest();
            //using getDest to find the connected edge
            int connectedEdgeReg = connectedEdge->getInfo().assignedRegister;
            //finding out what register is the connected edge assigned to, its 'color'

            //if the connectedEdge is already assigned an int, we will use a different one than that
            //cuz they overlap, so different registers
            if (connectedEdgeReg > 0) { usedReg.insert(connectedEdgeReg); }
        }
        //till here we find the registers we cannot assign to a specific vertex
        //can find the best suited reg, by assigning it the first number starting
        //from 1 that is not in the usedReg

        int assign = -1;//-1 is unassigned
        for (int r=1; r<=numReg; r++) {
            //if (usedReg.find(r) != usedReg.end()) {
            ////if r is not in usedReg, it can be used
            if (usedReg.count(r)==0){
                assign = r;
                break;//as soon as the first usable reg is found break the loop and move to the next
            }
        }

        //if no regs free, then the basic doesn't apply and funtion fails
        if (assign==-1) {return false;}

        vertex->getInfo().assignedRegister = assign;
    }
    //else the function works trough all of it and return true
    return true;*/
    bool working = true;
    while (working) {
        working = false;
        for (auto v : vertices) {
            if (removedV.count(v)) continue;

            int activeEdges = 0;
            for (auto edge : v->getAdj()) {
                if (removedV.count(edge->getDest()) == 0) activeEdges++;
            }

            if (activeEdges < numReg) {
                colorStack.push(v);
                removedV.insert(v);
                working = true;
                break;
            }
        }

        // if blocked, node pushed (Optimistic Spilling Heuristic)
        if (!working && removedV.size() < vertices.size()) {
            Vertex<Web>* spillVertex = nullptr;
            int maxDgree = -1;

            for (auto v: vertices) {
                if (removedV.count(v)) continue;
                int activeEdgeReg = 0;
                for (auto edge: v->getAdj()) {
                    if (removedV.count(edge->getDest()) == 0) activeEdgeReg++;
                }
                if (activeEdgeReg > maxDgree) {
                    maxDgree = activeEdgeReg;
                    spillVertex = v;
                }
            }
            if (spillVertex) {
                colorStack.push(spillVertex);
                removedV.insert(spillVertex);
                working = true;
            }
        }
    }

    bool success = true;
    while (!colorStack.empty()) {
        auto v = colorStack.top();
        colorStack.pop();

        set<int> usedReg;
        for (auto edge : v->getAdj()) {
            int adjReg = edge->getDest()->getInfo().assignedRegister;
            if (adjReg >= 0) usedReg.insert(adjReg);
        }

        int selectedReg = -1;
        for (int i=0; i<numReg; i++) {
            if (usedReg.count(i)==0) {
                selectedReg = i;
                break;
            }
        }

        v->getInfo().assignedRegister = selectedReg;
        if (selectedReg == -1) {
            success = false;
        }
    }
    return success;
}

void regAlloc::spilling(Graph<Web> &g, int numReg, int maxSpilling) {
    int countSpill = 0;

    //until basic allocation doesn't work AND that is less than that
    while (!baseAllocation(g, numReg) && countSpill < maxSpilling) {
        Vertex<Web>* v = findMaxDegree(g);
        if (!v) break; //if there is no v with max -- will only be possible if edges for all are 0

        //else
        //keep the max degree node to memory
        v->getInfo().assignedRegister = -2;//keeping it -2 since any +ve number can be represented by the registers
        countSpill++;

        cout << "Spilled for the web: " << v->getInfo().id << " | name: " << v->getInfo().varName << endl;
    }
    baseAllocation(g, numReg);
}

//using recursion for this
void regAlloc::splitting(Graph<Web> &g, int numReg, int maxSplitting, int &countSplit) {
    //checking if graph already perfectly colored
    if (baseAllocation(g, numReg) || countSplit >= maxSplitting) return; //works just fine, does not need to be split

    //loops until countSplit < maxSplit
    while (countSplit < maxSplitting) {
        //finding vertex with most edges
        Vertex<Web>* v = findMaxDegree(g);
        if (!v) return;

        /*splitting logic*/
        Web &prevWebCopy = v->getInfo();

        //make sure the web has more than points
        if (prevWebCopy.progPoints.size() <= 1) {
            prevWebCopy.assignedRegister = -2; continue;
        }

        //divide the points by 2
        set<int> partA, partB;
        int mid = prevWebCopy.progPoints.size()/2;
        int i = 0;
        for (int p : prevWebCopy.progPoints) {
            if (i < mid) partA.insert(p);
            else partB.insert(p);
            i++;
        }

        //we need a new web of partB
        Web splitWeb;
        splitWeb.id = g.getNumVertex()+1;
        splitWeb.varName = prevWebCopy.varName+"2";
        splitWeb.progPoints = partB;
        splitWeb.assignedRegister = -1;

        //replace the part/vertex with partA
        v->getInfo().progPoints = partA;

        //adding the new web/vertex to the graph
        g.addVertex(splitWeb);
        Vertex<Web>* ogVertex = g.findVertex(prevWebCopy);
        Vertex<Web>* newVertex = g.findVertex(splitWeb);

        //re-doing interference edges with the new web system
        /*using websInterfere from createGraph.cpp*/
        auto allVertices = g.getVertexSet();
        for (auto vertex : allVertices) {
            if (vertex == ogVertex || vertex == newVertex) continue;
            //since newVertex already has the splitWeb and it cannot have the original Vetrex beore split
            //for partA
            if (createGraph::WebsInterfere(ogVertex->getInfo(), vertex->getInfo())) {
                g.addFlowEdge(ogVertex->getInfo(), vertex->getInfo(), 1);
                g.addFlowEdge(vertex->getInfo(), ogVertex->getInfo(), 1);
            }
            //for partB
            if (createGraph::WebsInterfere(newVertex->getInfo(), vertex->getInfo())) {
                g.addFlowEdge(newVertex->getInfo(), vertex->getInfo(), 1);
                g.addFlowEdge(vertex->getInfo(), newVertex->getInfo(), 1);
            }
        }

        //web interference between the two new parts
        if (createGraph::WebsInterfere(newVertex->getInfo(), ogVertex->getInfo())) {
            g.addFlowEdge(newVertex->getInfo(), ogVertex->getInfo(), 1);
            g.addFlowEdge(ogVertex->getInfo(), newVertex->getInfo(), 1);
        }
        countSplit++;

        //recursive call
        splitting(g, numReg, maxSplitting, countSplit);

        if (baseAllocation(g, numReg)) return;
        /*after each splitting it will check if baseAllocation is true or not
         * as soon as it is, it gets out of the loop
         * in recursion once the solution is found and the program backtracks to the
         * first loop, to backtrack the 'return' is important to finish off the working loop
         */
    }
}

//using stacks
bool regAlloc::customAllocation(Graph<Web>& g, int numReg) {
    auto vertices = g.getVertexSet();
    //reset all allocation
    for (auto v: vertices) v->getInfo().assignedRegister = -1;

    stack<Vertex<Web>*> colorStack;
    set<Vertex<Web>*> removedV;

    bool working = true;
    while (removedV.size() < vertices.size() && working) {
        working = false;
        for (auto v: vertices) {
            if (removedV.count(v)) continue;

            //counting active adj v
            int activeEdgeReg = 0;
            for (auto edge: v->getAdj()) {
                if (removedV.count(edge->getDest()) == 0) activeEdgeReg++;
            }

            if (activeEdgeReg < numReg) {
                colorStack.push(v);
                removedV.insert(v);
                working = true;
            }
        }

        //if blocked, node pushed
        if (!working && removedV.size() < vertices.size()) {
            Vertex<Web>* spillVertex = nullptr;
            int maxDgree = -1;

            for (auto v: vertices) {
                if (removedV.count(v)) continue;
                int activeEdgeReg = 0;
                for (auto edge: v->getAdj()) {
                    if (removedV.count(edge->getDest()) == 0) activeEdgeReg++;
                }
                if (activeEdgeReg > maxDgree) {
                    maxDgree = activeEdgeReg;
                    spillVertex = v;
                }
            }
            if (spillVertex) {
                colorStack.push(spillVertex);
                removedV.insert(spillVertex);
                working = true;
            }
        }
    }
    bool success = true;
    while (!colorStack.empty()) {
        auto v = colorStack.top();
        colorStack.pop();

        set<int> usedReg;
        for (auto edge : v->getAdj()) {
            int adjReg = edge->getDest()->getInfo().assignedRegister;
            if (adjReg >= 0) usedReg.insert(adjReg);
        }

        int selectedReg = -1;
        for (int i=0; i<numReg; i++) {
            if (usedReg.count(i)==0) {
                selectedReg = i;
                break;
            }
        }

        if (selectedReg == -1) v->getInfo().assignedRegister = selectedReg;
        else {
            v->getInfo().assignedRegister = -2;
            success = false;
        }
    }

    return success;
}

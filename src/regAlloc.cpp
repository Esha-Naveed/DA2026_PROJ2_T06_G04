//
// Created by esha-naveed on 5/16/26.
//
/*allocates registers or 'Colors' them
 * contains function of spilling and splitting
 */
#include "regAlloc.h"
#include <algorithm>
using namespace std;

bool regAlloc::baseAllocation(Graph<Web> &g, int numReg) {
    //getting vertices that we will work with
    auto vertices = g.getVertexSet();

    //reset all allocations, in case they were not done before or whatever; better safe than sorry
    for (auto v: vertices) {
        Web &w = v->getInfo();
        w.assignedRegister = -1;
    }

    //Greedy interation over each vertex
    for (auto vertex : vertices) {
        set<int> usedReg;//an empty list fr the current vertex

        //every edge connected to the vertex
        for (auto edge : vertex->getAdj()) {
            auto connectedEdge = edge->getDest();
            //using getDest to find the connected edge
            int connectedEdgeReg = connectedEdge->getInfo().assignedRegister;
            //finding out what register is the connected edge assigned to, its 'color'

            //if the connectedEdge is already assigned an int, we will use a different one than that
            //cuz they overlap, so different registers
            if (connectedEdgeReg != -1) {
                usedReg.insert(connectedEdgeReg);
            }
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
    }
    //else the function works trough all of it and return true
    return true;
}

void regAlloc::spilling(Graph<Web> &g, int numReg, int maxSpilling) {

}

void regAlloc::splitting(Graph<Web> &g, int numReg, int maxSplitting) {

}

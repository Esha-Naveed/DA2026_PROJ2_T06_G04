//
// Created by esha-naveed on 5/16/26.
//
/**
 * @file regAlloc.h
 * @brief Class declarations for register allocation, spilling, and web splitting algorithms.
 * @details Provides utility structures to manage greedy graph coloring heuristics
 * and handle allocation failures via spilling or recursive live-range splitting.
 */

#ifndef DA2026_PROJ2_T06_G04_REGALLOC_H
#define DA2026_PROJ2_T06_G04_REGALLOC_H

#include "graph.h"
#include "dataStruct.h"

/**
 * @class regAlloc
 * @brief Class responsible for executing graph coloring algorithms and resource-overflow strategies.
 */
class regAlloc {
public:
    /**
     * @brief Performs a standard greedy register allocation over the graph.
     * @timecomplexity O(V * E) where V is the number of vertices (webs) and E is the average number of edges checked per node.
     * @spacecomplexity O(R) where R is the number of registers tracked in the local used-register set.
     * @param g Reference to the active web interference graph.
     * @param numReg Total number of available physical hardware registers.
     * @return true if all non-spilled webs are colored successfully, false if resource capacity is exceeded.
     */
    static bool baseAllocation (Graph<Web>& g, int numReg);
    //bool so that we could get false if allocNum>numReg, and true if done successfully

    /**
     * @brief Progressively spills high-degree nodes to memory until the graph can be successfully allocated.
     * @timecomplexity O(S * V * E) where S is the number of spill iterations needed, V is vertex count, and E is edge count.
     * @spacecomplexity O(1) auxiliary memory footprint.
     * @param g Reference to the active web interference graph.
     * @param numReg Total number of available physical hardware registers.
     * @param maxSpilling Hard threshold limiting the maximum number of webs allowed to be spilled.
     */
    static void spilling(Graph<Web>& g, int numReg, int maxSpilling);

    /**
     * @brief Recursively bisects high-interference live ranges into smaller sub-webs to facilitate allocation.
     * @timecomplexity O(2^S * V^2) due to recursive bisection logic and global interference recalculation sweeps.
     * @spacecomplexity O(S) where S is the maximum recursion depth on the execution call stack.
     * @param g Reference to the active web interference graph.
     * @param numReg Total number of available physical hardware registers.
     * @param maxSplitting Hard threshold limiting the maximum number of structural splits allowed.
     * @param countSplit Reference tracking counter monitoring currently processed split iterations.
     */
    static void splitting(Graph<Web>& g, int numReg, int maxSplitting, int &countSplit);

    /**
     * @brief Identifies the unallocated vertex holding the maximum active constraints (highest active edge degree).
     * @timecomplexity O(V * E) where V is the total vertex pool and E is the adjacency evaluation depth.
     * @spacecomplexity O(1) auxiliary variable storage.
     * @param g Reference to the active web interference graph.
     * @return Vertex<Web>* Pointer to the maximum degree node, or nullptr if all remaining active nodes are isolated.
     */
    static Vertex<Web>* findMaxDegree(Graph<Web>& g);
};


#endif //DA2026_PROJ2_T06_G04_REGALLOC_H

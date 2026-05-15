/**
* @file createGraph.h
 * @brief Header file for the graph construction logic.
 * @details This class acts as the bridge between the parsed CSV data and the
 * flow network used for the assignment algorithm.
 */

#ifndef DA_PROJECT1_CREATEGRAPH_H
#define DA_PROJECT1_CREATEGRAPH_H

#include "graph.h"
#include "dataStructures.h"

using namespace std;

/**
 * @class createGraph
 * @brief Provides static methods to transform project data into a flow network.
 */
class createGraph {
public:
    /**
     * @brief Transforms parsed ProjectData into a directed Flow Graph.
     * * This function creates a source node, a sink node, and nodes for all
     * reviewers and submissions. It adds edges with capacities based on
     * expertise matching.
     * * @param data The projectData structure containing all reviewers and papers.
     * @return A Graph object configured for the Max-Flow algorithm.
     * * @time_complexity O(R * S) where R is the number of reviewers and S is
     * the number of submissions, as we must check matches for every pair.
     * @space_complexity O(V + E) where V = R + S + 2 and E is the number of valid matches.
     */
    static Graph<int> buildGraph(projectData& data);

    /**
     * @brief Exports assignment results and risk analysis to a CSV file.
     * @details Traverses the graph to identify edges with positive flow. It generates
     * lists of assignments, calculates total matches, and identifies "Risk" submissions
     * that did not meet the minimum review requirement.
     * @param data The projectData structure for mapping node IDs to real IDs.
     * @param g The Graph after the Edmonds-Karp algorithm has been executed.
     * @time_complexity O(V + E) to traverse the graph and check edge flows.
     */
    static void output_file(projectData &data, Graph<int> &g);

    /**
     * @brief Identifies reviewers who are at "risk" or under-assigned.
     * @details Analyzes the flow network to find reviewers whose current assignments
     * are below a certain threshold or who are critical bottlenecks in the matching.
     * @param data The projectData structure containing reviewer constraints.
     * @param g The Graph containing the final flow results.
     * @return A vector of integer IDs representing the reviewers at risk.
     * @time_complexity O(V) where V is the number of reviewer nodes in the graph.
     */
    static vector<int> risk_revs(projectData &data, Graph<int> g);
};

#endif //DA_PROJECT1_CREATEGRAPH_H

//
// Created by esha-naveed on 3/30/26.
//

#include "graph.h"
#include "dataStructures.h"
#include "createGraph.h"
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

#include "algorithm.h"
using namespace std;
/*
 *Risk Analysis:
 *checking if there is actually a need of haivg a reviewer
 *this check will only run if in control the risk ANalysis is 1
 *otherwise NOPE!
 *basically running the edmond-krap without a reviewer, if the flow is less than the actual flow
 *the reviewer seems to be important and thius cannot get fired
 *otherwise it's bye!bye!bye!
 *welp technically (literally here!) it is added to the vector
 *basicaly saving the data to print in out to the output file
 */
Graph<int> createGraph::buildGraph(projectData &data) {
    //basic info
    Graph<int> g;
    int numSub = data.submissions.size();
    int numRev = data.reviewers.size();

    //S(super source), T(super sink),
    int S = 0;
    int T = numSub + numRev + 1;

    //adding all vertices to g
    for (int i = 0; i <= T; i++) { g.addVertex(i); }

    //grapgh: S->subs->revs->T
    /*
    *vertex 0: super source s
    vertex 1 to n: submissions
    vertex n+1 to n+m: reviewers
    vertex n+m+1: super-sink t
    */

    //S->subs
    //capacity==MinReviewsPerSubmission
    for (int i = 0; i < numSub; i++) {
        int nodeID = i + 1;
        data.nodeToRealID[nodeID] = data.submissions[i].id;
        g.addFlowEdge(S, nodeID, data.config.minReviewsPerSubmission);
    }

    //revs->T
    //capacity==MaxReviewsPerReviewer
    for (int j = 0; j < numRev; j++) {
        int nodeID = numSub + j + 1;
        data.nodeToRealID[nodeID] = data.reviewers[j].id;
        g.addFlowEdge(nodeID, T, data.config.maxReviewsPerReviewer);
    }

    //based on mode, connect subs->revs
    for (int i = 0; i < numSub; i++) {
        for (int j = 0; j < numRev; j++) {
            const auto& sub = data.submissions[i];
            const auto& rev = data.reviewers[j];
            bool match = false;

            //based on GenerateAssignments
            if (data.config.generateAssignments == 1) {
                //1: matches primary subs to primary revs
                if (sub.primary == rev.primary) match = true;
            }
            else if (data.config.generateAssignments == 2) {
                //2: only primary revs can be linked to subs, first to primary subs then secondary
                if ((sub.primary == rev.primary || sub.secondary == rev.primary)) match = true;
            }
            else if (data.config.generateAssignments == 3) {
                //3: either rev to either sub with parameters from the 4 other things

                // Check if match is possible based on toggled expertise/domain
                bool pMatch = (data.config.primarySubmissionDomain == 1 && data.config.primaryReviewerExpertise == 1 && sub.primary == rev.primary);
                bool sMatch = (data.config.secondarySubmissionDomain == 1 && data.config.primaryReviewerExpertise == 1 && sub.secondary == rev.primary);
                bool prMatch = (data.config.primarySubmissionDomain == 1 && data.config.secondaryReviewerExpertise == 1 && sub.primary == rev.secondary);
                bool srMatch = (data.config.secondarySubmissionDomain == 1 && data.config.secondaryReviewerExpertise == 1 && sub.secondary == rev.secondary);

                if (pMatch || sMatch || prMatch || srMatch) match = true;
            }
                /*
                //checking for the 4 parameters:
                bool pSub = (data.config.primarySubmissionDomain == 1);
                bool sSub = (data.config.secondarySubmissionDomain == 1);
                bool pRev = (data.config.primaryReviewerExpertise== 1);
                bool sRev = (data.config.secondaryReviewerExpertise == 1);

                //checking the conditions + the id thingy for each sub and rev
                if (pSub && pRev && sub.primary == rev.primary) match = true;
                if (sSub && pRev && sub.secondary == rev.primary) match = true;
                if (pSub && sRev && sub.primary == rev.secondary) match = true;
                if (sSub && sRev && sub.secondary == rev.secondary) match = true;
                */

            if (match) {
                //capacity:paper to review - 1, one paper goes to one reviewer, one rev can take many subs
                g.addFlowEdge(i + 1, numSub + j + 1, 1);
            }
            else match=true;
        }
    }
    return g;
}

vector<int> createGraph::risk_revs(projectData &data, Graph<int> g) {
    int S =0;//source
    int T = data.submissions.size() + data.reviewers.size() + 1;//sink

    g.resetFlow();
    int OG_max_flow = Edmonds_karp(&g, S, T);
    vector<int> risk_rev_id;

    //go through each rev - revs start after subs
    int rev_start = data.submissions.size()+1;//revs start after subs
    int rev_end = data.submissions.size() + data.reviewers.size();

    for (int i = rev_start; i <= rev_end; i++) {
        auto v = g.findVertex(i);
        if (!v) continue;

        //edges connecting teh rev to the T(sink)
        for (auto e : v->getAdj()) {
            //if the destination is the sink
            if (e->getDest()->getInfo() == T) {
                int rev_cap = e->getCapacity();//saving the cap
                e->setCapacity(0);//removing the rev
                g.resetFlow();//a brand new staaarrrtt~~

                int newFlow=Edmonds_karp(&g, S, T);

                //if the new flow is less than the OG max flow then the rev is sus
                if (newFlow < OG_max_flow) risk_rev_id.push_back(data.nodeToRealID[i]);

                e->setCapacity(rev_cap);//restoring the capacity to its full glory
                break;
            }
        }
            }
    //sorting too! really?
    sort(risk_rev_id.begin(), risk_rev_id.end());
    return risk_rev_id;
}

void createGraph::output_file(projectData &data, Graph<int> &g) {

    //name of file
    string filename = data.config.outputFileName;
    if (filename.empty()) filename = "no_name.csv";

    //creating file
    ofstream outFile(filename);
    if (!outFile.is_open()) { cout << "Error opening output file" << endl; exit(1); }

    //counter
    int count=0;

    int numSub = data.submissions.size();
    int numRev = data.reviewers.size();

    //subs list
    outFile << "#Submissions,Reviewer,Match" << endl;
    for (int i=1; i<=numSub; i++) {
        auto v = g.findVertex(i);//get vertex

        if (!v) continue;//if not, dude move on

        for (auto e : v->getAdj()) {//for each edge in the vertex
            /**conditions in if
             * if there is flow (>0)
             * edge is -> a arev, moving to sub com higer id, since subs 1st then revs
             * stops at revs - to not count sink
             */
            if ((e->getFlow() > 0) && (e->getDest()->getInfo() > numSub) && (e->getDest()->getInfo() <= (numSub + numRev))) {
                //outFile << data.nodeToRealID[i] << "," << data.nodeToRealID[e->getDest()->getInfo()] << "," << data.submissions[i-1].primary << endl;
                int reviewerNodeID = e->getDest()->getInfo();
                outFile << data.nodeToRealID[i] << "," << data.nodeToRealID[reviewerNodeID] << "," << data.submissions[i-1].primary << endl;
                count++;
            }
        }
    }

    outFile << "#Reviewer,Submission,Match" << endl;
    for (int j=0; j<numRev; j++) {
        int revNodeID = numSub + j + 1;
        for (int i = 1; i <= numSub; i++) {
            auto subV = g.findVertex(i);
            if (!subV) continue;
            for (auto e : subV->getAdj()) {
                if (e->getDest()->getInfo() == revNodeID && e->getFlow() > 0) {
                    outFile << data.nodeToRealID[revNodeID] << "," << data.nodeToRealID[i] << "," << data.reviewers[j].primary << endl;
                }
            }
        }
        //S->sub->rev->t
/*
        int revNodeIdx = numSub + j + 1;//starting from subs+1

        auto v = g.findVertex(revNodeIdx);//vertex of THIS rev
        if (!v) continue;

        for (auto vertex : g.getVertexSet()) {//in each vertex
            /*condition
             * for verteices between 1 and sub
             *
            if ((vertex->getInfo() >= 1) && (vertex->getInfo() <= numSub)) {
                //edges FROM a specific sub
                for (auto e : vertex->getAdj()) {
                    //if edge points at the cureent rev
                    if ((e->getDest() == v) && (e->getFlow() > 0)) {
                        outFile << data.nodeToRealID[revNodeIdx] << "," << data.nodeToRealID[vertex->getInfo()] << "," << data.reviewers[j].primary << endl;
                    }
                }
            }
        }*/
    }

    //totaL print
    outFile << "#Total: " << count << endl;

    //last list
    if (data.config.riskAnalysis == 1) {
        vector<int> risks = risk_revs(data, g);
        outFile << "#Risk Analysis: 1" << endl;
        for (size_t i = 0; i < risks.size(); i++) {
            outFile << risks[i] << (i == risks.size() - 1 ? "" : ", ");
        }
        outFile << endl;
    }else {
        outFile << "#Submissions,Domain,Missing,Reviews" << endl;
        for (int i = 1; i <= numSub; i++) {
            auto v = g.findVertex(i);//for each sub mode
            int curr_revs = 0;//counter for each sub

            if (v) {
                for (auto e : v->getAdj()) {
                    if (e->getFlow() > 0 && e->getDest()->getInfo() > numSub) curr_revs++;
                }
            }

            //minus from needed to the one you got!
            int missing = data.config.minReviewsPerSubmission - curr_revs;

            //IF IF IF sub has less revs throw in with the RISKY files!!!
            if (missing > 0) {
                outFile << data.nodeToRealID[i] << "," << data.submissions[i-1].primary << "," << missing << endl;
            }
        }
    }

    outFile.close();
}

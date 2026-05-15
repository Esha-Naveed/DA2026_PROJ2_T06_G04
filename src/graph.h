/**
* @file graph.h
 * @brief Graph infrastructure for flow networks.
 * @details Implements Vertices and Edges with support for residual graphs,
 * essential for Max-Flow algorithms like Edmonds-Karp.
 */

#ifndef DA_PROJECT1_GRAPH_H
#define DA_PROJECT1_GRAPH_H

#include <iostream>
#include <vector>

template <class T>
class Edge;

#define INF std::numeric_limits<double>::max()

/************************* Vertex  **************************/
template <class T>
class Vertex {
public:
    Vertex(T in);
    T getInfo() const;
    std::vector<Edge<T> *> getAdj() const;

    //for bfs
    bool isVisited() const;
    void setVisited(bool visited);
    Edge<T> *getPath() const;
    void setPath(Edge<T> *path);

    Edge<T> * addEdge(Vertex<T> *dest, double w);

protected:
    T info;
    std::vector<Edge<T> *> adj;
    bool visited = false;
    Edge<T> *path = nullptr; //use to backtrack aug path
};

/********************** Edge  ****************************/
template <class T>
class Edge {
public:
    Edge(Vertex<T> *orig, Vertex<T> *dest, double c);

    Vertex<T> * getDest() const;
    Vertex<T> * getOrig() const;
    double getCapacity() const;
    double getFlow() const;
    Edge<T> *getReverse() const;

    void setFlow(double flow);
    void setReverse(Edge<T> *reverse);
    void setCapacity(int capacity);

protected:
    Vertex<T> * orig;
    Vertex<T> * dest;
    double capacity;
    double flow = 0; // Initialize at 0!
    Edge<T> *reverse = nullptr;
};

/********************** Graph  ****************************/

template <class T>
class Graph {
public:
    /**
     * @brief Finds a vertex in the graph by its information content.
     * @param in The information to search for.
     * @return Pointer to the Vertex if found, nullptr otherwise.
     * @time_complexity O(V) where V is the number of vertices.
     */
    Vertex<T> *findVertex(const T &in) const;

    /**
     * @brief Adds a new vertex to the graph.
     * @param in The information content for the new vertex.
     * @return true if added, false if a vertex with that info already exists.
     * @time_complexity O(V) because it calls findVertex first.
     */
    bool addVertex(const T &in);

    /**
     * @brief Adds a directed flow edge between two vertices.
     * @details Creates both a forward edge with the given capacity and a
     * reverse (residual) edge with 0 capacity. Links them via setReverse().
     * * @param sourc The info of the source vertex.
     * @param dest The info of the destination vertex.
     * @param cap The maximum capacity of the flow edge.
     * @return true if both vertices exist and the edges were created, false otherwise.
     * * @time_complexity O(V) due to findVertex calls (scanning the vertex set).
     */
    bool addFlowEdge(const T &sourc, const T &dest, double cap);
    void resetFlow();
    int getNumVertex() const;
    std::vector<Vertex<T> *> getVertexSet() const;

protected:
    std::vector<Vertex<T> *> vertexSet;
    int findVertexIdx(const T &in) const;
};

/************************* Vertex  **************************/
template <class T>
Vertex<T>::Vertex(T in): info(in) {}

template <class T>
Edge<T> * Vertex<T>::addEdge(Vertex<T> *d, double w) {
    auto newEdge = new Edge<T>(this, d, w);
    adj.push_back(newEdge);
    return newEdge;
}

template <class T>
T Vertex<T>::getInfo() const { return this->info; }

template <class T>
std::vector<Edge<T>*> Vertex<T>::getAdj() const { return this->adj; }

template <class T>
bool Vertex<T>::isVisited() const { return this->visited; }

template <class T>
void Vertex<T>::setVisited(bool visited) { this->visited = visited; }

template <class T>
Edge<T> *Vertex<T>::getPath() const { return this->path; }

template <class T>
void Vertex<T>::setPath(Edge<T> *path) { this->path = path; }

/********************** Edge  ****************************/
template <class T>
Edge<T>::Edge(Vertex<T> *orig, Vertex<T> *dest, double c):
    orig(orig), dest(dest), capacity(c), flow(0) {}

template <class T>
double Edge<T>::getCapacity() const { return this->capacity; }

template <class T>
Vertex<T> * Edge<T>::getDest() const { return this->dest; }

template <class T>
Vertex<T> * Edge<T>::getOrig() const { return this->orig; }

template <class T>
double Edge<T>::getFlow() const { return flow; }

template <class T>
Edge<T> *Edge<T>::getReverse() const { return this->reverse; }

template <class T>
void Edge<T>::setReverse(Edge<T> *reverse) { this->reverse = reverse; }

template <class T>
void Edge<T>::setFlow(double flow) { this->flow = flow; }

template <class T>
void Edge<T>::setCapacity(int capacity) { this->capacity = capacity; }

/********************** Graph  ****************************/

template <class T>
int Graph<T>::getNumVertex() const {
    return vertexSet.size();
}

template <class T>
std::vector<Vertex<T> *> Graph<T>::getVertexSet() const {
    return vertexSet;
}

template <class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
    for (auto v : vertexSet)
        if (v->getInfo() == in)
            return v;
    return nullptr;
}

template <class T>
int Graph<T>::findVertexIdx(const T &in) const {
    for (unsigned i = 0; i < vertexSet.size(); i++)
        if (vertexSet[i]->getInfo() == in)
            return i;
    return -1;
}

template <class T>
bool Graph<T>::addVertex(const T &in) {
    if (findVertex(in) != nullptr)
        return false;
    vertexSet.push_back(new Vertex<T>(in));
    return true;
}

template <class T>
bool Graph<T>::addFlowEdge(const T &sourc, const T &dest, double cap) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr) return false;
    auto e1 = v1->addEdge(v2, cap);
    auto e2 = v2->addEdge(v1, 0);
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}

template <class T>
void Graph<T>::resetFlow() {
    for (auto v : vertexSet) {
        for (auto e : v->getAdj()) {
            e->setFlow(0);
        }
    }
}

#endif //DA_PROJECT1_GRAPH_H

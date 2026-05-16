//
// Created by esha-naveed on 5/15/26.
//

#ifndef DA2026_PROJ2_T06_G04_GRAPH_H
#define DA2026_PROJ2_T06_G04_GRAPH_H

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
    T &getInfo();
    void setInfo(const T &in);
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

    Vertex<T> *findVertex(const T &in) const;

    bool addVertex(const T &in);

    bool addFlowEdge(const T &source, const T &dest, double cap);
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
T& Vertex<T>::getInfo() { return this->info; }

template <class T>
void Vertex<T>::setInfo(const T &info) { this->info = info; }

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
int Graph<T>::getNumVertex() const { return vertexSet.size(); }

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
bool Graph<T>::addFlowEdge(const T &source, const T &dest, double cap) {
    auto v1 = findVertex(source);
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

#endif //DA2026_PROJ2_T06_G04_GRAPH_H

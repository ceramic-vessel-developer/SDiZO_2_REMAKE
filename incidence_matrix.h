//
// Created by kacpe on 29.05.2023.
//

#ifndef SDIZO_2_INCIDENCE_MATRIX_H
#define SDIZO_2_INCIDENCE_MATRIX_H

#include <string>
#include <cstdio>
#include <vector>
#include "kruskal_edge.h"

class incidence_matrix {
private:
    int** matrix = nullptr;
    int* weights = nullptr;
    int numVertices;
    int numEdges;
    bool directed = false;

public:
    struct Subset {
        int parent;
        int rank;
    };
    incidence_matrix();
    incidence_matrix(int vertices, int density, bool directed);
    incidence_matrix(std::string file, bool directed);
    ~incidence_matrix();

    void add_directed_edge(int start,int end, int weight);
    void add_undirected_edge(int start,int end, int weight);
    int are_connected(int start,int end);
    void displayMatrix();

    void prim(int** parent_p = nullptr, int** key_p = nullptr, bool print = false);
    void kruskal(kruskal_edge*** mst_p = nullptr, bool print = false);
    void dijkstra(int source, int end,int** parent_p = nullptr, int** distance_p = nullptr, bool print = false);
    void bellman_ford(int source, int end,int** parent_p = nullptr, int** distance_p = nullptr, bool print = false);

    void printDijkstra(int source, int end);
    void printPrim();
    void printKruskal();
    void print_bellman_ford(int source, int end);

    int findSet(Subset **subsets, int i);
    void unionSets(Subset **subsets, int x, int y);

    int getNumVertices() const;

    int getNumEdges() const;

    void generate_matrix(int vertices);

    void setNumEdges(int numEdges);

};


#endif //SDIZO_2_INCIDENCE_MATRIX_H

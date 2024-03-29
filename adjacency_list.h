//
// Created by kacpe on 31.05.2023.
//

#ifndef SDIZO_2_ADJACENCY_LIST_H
#define SDIZO_2_ADJACENCY_LIST_H


#include <string>
#include <vector>
#include "double_linked_list.h"
#include "kruskal_edge.h"

class adjacency_list {
private:
    int numVertices;
    int numEdges;
    bool directed;
    double_linked_list** adjacency_lists = nullptr;
public:
    struct Subset {
        int parent;
        int rank;
    };

    adjacency_list(int vertices, int density, bool directed);
    adjacency_list(std::string file, bool directed);

    void setNumEdges(int numEdges);



    ~adjacency_list();
    void add_directed_edge(int start,int end, int weight);
    void add_undirected_edge(int start,int end, int weight);
    int are_connected(int start,int end);
    void display_lists();

    void prim(int** parent_p = nullptr,bool print = false);
    void kruskal(kruskal_edge*** mst_p = nullptr,bool print = false);
    void dijkstra(int source, int end,int** parent_p = nullptr,int** distance_p = nullptr, bool print = false);
    void bellman_ford(int source, int end,int** parent_p = nullptr,int** distance_p = nullptr, bool print = false);

    void print_dijkstra(int source, int end);
    void printPrim();
    void printKruskal();
    void print_bellman_ford(int source, int end);

    void unionSets(Subset **subsets, int x, int y);
    int findSet(Subset **subsets, int i);

    void generate_graph(int vertices);

};


#endif //SDIZO_2_ADJACENCY_LIST_H

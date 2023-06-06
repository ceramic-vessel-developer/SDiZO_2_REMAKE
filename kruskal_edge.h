//
// Created by kacpe on 04.06.2023.
//

#ifndef TEST_KRUSKAL_EDGE_H
#define TEST_KRUSKAL_EDGE_H


class kruskal_edge {
public:
    int u; // Start vertex
    int v; // End vertex
    int weight; // Weight of the edge

    kruskal_edge(int start, int end, int w) : u(start), v(end), weight(w) {}
};


#endif //TEST_KRUSKAL_EDGE_H

//
// Created by kacpe on 08.06.2023.
//

#ifndef SDIZO_2_REMAKE_DIJKSTRAHEAP_H
#define SDIZO_2_REMAKE_DIJKSTRAHEAP_H


#include "dijkstraVertex.h"

class DijkstraHeap {
    int heapSize;
    int originalSize;

public:

    dijkstraVertex **dijkstraVertices;
    int *position;

    DijkstraHeap(int);

    ~DijkstraHeap();

    bool is_in_heap(int);

    bool is_not_empty();

    void create_heap();

    void heapify_down(int);

    dijkstraVertex *pop_min();
};


#endif //SDIZO_2_REMAKE_DIJKSTRAHEAP_H

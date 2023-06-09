//
// Created by kacpe on 08.06.2023.
//

#ifndef SDIZO_2_REMAKE_PRIMHEAP_H
#define SDIZO_2_REMAKE_PRIMHEAP_H


#include "primVertex.h"

class primHeap {
    int heapSize;
    int originalSize;

public:
    primVertex **primVertices;
    int *position; // position[i] pokazuje gdzie w stosie jest wierzchołek 'i'

    primHeap(int);

    ~primHeap();

    bool is_in_heap(int);

    bool is_not_empty();

    void create_heap();

    void heapify(int);

    primVertex *pop();
};


#endif //SDIZO_2_REMAKE_PRIMHEAP_H

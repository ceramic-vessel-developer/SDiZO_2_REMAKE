//
// Created by kacpe on 06.06.2023.
//

#ifndef SDIZO_2_REMAKE_SORT_H
#define SDIZO_2_REMAKE_SORT_H


#include "kruskal_edge.h"

class Sort {
public:
    static int partition(kruskal_edge* arr[], int low, int high);
    static void quickSortEdges(kruskal_edge* arr[], int low, int high);
    static void swapEdges(kruskal_edge** a, kruskal_edge** b);

};


#endif //SDIZO_2_REMAKE_SORT_H

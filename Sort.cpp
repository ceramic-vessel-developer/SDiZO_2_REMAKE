//
// Created by kacpe on 06.06.2023.
//

#include "Sort.h"

int Sort::partition(kruskal_edge **arr, int low, int high) {
    int pivot = arr[high]->weight;
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j]->weight <= pivot) {
            i++;
            swapEdges(&arr[i], &arr[j]);
        }
    }

    swapEdges(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void Sort::quickSortEdges(kruskal_edge **arr, int low, int high) {
    if (low < high) {
        int pivot = partition(arr, low, high);

        quickSortEdges(arr, low, pivot - 1);
        quickSortEdges(arr, pivot + 1, high);
    }

}

void Sort::swapEdges(kruskal_edge** a, kruskal_edge** b) {
    kruskal_edge** temp = a;
    a = b;
    b = temp;
}

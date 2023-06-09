//
// Created by kacpe on 08.06.2023.
//

#include "DijkstraHeap.h"
#include <climits>
DijkstraHeap::DijkstraHeap(int vertices) {
    dijkstraVertices = new dijkstraVertex *[vertices];
    position = new int[vertices];
    // stworzenie pomocniczych obiektów - wierzchołków (numer wierzchołka, distance)
    for (int i = 0; i < vertices; ++i) {
        dijkstraVertices[i] = new dijkstraVertex(i, INT_MAX,-1);
        position[i] = i;
    }
    heapSize = vertices;
    originalSize = vertices;
}

DijkstraHeap::~DijkstraHeap() {
    for (int i = 0; i < originalSize; ++i) {
        delete dijkstraVertices[i];
    }
    delete[] dijkstraVertices;
    delete[] position;
}

bool DijkstraHeap::is_in_heap(int vertex) {
    return position[vertex] < heapSize;
}

bool DijkstraHeap::is_not_empty() {
    return heapSize > 0;
}

void DijkstraHeap::create_heap() {
    for (int i = (heapSize - 2) / 2; i >= 0; --i)
        heapify_down(i);
}

void DijkstraHeap::heapify_down(int parent_index) {
    int smallestIndex = parent_index;
    int leftIndex = 2 * parent_index + 1;
    int rightIndex = 2 * parent_index + 2;
    if ((leftIndex < heapSize) &&
        dijkstraVertices[leftIndex]->distance < dijkstraVertices[smallestIndex]->distance)
        smallestIndex = leftIndex;
    if ((rightIndex < heapSize) &&
        dijkstraVertices[rightIndex]->distance < dijkstraVertices[smallestIndex]->distance)
        smallestIndex = rightIndex;
    if (smallestIndex != parent_index) {
        // zamiana elementów stosu oraz odnośników do ich pozycji
        position[dijkstraVertices[smallestIndex]->index] = parent_index;
        position[dijkstraVertices[parent_index]->index] = smallestIndex;
        auto swap = dijkstraVertices[parent_index];
        dijkstraVertices[parent_index] = dijkstraVertices[smallestIndex];
        dijkstraVertices[smallestIndex] = swap;
        heapify_down(smallestIndex);
    }
}

dijkstraVertex *DijkstraHeap::pop_min() {
    if (heapSize > 0) {
        --heapSize;
        dijkstraVertex *mini = dijkstraVertices[0];
        position[dijkstraVertices[0]->index] = heapSize;
        position[dijkstraVertices[heapSize]->index] = 0;
        auto swap = dijkstraVertices[0];
        dijkstraVertices[0] = dijkstraVertices[heapSize];
        dijkstraVertices[heapSize] = swap;
        return mini;
    }
    return nullptr;
}

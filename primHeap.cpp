//
// Created by kacpe on 08.06.2023.
//

#include <climits>
#include "primHeap.h"

primHeap::primHeap(int vertices) {
    primVertices = new primVertex *[vertices];
    position = new int[vertices];
    for (int i = 0; i < vertices; ++i) {
        primVertices[i] = new primVertex( INT_MAX,i);
        position[i] = i;
    }
    heapSize = vertices;
    originalSize = vertices;
}

primHeap::~primHeap() {
    for (int i = 0; i < originalSize; ++i) {
        delete primVertices[i];
    }
    delete[] primVertices;
    delete[] position;
}


bool primHeap::is_not_empty() {
    return heapSize > 0;
}

void primHeap::create_heap() {
    for (int i = (heapSize - 2) / 2; i >= 0; --i)
        heapify(i);
}

void primHeap::heapify(int parentIndex) {
    int smallestIndex = parentIndex;
    int leftIndex = 2 * parentIndex + 1;
    int rightIndex = 2 * parentIndex + 2;
    if ((leftIndex < heapSize) && primVertices[leftIndex]->key < primVertices[smallestIndex]->key)
        smallestIndex = leftIndex;
    if ((rightIndex < heapSize) && primVertices[rightIndex]->key < primVertices[smallestIndex]->key)
        smallestIndex = rightIndex;
    if (smallestIndex != parentIndex) {
        position[primVertices[smallestIndex]->vertex] = parentIndex;
        position[primVertices[parentIndex]->vertex] = smallestIndex;
        auto swap = primVertices[parentIndex];
        primVertices[parentIndex] = primVertices[smallestIndex];
        primVertices[smallestIndex] = swap;
        heapify(smallestIndex);
    }
}

primVertex *primHeap::pop() {
    if (heapSize > 0) {
        --heapSize;
        primVertex *mini = primVertices[0];
        position[primVertices[0]->vertex] = heapSize;
        position[primVertices[heapSize]->vertex] = 0;
        auto swap = primVertices[0];
        primVertices[0] = primVertices[heapSize];
        primVertices[heapSize] = swap;
        return mini;
    }
    return nullptr;
}

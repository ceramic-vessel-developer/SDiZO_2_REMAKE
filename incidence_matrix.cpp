//
// Created by kacpe on 29.05.2023.
//

#include <cstdlib>
#include <iostream>
#include <fstream>
#include "incidence_matrix.h"
#include "primVertex.h"
#include "dijkstraVertex.h"
#include "Sort.h"
#include "DijkstraHeap.h"
#include "primHeap.h"
#include "double_linked_list_int.h"
#include <vector>
#include <queue>
#include <climits>
#include <stack>
#include <algorithm>

incidence_matrix::incidence_matrix(int vertices, int density, bool directed) {
    srand(time(NULL));
    if (vertices<0) vertices = 0;
    if (density<0) density = 0;
    if (density > 100) density = 100;
    //declare basic information
    this->directed = directed;
    this->numVertices = vertices;
    if (directed){
        this->numEdges = (int)(((float)(vertices * (vertices - 1))*density)/100);
        if (numEdges < vertices) {
            numEdges = vertices;
        }
    }else{
        this->numEdges = (int)(((float)(vertices * (vertices - 1) / 2)*density)/100);
        if (numEdges < vertices - 1) {
            numEdges = vertices - 1;
        }

    }
    if (numEdges<=0){
        numEdges = 0;
        this->matrix = nullptr;
        this->weights = nullptr;
    } else {
        if (numVertices > 1) {
            this->matrix = new int *[numEdges];
            this->weights = new int[numEdges];
            for (int i = 0; i < numEdges; ++i) {
                matrix[i] = new int[numVertices];
                for (int j = 0; j < numVertices; ++j) {
                    matrix[i][j] = 0;
                }
            }
        }else{
            numEdges = 0;
        }
    }


}

void incidence_matrix::generate_matrix(int vertices){
    if (this->directed){
        //Generating matrix for directed graph
        if (numEdges < vertices) {
            numEdges = vertices;
        }
        int *verticesArray = new int[vertices - 1];
        for (int i = 0; i < vertices - 1; ++i) {
            verticesArray[i] = i + 1;
        }
        int vertexStart = 0;
        int vertexEnd;
        int edgeIndex = 0;

        // Ensuring that graph is connected
        for (int i = vertices - 1; i > 0; --i) {
            vertexEnd = rand()%i;

            add_directed_edge(vertexStart,verticesArray[vertexEnd],rand()%1000+1);
            ++edgeIndex;
            vertexStart = verticesArray[vertexEnd];
            verticesArray[vertexEnd] = verticesArray[i - 1];
        }
        delete[] verticesArray;

        //creating a cycle
        add_directed_edge(vertexStart,0,rand()%1000+1);

        //adding the rest of the graph

        int remaining_edges = numEdges - numVertices;
        int start;
        int end;

        for (int i = 0; i < remaining_edges; ++i) {
            while (true){
                start = rand()%numVertices;
                end = rand()%numVertices;
                if (are_connected(start,end) < 0) break;
            }
            add_directed_edge(start, end, rand()%1000+1);
        }

    }
    else{
        //Generating matrix for undirected graph
        if (numEdges < vertices - 1) {
            numEdges = vertices - 1;
        }
        int *verticesArray = new int[vertices - 1];
        for (int i = 0; i < vertices - 1; ++i) {
            verticesArray[i] = i + 1;
        }
        int vertexStart = 0;
        int vertexEnd;
        int edgeIndex = 0;

        // Ensuring that graph is connected
        for (int i = vertices - 1; i > 0; --i) {
            vertexEnd = rand()%i;

            add_undirected_edge(vertexStart,verticesArray[vertexEnd],rand()%1000+1);
            ++edgeIndex;
            vertexStart = verticesArray[vertexEnd];
            verticesArray[vertexEnd] = verticesArray[i - 1];
        }
        delete[] verticesArray;
        //adding the rest of the graph

        int remaining_edges = numEdges - numVertices + 1;
        int start;
        int end;

        for (int i = 0; i < remaining_edges; ++i) {
            while (true){
                start = rand()%numVertices;
                end = rand()%numVertices;
                if (are_connected(start,end) < 0) break;
            }
            add_undirected_edge(start, end, rand()%1000+1);
        }

    }

}

void incidence_matrix::displayMatrix() {
    for (int i = 0; i < numEdges; i++) {
        for (int j = 0; j < numVertices; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Weights:" << std::endl;
    for (int i = 0; i < numEdges; ++i) {
        std::cout << weights[i] << " ";
    }
    std::cout << std::endl;
}

incidence_matrix::incidence_matrix(std::string file, bool directed) {
    std::string path = "../input_files/"+file;
    std::fstream f;
    int start, end, weight;

    f.open(path);
    if (f.is_open()){
        f >> numVertices;
        f >> numEdges;
        this->directed = directed;
        if (numEdges<=0){
            numEdges = 0;
            this->matrix = nullptr;
            this->weights = nullptr;
        } else {
            if (numVertices > 1) {
                this->matrix = new int *[numEdges];
                this->weights = new int[numEdges];
                for (int i = 0; i < numEdges; ++i) {
                    matrix[i] = new int[numVertices];
                    for (int j = 0; j < numVertices; ++j) {
                        matrix[i][j] = 0;
                    }
                }
            } else {
                numEdges = 0;
            }

            if (directed) {
                while (f >> start) {
                    f >> end;
                    f >> weight;
                    add_directed_edge(start, end, weight);
                }
            } else {
                while (f >> start) {
                    f >> end;
                    f >> weight;
                    add_undirected_edge(start, end, weight);
                }
            }
        }
    }else{
        std::cout << "Plik nie zostal poprawnie otwarty" << std::endl;
    }

}

incidence_matrix::~incidence_matrix() {
    if(weights != nullptr){
        delete[] weights;
    }
    if(matrix != nullptr) {
        for (int i = 0; i < numEdges; ++i) {
            delete[] matrix[i];
        }
        delete[] matrix;
    }
}

void incidence_matrix::add_directed_edge(int start, int end, int weight) {
    if (start < 0 || start >= numVertices || end < 0 || end >= numVertices) {
        std::cout << "Invalid start or end vertex!" << std::endl;
        return;
    }

    // Find an available edge slot in the matrix
    int edge = -1;
    bool empty;
    for (int i = 0; i < numEdges; i++) {
        empty = true;
        for (int j = 0; j <numVertices ; ++j) {
            if(matrix[i][j] != 0){
                empty = false;
                break;
            }
        }
        if (empty){
            edge = i;
            break;
        }
    }

    if (edge == -1) {
        std::cout << "No available edge slot!" << std::endl;
        return;
    }

    // Add the edge between start and end vertices
    matrix[edge][start] = 1;
    matrix[edge][end] = -1;
    weights[edge] = weight;

}

void incidence_matrix::add_undirected_edge(int start, int end, int weight) {
    if (start < 0 || start >= numVertices || end < 0 || end >= numVertices) {
        std::cout << "Invalid start or end vertex!" << std::endl;
        return;
    }

    // Find an available edge slot in the matrix
    int edge = -1;
    bool empty;
    for (int i = 0; i < numEdges; i++) {
        empty = true;
        for (int j = 0; j <numVertices ; ++j) {
            if(matrix[i][j] != 0){
                empty = false;
                break;
            }
        }
        if (empty){
            edge = i;
            break;
        }
    }

    if (edge == -1) {
        std::cout << "No available edge slot!" << std::endl;
        return;
    }

    // Add the edge between start and end vertices
    matrix[edge][start] = 1;
    matrix[edge][end] = 1;
    weights[edge] = weight;

}

int incidence_matrix::are_connected(int start, int end) {
    for (int j = 0; j < numEdges; j++) {
        if (matrix[j][start] == 1 && matrix[j][end] != 0) {
            return weights[j];
        }
    }
    return -1;
}

int *incidence_matrix::neigbours(int vertex) {
    int* array = new int[numVertices-1];
    int index = 0;
    for (int i = 0; i < numVertices-1; ++i) {
        array[i] = 0;
    }
    for (int i = 0; i < numEdges; ++i) {
        if (matrix[i][vertex]==1){
            for (int j = 0; j < vertex; ++j) {
                if (matrix[i][j] != 0){
                    array[index] = j;
                    break;
                }
            }

            for (int j = vertex+1; j < numVertices; ++j) {
                if (matrix[i][j] != 0){
                    array[index] = j;
                    break;
                }
            }
        }
    }
    return array;
}

int incidence_matrix::getNumVertices() const {
    return numVertices;
}

int incidence_matrix::getNumEdges() const {
    return numEdges;
}

void incidence_matrix::prim() {
    if (numVertices == 0){
        return;
    } else if(numVertices == 1){
        return;
    }



    int *parent = new int[numVertices];
    for (int i = 0; i < numVertices; ++i) {
        parent[i] = -1;
    }

    int* key = new int[numVertices];
    for (int i = 0; i < numVertices; ++i) {
        key[i] =INT_MAX;
    }
    bool* mstSet = new bool[numVertices];
    for (int i = 0; i < numVertices; ++i) {
        mstSet[i] = false;
    }
    primHeap* pq = new primHeap(numVertices);
    // Start with vertex 0
    pq->primVertices[0]->key = 0;
    key[0] = 0;

    // Loop until all vertices are included in MST
    while (pq->is_not_empty()) {
        pq->create_heap();
        auto elem = pq->pop();
        int u = elem->vertex;

        if (mstSet[u]) continue;

        mstSet[u] = true; // Add the extracted vertex to the MST set

        // Update key value and parent index of the adjacent vertices of the extracted vertex
        for (int e = 0; e < numEdges; e++) {
            int v = -1;
            if (matrix[e][u] != 0) {
                // Find the other vertex of the edge incident on u
                for (int i = 0; i < numVertices; i++) {
                    if (matrix[e][i] == 1 && i != u) {
                        v = i;
                        break;
                    }
                }
            }

            if (v != -1 && !mstSet[v] && weights[e] < key[v]) {
                key[v] = weights[e];
                parent[v] = u;
                pq->primVertices[pq->position[v]]->key = weights[e];
            }
        }
    }
    delete[] mstSet;
    delete[] key;
    delete[] parent;
    delete pq;
}

void incidence_matrix::kruskal() {
    if (numVertices == 0){
        return;
    } else if(numVertices == 1){
        return;
    }


    int v = 0,u = 0, edge = 0;


    kruskal_edge** edges = new kruskal_edge * [numEdges];

    for (int e = 0; e < numEdges; e++) {
        v = -1;
        u = -1;
        for (int i = 0; i < numVertices; ++i) {
            if(matrix[e][i]){
                if (v == -1){
                    v = i;
                }else{
                    u = i;
                    edges[edge] = new kruskal_edge(v, u, weights[e]);
                    edge++;
                    break;
                }
            }
        }
    }



    Sort::quickSortEdges(edges,0,numEdges-1);

//    std::vector<Subset> subsets(numVertices);// Array to store subsets for union-find
    Subset** subsets = new Subset * [numVertices];


    for (int v = 0; v < numVertices; v++) {
        subsets[v] = new Subset();
        subsets[v]->parent = v;
        subsets[v]->rank = 0;
    }

    kruskal_edge** mst = new kruskal_edge * [numVertices-1];
    int edgeCount = 0;

    for (int i = 0; i< numEdges;i++) {
        int u = edges[i]->u;
        int v = edges[i]->v;

        int uParent = findSet(subsets, u);
        int vParent = findSet(subsets, v);

        if (uParent != vParent) {
            mst[edgeCount] = edges[i];
            unionSets(subsets, uParent, vParent);
            edgeCount++;
        }

        if (edgeCount == numVertices - 1) {
            break; // MST is complete
        }
    }
    delete [] subsets;
    delete[] edges;

}

void incidence_matrix::bellman_ford(int source, int end) {
    if (source > numVertices-1 || source < 0 || end > numVertices-1 || end < 0){
        return;
    }

    if (numVertices == 0){
        return;
    }else if (numVertices == 1){
        return;
    }

    int* distance = new int[numVertices];
    for (int i = 0; i < numVertices; ++i) {
        distance[i] = INT_MAX;
    }
    int* parent = new int[numVertices];
    for (int i = 0; i < numVertices; ++i) {
        parent[i] = -1;
    }
    int u = 0,v = 0,w;

    // Initialize distance array
    distance[source] = 0;

    // Relax edges repeatedly |V|-1 times
    for (int i = 0; i < numVertices - 1; ++i) {
        for (int j = 0; j< numEdges;j++) {
            u = 0;
            v = 0 ;
            for (int k = 0; k < numVertices; ++k) {
                if (matrix[j][k] == 1){
                    u = k;
                } else if (matrix[j][k] == -1){
                    v = k;
                }
                if (u != 0 && v != 0) break;

            }
            w = weights[j];
            if (distance[u] != INT_MAX && distance[u] + w < distance[v]) {
                distance[v] = distance[u] + w;
                parent[v] = u;
            }
        }
    }
    delete[] parent;
    delete[] distance;
}

void incidence_matrix::printPrim() {
    if (numVertices == 0){
        std::cout << "Graf jest pusty !" << std::endl;
        return;
    } else if(numVertices == 1){
        std::cout << "Minimum Spanning Tree:" << std::endl;
        return;
    }

    int *parent = new int[numVertices];
    for (int i = 0; i < numVertices; ++i) {
        parent[i] = -1;
    }

    int* key = new int[numVertices];
    for (int i = 0; i < numVertices; ++i) {
        key[i] =INT_MAX;
    }
    bool* mstSet = new bool[numVertices];
    for (int i = 0; i < numVertices; ++i) {
        mstSet[i] = false;
    }

    primHeap* pq = new primHeap(numVertices);
    // Start with vertex 0
    pq->primVertices[0]->key = 0;
    key[0] = 0;

    // Loop until all vertices are included in MST
    while (pq->is_not_empty()) {
        pq->create_heap();
        auto elem = pq->pop();
        int u = elem->vertex;


        if (mstSet[u]) continue;

        mstSet[u] = true; // Add the extracted vertex to the MST set

        // Update key value and parent index of the adjacent vertices of the extracted vertex
        for (int e = 0; e < numEdges; e++) {
            int v = -1;
            if (matrix[e][u] != 0) {
                // Find the other vertex of the edge incident on u
                for (int i = 0; i < numVertices; i++) {
                    if (matrix[e][i] == 1 && i != u) {
                        v = i;
                        break;
                    }
                }
            }

            if (v != -1 && !mstSet[v] && weights[e] < key[v]) {
                key[v] = weights[e];
                parent[v] = u;
                pq->primVertices[pq->position[v]]->key = weights[e];
            }
        }
    }

    std::cout << "Minimum Spanning Tree:\n";
    std::cout << "Edge\tWeight\n";
    for (int i = 1; i < numVertices; i++) {
        int u = parent[i];
        std::cout << u << " - " << i << "\t" << key[i] << std::endl;
    }
    delete[] mstSet;
    delete[] key;
    delete[] parent;
    delete pq;
}

void incidence_matrix::dijkstra(int source, int end) {
    if (source > numVertices-1 || source < 0 || end > numVertices-1 || end < 0){
        return;
    }

    if (numVertices == 0){
        return;
    }else if (numVertices == 1){
        return;
    }

    int *parent = new int[numVertices];
    for (int i = 0; i < numVertices; ++i) {
        parent[i] = -1;
    }

    int* dist = new int[numVertices];
    for (int i = 0; i < numVertices; ++i) {
        dist[i] =INT_MAX;
    }
    bool* visited = new bool[numVertices];
    for (int i = 0; i < numVertices; ++i) {
        visited[i] = false;
    }


    DijkstraHeap* pq = new DijkstraHeap(numVertices); // Priority queue to store vertices and their distances

    dist[source] = 0; // Distance from source to itself is 0
    pq->dijkstraVertices[source]->distance = dist[source];
    pq->dijkstraVertices[source]->parent = -1;

    while (pq->is_not_empty()) {
        pq->create_heap();
        auto elem = pq->pop_min();
        int u = elem->index;

        if (visited[u]) {
            continue; // Skip if the vertex is already visited
        }

        visited[u] = true; // Mark the vertex as visited

        // Update the distances of the neighboring vertices
        for (int e = 0; e < numEdges; e++) {
            if (matrix[e][u] == 1) {
                int v = -1; // Neighbor vertex

                for (int j = 0; j < numVertices; j++) {
                    if (matrix[e][j] == -1 ) {
                        v = j;
                        break;
                    }
                }

                if (v != -1) {
                    int weight = weights[e];

                    if (!visited[v] && dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                        dist[v] = dist[u] + weight; // Update the distance
                        pq->dijkstraVertices[v]->distance = dist[v];
                        pq->dijkstraVertices[v]->parent = u;
                        parent[v] = u;
                    }
                }
            }
        }
    }
    delete[] dist;
    delete[] parent;
    delete[] visited;
    delete pq;

}

void incidence_matrix::printDijkstra(int source, int end) {
    if (source > numVertices-1 || source < 0 || end > numVertices-1 || end < 0){
        std::cout << "Nieprawidlowe wiercholki" << std::endl;
        return;
    }
    if (numVertices == 0){
        std::cout <<"Sciezka nie istnieje poniewaz graf jest pusty" << std::endl;
        return;
    }else if (numVertices == 1){
        std::cout << "Distance: " << end << " 0" << std::endl;
        std::cout << "Path: " << std::endl;
        return;
    }
    int *parent = new int[numVertices];
    for (int i = 0; i < numVertices; ++i) {
        parent[i] = -1;
    }

    int* dist = new int[numVertices];
    for (int i = 0; i < numVertices; ++i) {
        dist[i] =INT_MAX;
    }
    bool* visited = new bool[numVertices];
    for (int i = 0; i < numVertices; ++i) {
        visited[i] = false;
    }


    DijkstraHeap* pq = new DijkstraHeap(numVertices); // Priority queue to store vertices and their distances

    dist[source] = 0; // Distance from source to itself is 0
    pq->dijkstraVertices[source]->distance = dist[source];
    pq->dijkstraVertices[source]->parent = -1;

    while (pq->is_not_empty()) {
        pq->create_heap();
        auto elem = pq->pop_min();
        int u = elem->index;

        if (visited[u]) {
            continue; // Skip if the vertex is already visited
        }

        visited[u] = true; // Mark the vertex as visited

        // Update the distances of the neighboring vertices
        for (int e = 0; e < numVertices; e++) {
            if (matrix[e][u] == 1) {
                int v = -1; // Neighbor vertex

                for (int j = 0; j < numVertices; j++) {
                    if (matrix[e][j] == -1 ) {
                        v = j;
                        break;
                    }
                }

                if (v != -1) {
                    int weight = weights[e];

                    if (!visited[v] && dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                        dist[v] = dist[u] + weight; // Update the distance
                        pq->dijkstraVertices[pq->position[v]]->distance = dist[v];
                        pq->dijkstraVertices[pq->position[v]]->parent = u;
                        parent[v] = u;
                    }
                }
            }
        }
    }
    std::cout << "Shortest path from source to vertex " << end << ":" << std::endl;
    if (dist[end] == INT_MAX) {
        std::cout << "No path exists." << std::endl;
    } else {
        std::cout << "Distance: " << dist[end] << std::endl;
        std::cout << "Path: ";
        double_linked_list_int* path = new double_linked_list_int(0);
        int current = end;
        int path_length = 0;
        while (current != -1) {
            path->add_back(current);
            current = parent[current];
        }
        while (path->is_not_empty()) {
            std::cout << path->pop_back() << " ";
        }
        std::cout << std::endl;
    }
    delete[] dist;
    delete[] parent;
    delete[] visited;
    delete pq;
}

void incidence_matrix::printKruskal() {
    if (numVertices == 0){
        std::cout << "Graf jest pusty !" << std::endl;
        return;
    } else if(numVertices == 1){
        std::cout << "Minimum Spanning Tree:" << std::endl;
        return;
    }

    int v = 0,u = 0, edge = 0;

    // Create a vector of edges
    kruskal_edge** edges = new kruskal_edge * [numEdges];

    for (int e = 0; e < numEdges; e++) {
        v = -1;
        u = -1;
        for (int i = 0; i < numVertices; ++i) {
            if(matrix[e][i]){
                if (v == -1){
                    v = i;
                }else{
                    u = i;
                    edges[edge] = new kruskal_edge(v, u, weights[e]);
                    edge++;
                    break;
                }
            }
        }
    }


    // Sort the edges in non-decreasing order of weights

    Sort::quickSortEdges(edges,0,numEdges-1);

//    std::vector<Subset> subsets(numVertices);// Array to store subsets for union-find
    Subset** subsets = new Subset * [numVertices];


    for (int v = 0; v < numVertices; v++) {
        subsets[v] = new Subset();
        subsets[v]->parent = v;
        subsets[v]->rank = 0;
    }

    kruskal_edge** mst = new kruskal_edge * [numVertices-1];

    int edgeCount = 0; // Count of edges included in the MST

    for (int i = 0; i< numEdges;i++) {
        int u = edges[i]->u;
        int v = edges[i]->v;

        int uParent = findSet(subsets, u);
        int vParent = findSet(subsets, v);

        if (uParent != vParent) {
            mst[edgeCount] = edges[i];
            unionSets(subsets, uParent, vParent);
            edgeCount++;
        }

        if (edgeCount == numVertices - 1) {
            break; // MST is complete
        }
    }
    std::cout << "Minimum Spanning Tree:\n";
    std::cout << "Edge\tWeight\n";
    for (int i = 0; i<numVertices-1; i++) {
        std::cout << mst[i]->u << " - " << mst[i]->v << " \t " << mst[i]->weight << "\n";
    }
    delete [] subsets;
    delete[] edges;
}

int incidence_matrix::findSet(Subset **subsets, int i) {
    if (subsets[i]->parent != i) {
        subsets[i]->parent = findSet(subsets, subsets[i]->parent);
    }
    return subsets[i]->parent;
}

void incidence_matrix::unionSets(Subset **subsets, int x, int y) {
    int xRoot = findSet(subsets, x);
    int yRoot = findSet(subsets, y);

    if (subsets[xRoot]->rank < subsets[yRoot]->rank) {
        subsets[xRoot]->parent = yRoot;
    } else if (subsets[xRoot]->rank > subsets[yRoot]->rank) {
        subsets[yRoot]->parent = xRoot;
    } else {
        subsets[yRoot]->parent = xRoot;
        subsets[xRoot]->rank++;
    }
}

bool incidence_matrix::compareEdges(const kruskal_edge &a, const kruskal_edge &b) {
    return a.weight < b.weight;
}

void incidence_matrix::print_bellman_ford(int source, int end) {
    if (source > numVertices-1 || source < 0 || end > numVertices-1 || end < 0){
        std::cout << "Nieprawidlowe wiercholki" << std::endl;
        return;
    }
    if (numVertices == 0){
        std::cout <<"Sciezka nie istnieje poniewaz graf jest pusty" << std::endl;
        return;
    }else if (numVertices == 1){
        std::cout << "Distance: " << end << " 0" << std::endl;
        std::cout << "Path: " << std::endl;
        return;
    }
    int* distance = new int[numVertices];
    for (int i = 0; i < numVertices; ++i) {
        distance[i] = INT_MAX;
    }
    int* parent = new int[numVertices];
    for (int i = 0; i < numVertices; ++i) {
        parent[i] = -1;
    }
    int u = 0,v = 0,w;

    // Initialize distance array
    distance[source] = 0;

    // Relax edges repeatedly |V|-1 times
    for (int i = 0; i < numVertices - 1; ++i) {
        for (int j = 0; j< numEdges;j++) {
            u = 0;
            v = 0 ;
            for (int k = 0; k < numVertices; ++k) {
                if (matrix[j][k] == 1){
                    u = k;
                } else if (matrix[j][k] == -1){
                    v = k;
                }
                if (u != 0 && v != 0) break;
            }
            w = weights[j];
            if (distance[u] != INT_MAX && distance[u] + w < distance[v]) {
                distance[v] = distance[u] + w;
                parent[v] = u;
            }
        }
    }

    std::cout << "Shortest path from source to vertex " << end << ":" << std::endl;
    if (distance[end] == INT_MAX) {
        std::cout << "No path exists." << std::endl;
    } else {
        std::cout << "Distance: " << distance[end] << std::endl;
        std::cout << "Path: ";
        double_linked_list_int* path = new double_linked_list_int(0);
        int current = end;
        while (current != -1) {
            path->add_back(current);
            current = parent[current];
        }
        while (path->is_not_empty()) {
            std::cout << path->pop_back()<< " ";
        }
        std::cout << std::endl;
        delete path;
    }
    delete[] parent;
    delete[] distance;

}

void incidence_matrix::setNumEdges(int numEdges) {
    incidence_matrix::numEdges = numEdges;
}

incidence_matrix::incidence_matrix() = default;


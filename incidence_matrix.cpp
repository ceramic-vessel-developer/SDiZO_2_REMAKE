//
// Created by kacpe on 29.05.2023.
//

#include <cstdlib>
#include <iostream>
#include <fstream>
#include "incidence_matrix.h"
#include "primVertex.h"
#include "dijkstraVertex.h"
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



    std::vector<int> parent(numVertices, -1); // Array to store constructed MST
    std::vector<int> key(numVertices, INT_MAX); // Key values used to pick the minimum weight edge
    std::vector<bool> mstSet(numVertices, false); // To represent set of vertices included in MST

    std::priority_queue<primVertex, std::vector<primVertex>, std::greater<primVertex>> pq; // Priority queue to store vertices and their weights

    // Start with vertex 0
    pq.push(primVertex(0,0)); // Vertex 0 has key 0
    key[0] = 0;

    // Loop until all vertices are included in MST
    while (!pq.empty()) {
        int u = pq.top().vertex; // Extract the vertex with minimum key value
        pq.pop();

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
                pq.push(primVertex(key[v],v));
            }
        }
    }
}

void incidence_matrix::kruskal() {
    if (numVertices == 0){
        return;
    } else if(numVertices == 1){
        return;
    }


    int v = 0,u = 0;


    std::vector<kruskal_edge> edges;

    for (int e = 0; e < numEdges; e++) {
        v = -1;
        u = -1;
        for (int i = 0; i < numVertices; ++i) {
            if(matrix[e][i]){
                if (v == -1){
                    v = i;
                }else{
                    u = i;
                    edges.push_back(kruskal_edge(v, u, weights[e]));
                    break;
                }
            }
        }
    }



    std::sort(edges.begin(), edges.end(), compareEdges);

    std::vector<Subset> subsets(numVertices);

    for (int v = 0; v < numVertices; v++) {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    std::vector<kruskal_edge> mst;
    int edgeCount = 0;

    for (const kruskal_edge& edge : edges) {
        int u = edge.u;
        int v = edge.v;

        int uParent = findSet(subsets, u);
        int vParent = findSet(subsets, v);

        if (uParent != vParent) {
            mst.push_back(edge);
            unionSets(subsets, uParent, vParent);
            edgeCount++;
        }

        if (edgeCount == numVertices - 1) {
            break;
        }
    }
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

    std::vector<int> distance(numVertices, INT_MAX);
    std::vector<int> predecessor(numVertices, -1);
    int u,v,w;

    // Initialize distance array
    distance[source] = 0;

    // Relax edges repeatedly |V|-1 times
    for (int i = 0; i < numVertices - 1; ++i) {
        for (int j = 0; j< numEdges;j++) {
            for (int k = 0; k < numVertices; ++k) {
                if (matrix[j][k] == 1){
                    u = k;
                } else if (matrix[j][k] == -1){
                    v = k;
                }
            }
            w = weights[j];
            if (distance[u] != INT_MAX && distance[u] + w < distance[v]) {
                distance[v] = distance[u] + w;
                predecessor[v] = u;
            }
        }
    }
}

void incidence_matrix::printPrim() {
    if (numVertices == 0){
        std::cout << "Graf jest pusty !" << std::endl;
        return;
    } else if(numVertices == 1){
        std::cout << "Minimum Spanning Tree:" << std::endl;
        return;
    }

    std::vector<int> parent(numVertices, -1); // Array to store constructed MST
    std::vector<int> key(numVertices, INT_MAX); // Key values used to pick the minimum weight edge
    std::vector<bool> mstSet(numVertices, false); // To represent set of vertices included in MST

    std::priority_queue<primVertex, std::vector<primVertex>, std::greater<primVertex>> pq; // Priority queue to store vertices and their weights

    // Start with vertex 0
    pq.push(primVertex(0,0)); // Vertex 0 has key 0
    key[0] = 0;

    // Loop until all vertices are included in MST
    while (!pq.empty()) {
        int u = pq.top().vertex; // Extract the vertex with minimum key value
        pq.pop();

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
                pq.push(primVertex(key[v],v));
            }
        }
    }

    std::cout << "Minimum Spanning Tree:\n";
    std::cout << "Edge\tWeight\n";
    for (int i = 1; i < numVertices; i++) {
        int u = parent[i];
        std::cout << u << " - " << i << "\t" << key[i] << std::endl;
    }
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

    std::vector<int> dist(numVertices, INT_MAX); // Array to store the shortest distances from the source
    std::vector<bool> visited(numVertices, false); // Array to track visited vertices
    std::vector<int> parent(numVertices, -1);


    std::priority_queue<dijkstraVertex, std::vector<dijkstraVertex>, std::greater<dijkstraVertex>> pq; // Priority queue to store vertices and their distances

    dist[source] = 0; // Distance from source to itself is 0
    pq.push(dijkstraVertex(source, dist[source], -1)); // Push the source vertex into the priority queue

    while (!pq.empty()) {
        int u = pq.top().index;
        // Extract the vertex with minimum distance
        pq.pop();

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
                        pq.push(dijkstraVertex(v, dist[v],u)); // Push the updated distance into the priority queue
                        parent[v] = u;
                    }
                }
            }
        }
    }

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
    std::vector<int> distance(numVertices, INT_MAX); // Array to store the shortest distances from the source
    std::vector<bool> visited(numVertices, false); // Array to track visited vertices
    std::vector<int> parent(numVertices, -1);


    std::priority_queue<dijkstraVertex, std::vector<dijkstraVertex>, std::greater<dijkstraVertex>> pq; // Priority queue to store vertices and their distances

    distance[source] = 0; // Distance from source to itself is 0
    pq.push(dijkstraVertex(source, distance[source], -1)); // Push the source vertex into the priority queue

    while (!pq.empty()) {
        int u = pq.top().index;
        // Extract the vertex with minimum distance
        pq.pop();

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

                    if (!visited[v] && distance[u] != INT_MAX && distance[u] + weight < distance[v]) {
                        distance[v] = distance[u] + weight; // Update the distance
                        pq.push(dijkstraVertex(v, distance[v], u)); // Push the updated distance into the priority queue
                        parent[v] = u;
                    }
                }
            }
        }
    }
    std::cout << "Shortest path from source to vertex " << end << ":" << std::endl;
    if (distance[end] == INT_MAX) {
        std::cout << "No path exists." << std::endl;
    } else {
        std::cout << "Distance: " << distance[end] << std::endl;
        std::cout << "Path: ";
        std::stack<int> path;
        int current = end;
        while (current != -1) {
            path.push(current);
            current = parent[current];
        }
        while (!path.empty()) {
            std::cout << path.top() << " ";
            path.pop();
        }
        std::cout << std::endl;
    }

}

void incidence_matrix::printKruskal() {
    if (numVertices == 0){
        std::cout << "Graf jest pusty !" << std::endl;
        return;
    } else if(numVertices == 1){
        std::cout << "Minimum Spanning Tree:" << std::endl;
        return;
    }



    int v = 0,u = 0;

    // Create a vector of edges
    std::vector<kruskal_edge> edges;

    for (int e = 0; e < numEdges; e++) {
        v = -1;
        u = -1;
        for (int i = 0; i < numVertices; ++i) {
            if(matrix[e][i]){
                if (v == -1){
                    v = i;
                }else{
                    u = i;
                    edges.push_back(kruskal_edge(v, u, weights[e]));
                    break;
                }
            }
        }
    }


    // Sort the edges in non-decreasing order of weights
    std::sort(edges.begin(), edges.end(), compareEdges);

    std::vector<Subset> subsets(numVertices); // Array to store subsets for union-find

    for (int v = 0; v < numVertices; v++) {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    std::vector<kruskal_edge> mst; // Minimum spanning tree
    int edgeCount = 0; // Count of edges included in the MST

    for (const kruskal_edge& edge : edges) {
        int u = edge.u;
        int v = edge.v;

        int uParent = findSet(subsets, u);
        int vParent = findSet(subsets, v);

        if (uParent != vParent) {
            mst.push_back(edge);
            unionSets(subsets, uParent, vParent);
            edgeCount++;
        }

        if (edgeCount == numVertices - 1) {
            break; // MST is complete
        }
    }
    std::cout << "Minimum Spanning Tree:\n";
    std::cout << "Edge\tWeight\n";
    for (const kruskal_edge& edge : mst) {
        std::cout << edge.u << " - " << edge.v << " \t " << edge.weight << "\n";
    }
}

int incidence_matrix::findSet(std::vector<Subset> &subsets, int i) {
    if (subsets[i].parent != i) {
        subsets[i].parent = findSet(subsets, subsets[i].parent);
    }
    return subsets[i].parent;
}

void incidence_matrix::unionSets(std::vector<Subset> &subsets, int x, int y) {
    int xRoot = findSet(subsets, x);
    int yRoot = findSet(subsets, y);

    if (subsets[xRoot].rank < subsets[yRoot].rank) {
        subsets[xRoot].parent = yRoot;
    } else if (subsets[xRoot].rank > subsets[yRoot].rank) {
        subsets[yRoot].parent = xRoot;
    } else {
        subsets[yRoot].parent = xRoot;
        subsets[xRoot].rank++;
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

    std::vector<int> distance(numVertices, INT_MAX);
    std::vector<int> parent(numVertices, -1);
    int u,v,w;

    // Initialize distance array
    distance[source] = 0;

    // Relax edges repeatedly |V|-1 times
    for (int i = 0; i < numVertices - 1; ++i) {
        for (int j = 0; j< numEdges;j++) {
            for (int k = 0; k < numVertices; ++k) {
                if (matrix[j][k] == 1){
                    u = k;
                } else if (matrix[j][k] == -1){
                    v = k;
                }
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
        std::stack<int> path;
        int current = end;
        while (current != -1) {
            path.push(current);
            current = parent[current];
        }
        while (!path.empty()) {
            std::cout << path.top() << " ";
            path.pop();
        }
        std::cout << std::endl;
    }
}

void incidence_matrix::setNumEdges(int numEdges) {
    incidence_matrix::numEdges = numEdges;
}

incidence_matrix::incidence_matrix() = default;

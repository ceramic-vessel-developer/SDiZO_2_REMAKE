//
// Created by kacpe on 31.05.2023.
//

#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>
#include "adjacency_list.h"
#include "primVertex.h"
#include "dijkstraVertex.h"
#include "primHeap.h"
#include "Sort.h"
#include "DijkstraHeap.h"
#include "double_linked_list_int.h"

adjacency_list::adjacency_list(int vertices, int density, bool directed) {
    if (vertices<0) vertices = 0;
    if (density<0) density = 0;
    if (density > 100) density = 100;
    numVertices = vertices;
    this->directed = directed;
    srand(time(NULL));
    //declare basic information
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
    if (numVertices<=0) {
        numVertices = 0;
        numEdges = 0;
        this->adjacency_lists = nullptr;
    }else{
        this->adjacency_lists = new double_linked_list * [vertices];

        for (int i = 0; i < vertices; ++i) {
            this->adjacency_lists[i] = new double_linked_list();
        }
    }


}

adjacency_list::adjacency_list(std::string file, bool directed) {
    std::string path = "../input_files/"+file;
    std::fstream f;
    int start, end, weight;

    f.open(path);
    if (f.is_open()){
        f >> numVertices;
        f >> numEdges;
        this->directed = directed;
        if (numVertices<=0) {
            numVertices = 0;
            numEdges = 0;
            this->adjacency_lists = nullptr;
        }else{
            this->adjacency_lists = new double_linked_list * [numVertices];

            for (int i = 0; i < numVertices; ++i) {
                this->adjacency_lists[i] = new double_linked_list();
            }
        }
        if (directed){
            while(f >> start){
                f >> end;
                f >> weight;
                add_directed_edge(start, end, weight);
            }
        }else{
            while(f >> start){
                f >> end;
                f >> weight;
                add_undirected_edge(start, end, weight);
            }
        }

    }else{
        std::cout << "Plik nie zostal poprawnie otwarty" << std::endl;
    }


}

adjacency_list::~adjacency_list() {
    for (int i = 0; i < numVertices; ++i) {
        delete adjacency_lists[i];
    }
    delete [] adjacency_lists;
}

void adjacency_list::add_directed_edge(int start, int end, int weight) {
    if (start < 0 || start >= numVertices || end < 0 || end >= numVertices) {
        std::cout << "Invalid start or end vertex!" << std::endl;
        return;
    }

    this->adjacency_lists[start]->add_back(end,weight);

}

void adjacency_list::add_undirected_edge(int start, int end, int weight) {
    if (start < 0 || start >= numVertices || end < 0 || end >= numVertices) {
        std::cout << "Invalid start or end vertex!" << std::endl;
        return;
    }

    this->adjacency_lists[start]->add_back(end,weight);
    this->adjacency_lists[end]->add_back(start,weight);
}

int adjacency_list::are_connected(int start, int end) {
    edge* temp;
    
    temp = adjacency_lists[start]->select(end);

    if (temp){
        return temp->weight;
    }else{
        return -1;
    }
}

double_linked_list* adjacency_list::neigbours(int vertex) {
    return adjacency_lists[vertex];
}

void adjacency_list::display_lists() {
    for (int i = 0; i < numVertices; ++i) {
        std::cout << i <<": ";
        adjacency_lists[i]->show();
        std::cout << std::endl;
    }

}

void adjacency_list::prim() {
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

    pq->primVertices[0]->key = 0;
    key[0] = 0;

    while (pq->is_not_empty()) {
        pq->create_heap();
        auto elem = pq->pop();
        int u = elem->vertex;

        mstSet[u] = true;
        auto list = adjacency_lists[u]->get_head();
        while (list) {
            int v = list->value.neighbour;
            int weight = list->value.weight;
            if (!mstSet[v] && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;
                pq->primVertices[pq->position[v]]->key = weight;
            }
            list = list->next;
        }
    }
    delete[] key;
    delete[] mstSet;
    delete[] parent;
}

void adjacency_list::kruskal() {
    if (numVertices == 0){
        return;
    } else if(numVertices == 1){
        return;
    }
    int edge = 0;
    kruskal_edge** edges = new kruskal_edge * [numEdges*2];
    for (int u = 0; u < numVertices; u++) {
        auto elem = adjacency_lists[u]->get_head();
        while(elem) {
            int v = elem->value.neighbour;
            int weight = elem->value.weight;
            edges[edge] = new kruskal_edge(v,u, weight);
            edge++;
            elem = elem->next;
        }
    }



    Sort::quickSortEdges(edges,0,numEdges-1);

    Subset** subsets = new Subset * [numVertices];

    for (int v = 0; v <numVertices; v++) {
        subsets[v] = new Subset();
        subsets[v]->parent = v;
        subsets[v]->rank = 0;
    }

    kruskal_edge** mst = new kruskal_edge * [numVertices-1];
    int edgeCount = 0;

    for (int i = 0; i< numEdges*2;i++) {
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
            break;
        }
    }
    delete [] subsets;
    delete[] edges;
    delete[] mst;
}

void adjacency_list::dijkstra(int source, int end) {
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

    int* distance = new int[numVertices];
    for (int i = 0; i < numVertices; ++i) {
        distance[i] =INT_MAX;
    }
    bool* visited = new bool[numVertices];
    for (int i = 0; i < numVertices; ++i) {
        visited[i] = false;
    }


    DijkstraHeap* pq = new DijkstraHeap(numVertices);

    distance[source] = 0;
    pq->dijkstraVertices[source]->distance = distance[source];
    pq->dijkstraVertices[source]->parent = -1;

    while (pq->is_not_empty()) {
        pq->create_heap();
        auto vertex = pq->pop_min();
        int u = vertex->index;

        if (visited[u]) {
            continue;
        }

        visited[u] = true;

        auto elem = adjacency_lists[u]->get_head();
        while(elem) {
            int v = elem->value.neighbour;
            int weight = elem->value.weight;

            if (!visited[v] && distance[u] != INT_MAX && distance[u] + weight < distance[v]) {
                distance[v] = distance[u] + weight;
                pq->dijkstraVertices[v]->distance = distance[v];
                pq->dijkstraVertices[v]->parent = u;
                parent[v] = u;
            }
            elem = elem->next;
        }
    }
    delete[] distance;
    delete[] visited;
    delete[] parent;
    delete pq;
}

void adjacency_list::bellman_ford(int source, int end) {
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

    distance[source] = 0;

    int edge = 0;
    kruskal_edge** edges = new kruskal_edge * [numEdges];
    for (int u = 0; u < numVertices; u++) {
        auto elem = adjacency_lists[u]->get_head();
        while(elem) {
            int v = elem->value.neighbour;
            int weight = elem->value.weight;
            edges[edge] = new kruskal_edge(u,v, weight);
            edge++;
            elem = elem->next;
        }
    }

    int u, v, w;
    for (int i = 1; i <= numVertices - 1; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            u = edges[j]->u;
            v = edges[j]->v;
            w = edges[j]->weight;

            if (distance[u] != INT_MAX && distance[u] + w < distance[v]) {
                distance[v] = distance[u] + w;
                parent[v] = u;
            }

        }
    }
    delete[] edges;
    delete[] distance;
    delete[] parent;
}

void adjacency_list::print_dijkstra(int source, int end) {
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

    int* distance = new int[numVertices];
    for (int i = 0; i < numVertices; ++i) {
        distance[i] =INT_MAX;
    }
    bool* visited = new bool[numVertices];
    for (int i = 0; i < numVertices; ++i) {
        visited[i] = false;
    }


    DijkstraHeap* pq = new DijkstraHeap(numVertices);

    distance[source] = 0;
    pq->dijkstraVertices[source]->distance = distance[source];
    pq->dijkstraVertices[source]->parent = -1;

    while (pq->is_not_empty()) {
        pq->create_heap();
        auto vertex = pq->pop_min();
        int u = vertex->index;

        if (visited[u]) {
            continue;
        }

        visited[u] = true;

        auto elem = adjacency_lists[u]->get_head();
        while(elem) {
            int v = elem->value.neighbour;
            int weight = elem->value.weight;

            if (!visited[v] && distance[u] != INT_MAX && distance[u] + weight < distance[v]) {
                distance[v] = distance[u] + weight;
                pq->dijkstraVertices[v]->distance = distance[v];
                pq->dijkstraVertices[v]->parent = u;
                parent[v] = u;
            }
            elem = elem->next;
        }
    }

    std::cout << "Shortest path from source to vertex " << end << ":" << std::endl;
    if (distance[end] == INT_MAX) {
        std::cout << "No path exists." <<std::endl;
    } else {
        std::cout << "Distance: " << distance[end] << std::endl;
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
        delete path;
    }
    delete[] distance;
    delete[] parent;
    delete[] visited;
}

void adjacency_list::printPrim() {
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
    pq->primVertices[0]->key = 0;
    key[0] = 0;

    while (pq->is_not_empty()) {
        pq->create_heap();
        auto elem = pq->pop();
        int u = elem->vertex;

        mstSet[u] = true;
        auto list = adjacency_lists[u]->get_head();
        while (list) {
            int v = list->value.neighbour;
            int weight = list->value.weight;
            if (!mstSet[v] && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;
                pq->primVertices[pq->position[v]]->key = weight;
            }
            list = list->next;
        }
    }
    std::cout << "Minimum Spanning Tree:\n";
    std::cout << "Edge\tWeight\n";
    for (int i = 1; i < numVertices; i++) {
        int u = parent[i];
        auto elem = adjacency_lists[u]->get_head();
        while(elem) {
            if (elem->value.neighbour == i) {
                std::cout << u << " - " << i << "\t" << elem->value.weight << std::endl;
                break;
            }
            elem = elem->next;
        }
    }
    delete[] mstSet;
    delete[] key;
    delete[] parent;
    delete pq;
}

void adjacency_list::printKruskal() {
    if (numVertices == 0){
        std::cout << "Graf jest pusty !" << std::endl;
        return;
    } else if(numVertices == 1){
        std::cout << "Minimum Spanning Tree:" << std::endl;
        return;
    }

    int edge = 0;
    kruskal_edge** edges = new kruskal_edge * [numEdges*2];
    for (int u = 0; u < numVertices; u++) {
        auto elem = adjacency_lists[u]->get_head();
        while(elem) {
            int v = elem->value.neighbour;
            int weight = elem->value.weight;
            edges[edge] = new kruskal_edge(v, u, weight);
            edge++;
            elem = elem->next;
        }
    }

    Sort::quickSortEdges(edges,0,(numEdges*2)-1);

    Subset** subsets = new Subset * [numVertices];

    for (int v = 0; v <numVertices; v++) {
        subsets[v] = new Subset();
        subsets[v]->parent = v;
        subsets[v]->rank = 0;
    }

    kruskal_edge** mst = new kruskal_edge * [numVertices-1];
    int edgeCount = 0;

    for (int i = 0; i< numEdges*2;i++) {
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
            break;
        }
    }
    std::cout << "Minimum Spanning Tree:\n";
    std::cout << "Edge\tWeight\n";
    for (int i = 0; i<numVertices-1; i++) {
        std::cout << mst[i]->u << " - " << mst[i]->v << " \t " << mst[i]->weight << "\n";
    }
    delete [] subsets;
    delete[] mst;
    delete[] edges;
}

void adjacency_list::unionSets(Subset **subsets, int x, int y) {
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

int adjacency_list::findSet(Subset **subsets, int i) {
    if (subsets[i]->parent != i) {
        subsets[i]->parent = findSet(subsets, subsets[i]->parent);
    }
    return subsets[i]->parent;
}

bool adjacency_list::compareEdges(const kruskal_edge &a, const kruskal_edge &b) {
    return a.weight < b.weight;
}

void adjacency_list::print_bellman_ford(int source, int end) {
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

    distance[source] = 0;

    int edge = 0;
    kruskal_edge** edges = new kruskal_edge * [numEdges];
    for (int u = 0; u < numVertices; u++) {
        auto elem = adjacency_lists[u]->get_head();
        while(elem) {
            int v = elem->value.neighbour;
            int weight = elem->value.weight;
            edges[edge] = new kruskal_edge(u,v, weight);
            edge++;
            elem = elem->next;
        }
    }

    int u, v, w;
    for (int i = 1; i <= numVertices - 1; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            u = edges[j]->u;
            v = edges[j]->v;
            w = edges[j]->weight;

            if (distance[u] != INT_MAX && distance[u] + w < distance[v]) {
                distance[v] = distance[u] + w;
                parent[v] = u;
            }

        }
    }

    std::cout << "Shortest path from source to vertex " << end << ":" << std::endl;
    if (distance[end] == INT_MAX) {
        std::cout << "No path exists." <<std::endl;
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

void adjacency_list::generate_graph(int vertices) {
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

int adjacency_list::getNumEdges() const {
    return numEdges;
}

void adjacency_list::setNumEdges(int numEdges) {
    adjacency_list::numEdges = numEdges;
}

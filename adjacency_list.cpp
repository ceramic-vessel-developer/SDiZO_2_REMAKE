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


    std::vector<int> parent(numVertices, -1); // Array to store constructed MST
    std::vector<int> key(numVertices, INT_MAX); // Key values used to pick the minimum weight edge
    std::vector<bool> mstSet(numVertices, false); // To represent set of vertices included in MST



    std::priority_queue<primVertex, std::vector<primVertex>, std::greater<primVertex>> pq; // Priority queue to store vertices and their weights

    // Start with vertex 0
    pq.push(primVertex(0, 0)); // Vertex 0 has key 0
    key[0] = 0;

    // Loop until all vertices are included in MST
    while (!pq.empty()) {
        int u = pq.top().vertex; // Extract the vertex with minimum key value
        pq.pop();

        mstSet[u] = true; // Add the extracted vertex to the MST set
        auto list = adjacency_lists[u]->get_head();
        // Update key value and parent index of the adjacent vertices of the extracted vertex
        while (list) {
            int v = list->value.neighbour;
            int weight = list->value.weight;
            if (!mstSet[v] && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;
                pq.push(primVertex(key[v], v));
            }
            list = list->next;
        }
    }
}

void adjacency_list::kruskal() {
    if (numVertices == 0){
        return;
    } else if(numVertices == 1){
        return;
    }


    std::vector<kruskal_edge> edges;
    for (int u = 0; u < numVertices; u++) {
        auto elem = adjacency_lists[u]->get_head();
        while(elem) {
            int v = elem->value.neighbour;
            int weight = elem->value.weight;
            edges.push_back(kruskal_edge(u, v, weight));
            elem = elem->next;
        }
    }

    // Sort the edges in non-decreasing order of weights
    std::sort(edges.begin(), edges.end(), compareEdges);

    std::vector<Subset> subsets(numVertices); // Array to store subsets for union-find

    for (int v = 0; v <numVertices; v++) {
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

    std::vector<int> distance(numVertices, INT_MAX); // Array to store the shortest distances from the source
    std::vector<bool> visited(numVertices, false); // Array to track visited vertices
    std::vector<int> parent(numVertices, -1);


    std::priority_queue<dijkstraVertex, std::vector<dijkstraVertex>, std::greater<dijkstraVertex>> pq; // Priority queue to store vertices and their distances

    distance[source] = 0; // Distance from source to itself is 0
    pq.push(dijkstraVertex(source, distance[source], -1)); // Push the source vertex into the priority queue

    while (!pq.empty()) {
        int u = pq.top().index; // Extract the vertex with minimum distance

        pq.pop();

        if (visited[u]) {
            continue; // Skip if the vertex is already visited
        }

        visited[u] = true; // Mark the vertex as visited

        auto elem = adjacency_lists[u]->get_head();
        while(elem) {
            int v = elem->value.neighbour;
            int weight = elem->value.weight;

            if (!visited[v] && distance[u] != INT_MAX && distance[u] + weight < distance[v]) {
                distance[v] = distance[u] + weight; // Update the distance
                pq.push(dijkstraVertex(v, distance[v], u));
                parent[v] = u;// Push the updated distance into the priority queue
            }
            elem = elem->next;
        }
    }

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

    std::vector<int> distance(numVertices, INT_MAX);
    std::vector<int> parent(numVertices, -1);

    // Initialize distance array
    distance[source] = 0;

    // Relax edges repeatedly |V|-1 times
    for (int i = 1; i <= numVertices - 1; ++i) {
        for (int u = 0; u < numVertices; ++u) {
            auto elem = adjacency_lists[u]->get_head();
            while(elem) {
                int v = elem->value.neighbour;
                int w = elem->value.weight;

                if (distance[u] != INT_MAX && distance[u] + w < distance[v]) {
                    distance[v] = distance[u] + w;
                    parent[v] = u;
                }
                elem = elem->next;
            }
        }
    }

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

    std::vector<int> distance(numVertices, INT_MAX); // Array to store the shortest distances from the source
    std::vector<bool> visited(numVertices, false); // Array to track visited vertices
    std::vector<int> parent(numVertices, -1);


    std::priority_queue<dijkstraVertex, std::vector<dijkstraVertex>, std::greater<dijkstraVertex>> pq; // Priority queue to store vertices and their distances

    distance[source] = 0; // Distance from source to itself is 0
    pq.push(dijkstraVertex(source, distance[source], -1)); // Push the source vertex into the priority queue

    while (!pq.empty()) {
        int u = pq.top().index; // Extract the vertex with minimum distance

        pq.pop();

        if (visited[u]) {
            continue; // Skip if the vertex is already visited
        }

        visited[u] = true; // Mark the vertex as visited

        auto elem = adjacency_lists[u]->get_head();
        while(elem) {
            int v = elem->value.neighbour;
            int weight = elem->value.weight;

            if (!visited[v] && distance[u] != INT_MAX && distance[u] + weight < distance[v]) {
                distance[v] = distance[u] + weight; // Update the distance
                pq.push(dijkstraVertex(v, distance[v], u));
                parent[v] = u;// Push the updated distance into the priority queue
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

void adjacency_list::printPrim() {
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
    pq.push(primVertex(0, 0)); // Vertex 0 has key 0
    key[0] = 0;

    // Loop until all vertices are included in MST
    while (!pq.empty()) {
        int u = pq.top().vertex; // Extract the vertex with minimum key value
        pq.pop();

        mstSet[u] = true; // Add the extracted vertex to the MST set
        auto list = adjacency_lists[u]->get_head();
        // Update key value and parent index of the adjacent vertices of the extracted vertex
        while (list) {
            int v = list->value.neighbour;
            int weight = list->value.weight;
            if (!mstSet[v] && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;
                pq.push(primVertex(key[v], v));
            }
            list = list->next;
        }
    }
    std::cout << "Minimum Spanning Tree:\n";
    std::cout << "Edge\tWeight\n";
    for (int i = 1; i < parent.size(); i++) {
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


}

void adjacency_list::printKruskal() {
    if (numVertices == 0){
        std::cout << "Graf jest pusty !" << std::endl;
        return;
    } else if(numVertices == 1){
        std::cout << "Minimum Spanning Tree:" << std::endl;
        return;
    }


    std::vector<kruskal_edge> edges;
    for (int u = 0; u < numVertices; u++) {
        auto elem = adjacency_lists[u]->get_head();
        while(elem) {
            int v = elem->value.neighbour;
            int weight = elem->value.weight;
            edges.push_back(kruskal_edge(u, v, weight));
            elem = elem->next;
        }
    }

    // Sort the edges in non-decreasing order of weights
    std::sort(edges.begin(), edges.end(), compareEdges);

    std::vector<Subset> subsets(numVertices); // Array to store subsets for union-find

    for (int v = 0; v <numVertices; v++) {
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
        std::cout << edge.u << " - " << edge.v << "\t" << edge.weight << "\n";
    }
}

void adjacency_list::unionSets(std::vector<Subset> &subsets, int x, int y) {
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

int adjacency_list::findSet(std::vector<Subset> &subsets, int i) {
    if (subsets[i].parent != i) {
        subsets[i].parent = findSet(subsets, subsets[i].parent);
    }
    return subsets[i].parent;

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

    std::vector<int> distance(numVertices, INT_MAX);
    std::vector<int> parent(numVertices, -1);

    // Initialize distance array
    distance[source] = 0;

    // Relax edges repeatedly |V|-1 times
    for (int i = 1; i <= numVertices - 1; ++i) {
        for (int u = 0; u < numVertices; ++u) {
            auto elem = adjacency_lists[u]->get_head();
            while(elem) {
                int v = elem->value.neighbour;
                int w = elem->value.weight;

                if (distance[u] != INT_MAX && distance[u] + w < distance[v]) {
                    distance[v] = distance[u] + w;
                    parent[v] = u;
                }
                elem = elem->next;
            }
        }
    }

    std::cout << "Shortest path from source to vertex " << end << ":" << std::endl;
    if (distance[end] == INT_MAX) {
        std::cout << "No path exists." <<std::endl;
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

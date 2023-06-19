//
// Created by kacpe on 04.06.2023.
//

#include <iostream>
#include "Menu_helper.h"
#include "incidence_matrix.h"
#include "adjacency_list.h"


void menu_helper::primm() {
    std::cout << "MACIERZ" << std::endl;
    matrix_graph->printPrim();
    std::cout << "LISTA" << std::endl;
    list_graph->printPrim();
}

void menu_helper::kruskal() {
    std::cout << "MACIERZ" << std::endl;
    matrix_graph->printKruskal();
    std::cout << "LISTA" << std::endl;
    list_graph->printKruskal();

}

void menu_helper::dijkstra(int start, int end) {
    std::cout << "MACIERZ" << std::endl;
    matrix_graph->printDijkstra(start,end);
    std::cout << "LISTA" << std::endl;
    list_graph->print_dijkstra(start,end);
}

void menu_helper::bellman_ford(int start, int end) {
    std::cout << "MACIERZ" << std::endl;
    matrix_graph->print_bellman_ford(start, end);
    std::cout << "LISTA" << std::endl;
    list_graph->print_bellman_ford(start, end);
}

menu_helper::menu_helper() {
    matrix_graph = nullptr;
    list_graph = nullptr;
}

void menu_helper::generate_from_file(std::string file, bool directed) {
    if (matrix_graph) delete matrix_graph;
    if (list_graph) delete list_graph;
    this->matrix_graph = new incidence_matrix(file,directed);
    this->list_graph = new adjacency_list(file, directed);
}

void menu_helper::generate_random(int vertices, int density, bool directed) {
    if (vertices<0) vertices = 0;
    if (density<0) density = 0;
    if (density > 100) density = 100;
    if (matrix_graph) delete matrix_graph;
    if (list_graph) delete list_graph;
    this->matrix_graph = new incidence_matrix(vertices,density,directed);
    this->list_graph = new adjacency_list(vertices,density, directed);
    int weight;
    if (matrix_graph->getNumEdges() == 0) return;
    if (matrix_graph->getNumVertices() == 1) return;
    if (directed){
        //Generating matrix for directed graph
        if (matrix_graph->getNumVertices() < vertices) {
            matrix_graph->setNumEdges(vertices);
            list_graph->setNumEdges(vertices);
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
            weight = rand()%(INT_MAX-1)+1;
            matrix_graph->add_directed_edge(vertexStart,verticesArray[vertexEnd],weight);
            list_graph->add_directed_edge(vertexStart,verticesArray[vertexEnd],weight);
            ++edgeIndex;
            vertexStart = verticesArray[vertexEnd];
            verticesArray[vertexEnd] = verticesArray[i - 1];
        }
        delete[] verticesArray;

        //creating a cycle
        weight = rand()%(INT_MAX-1)+1;
        matrix_graph->add_directed_edge(vertexStart,0,weight);
        list_graph->add_directed_edge(vertexStart,0,weight);

        //adding the rest of the graph

        int remaining_edges = matrix_graph->getNumEdges() - vertices;
        int start;
        int end;

        for (int i = 0; i < remaining_edges; ++i) {
            while (true){
                start = rand()%vertices;
                end = rand()%vertices;
                if (matrix_graph->are_connected(start,end) < 0) break;
            }
            weight = rand()%(INT_MAX-1)+1;
            matrix_graph->add_directed_edge(start, end, weight);
            list_graph->add_directed_edge(start, end, weight);

        }

    }
    else{
        //Generating matrix for undirected graph
        if (matrix_graph->getNumEdges() < vertices - 1) {
            matrix_graph->setNumEdges(vertices-1);
            list_graph->setNumEdges(vertices-1);

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
            weight = rand()%(INT_MAX-1)+1;
            matrix_graph->add_undirected_edge(vertexStart,verticesArray[vertexEnd],weight);
            list_graph->add_undirected_edge(vertexStart,verticesArray[vertexEnd],weight);
            ++edgeIndex;
            vertexStart = verticesArray[vertexEnd];
            verticesArray[vertexEnd] = verticesArray[i - 1];
        }
        delete[] verticesArray;
        //adding the rest of the graph

        int remaining_edges = matrix_graph->getNumEdges() - vertices + 1;
        int start;
        int end;

        for (int i = 0; i < remaining_edges; ++i) {
            while (true){
                start = rand()%vertices;
                end = rand()%vertices;
                if (matrix_graph->are_connected(start,end) < 0) break;
            }
            weight = rand()%(INT_MAX-1)+1;
            matrix_graph->add_undirected_edge(start, end, weight);
            list_graph->add_undirected_edge(start, end, weight);
        }

    }
}

void menu_helper::display_list() {
    list_graph->display_lists();
}

void menu_helper::display_matrix() {
    matrix_graph->displayMatrix();
}

void menu_helper::display_both() {
    std::cout << "MACIERZ" << std::endl;
    matrix_graph->displayMatrix();
    std::cout << "LISTA" << std::endl;
    list_graph->display_lists();
}

menu_helper::~menu_helper() {
    delete matrix_graph;
    delete list_graph;

}

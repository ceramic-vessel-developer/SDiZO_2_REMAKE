//
// Created by kacpe on 04.06.2023.
//

#ifndef SDIZO_2_MENU_HELPER_H
#define SDIZO_2_MENU_HELPER_H


#include "incidence_matrix.h"
#include "adjacency_list.h"
#include <cstdio>

class menu_helper {
private:
    incidence_matrix* matrix_graph = nullptr;
    adjacency_list* list_graph = nullptr;
public:
    menu_helper();
    ~menu_helper();

    void generate_from_file(std::string file, bool directed);
    void generate_random(int vertices, int density, bool directed);

    void primm();
    void kruskal();
    void dijkstra(int start, int end);
    void bellman_ford(int start, int end);

    void display_list();
    void display_matrix();
    void display_both();
};


#endif //SDIZO_2_MENU_HELPER_H

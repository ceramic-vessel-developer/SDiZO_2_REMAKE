//
// Created by kacpe on 18.03.2023.
//

#ifndef SDIZO_1_DOUBLE_LINKED_LIST_H
#define SDIZO_1_DOUBLE_LINKED_LIST_H


#include "edge.h"

class double_linked_list {
public:
    int length = 0;
    struct dll_elem{
        dll_elem();

        edge value;
        dll_elem* previous;
        dll_elem* next;
    };

    double_linked_list();
    ~double_linked_list();

    void clear(dll_elem* elem);

    void add_back(int vertex, int weight);

    dll_elem* get_head();

    edge* select(int value);

    void show();
private:

    dll_elem* first = nullptr;
    dll_elem* last = nullptr;



};


#endif //SDIZO_1_DOUBLE_LINKED_LIST_H

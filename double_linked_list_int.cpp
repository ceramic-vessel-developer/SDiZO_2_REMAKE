//
// Created by kacpe on 18.03.2023.
//

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include "double_linked_list_int.h"

double_linked_list_int::double_linked_list_int(int size) {
    srand((unsigned) time(nullptr));

    for (int i = 0; i < size; ++i) {
        add_back(rand());
    }
}


void double_linked_list_int::add_back(int item) {
    dll_elem* node = new dll_elem();

    node->value = item;
    node->previous = last;
    node->next = nullptr;

    //if there ia no nodes, set new one as first and last
    if (last){
        last->next = node;
    }else{
        first = node;
    }
    last = node;
}

int double_linked_list_int::pop_back() {
    if (last == nullptr) return -1;
    int value = last->value;
    dll_elem* temp = last->previous;

    delete last;

    if (temp == nullptr) {
        last = nullptr;
        first = nullptr;
    } else {
        temp->next = nullptr;
        last = temp;
    }
    return value;
}

double_linked_list_int::~double_linked_list_int() {
    clear(first);
}

void double_linked_list_int::clear(double_linked_list_int::dll_elem *elem) {
    if (elem){
        clear(elem->next);
        delete elem;
    }
}

bool double_linked_list_int::is_not_empty() {
    return first != nullptr && last != nullptr;
}

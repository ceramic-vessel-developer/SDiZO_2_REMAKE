//
// Created by kacpe on 18.03.2023.
//

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include "double_linked_list.h"


void double_linked_list::add_back(int vertex, int weight) {
    dll_elem* node = new dll_elem();

    node->value.neighbour = vertex;
    node->value.weight = weight;
    node->previous = last;
    node->next = nullptr;

    //if there ia no nodes, set new one as first and last
    if (last){
        last->next = node;
    }else{
        first = node;
    }
    last = node;
    length++;
}


edge* double_linked_list::select(int value) {
    dll_elem* temp = first;
    while (temp){
        if (temp->value.neighbour == value){
            return &temp->value;
        }
        temp = temp->next;
    }
    return nullptr;
}

void double_linked_list::show() {
    dll_elem* temp = first;
    while(temp){
        std::cout << temp->value.neighbour << ' ' <<temp->value.weight << " ";
        temp = temp->next;
    }
    std::cout << std::endl;
}

double_linked_list::~double_linked_list() {
    clear(first);
}

void double_linked_list::clear(double_linked_list::dll_elem *elem) {
    if (elem){
        clear(elem->next);
        delete elem;
    }
}

double_linked_list::dll_elem *double_linked_list::get_head() {
    return first;
}

double_linked_list::double_linked_list() = default;

double_linked_list::dll_elem::dll_elem() {

}

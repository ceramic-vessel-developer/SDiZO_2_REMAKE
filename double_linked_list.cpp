//
// Created by kacpe on 18.03.2023.
//

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include "double_linked_list.h"

double_linked_list::double_linked_list(int size) {
    srand((unsigned) time(nullptr));

    for (int i = 0; i < size; ++i) {
        add_back(rand(), rand());
    }
}

void double_linked_list::add_front(int vertex, int weight) {
    dll_elem* node = new dll_elem();

    node->value.neighbour = vertex;
    node->value.weight = weight;
    node->previous = nullptr;
    node->next = first;

    //if there ia no nodes, set new one as first and last
    if (first){
        first->previous = node;
    }else{
        last = node;
    }
    first = node;
    length++;
}

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

void double_linked_list::add(int vertex, int weight, int index) {
    dll_elem* node = new dll_elem();

    dll_elem* current_node = search(index);

    node->value.neighbour = vertex;
    node->value.weight = weight;
    node->next = current_node;

    if (current_node) {

        node->previous = current_node->previous;
        if (!current_node->previous){
            first = node;
            return;
        }

        current_node->previous->next = node;

        current_node->previous = node;

    }else if (index == 0){
        node->previous = nullptr;
        node->next = nullptr;
        first = node;
        last = node;
    } else{
        last->next = node;
        node->previous = last;
        last = node;
    }
    length++;
}

void double_linked_list::delete_front() {
    if(first == nullptr)return;

    dll_elem* temp = first->next;
    delete first;
    if (temp){
        temp->previous = nullptr;
    }
    first = temp;
}

void double_linked_list::delete_back() {
    if (last == nullptr) return;

    dll_elem* temp = last->previous;

    delete last;

    if (temp == nullptr) {
        last = nullptr;
    } else {
        temp->next = nullptr;
        last = temp;
    }

}

void double_linked_list::delete_with_index(int index) {
    if (!last) return;

    dll_elem* temp = search(index);
    if (!temp) return;

    if (temp->next and temp->previous){
            temp->previous->next = temp->next;
            temp->next->previous = temp->previous;
    }else if (temp->next){
        first = temp->next;
        temp->next->previous = nullptr;
    }else if (temp->previous){
        last = temp->previous;
        temp->previous->next = nullptr;
    }else{
        last = nullptr;
        first = nullptr;
    }
    delete temp;
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

double_linked_list::dll_elem *double_linked_list::search(int index) {

    dll_elem* temp;
    temp = first;
    if (temp){
        for (int i = 0; i < index; ++i) {
            if (temp) {
                temp = temp->next;
            }else{
                return nullptr;
            }
        }
    }
    return temp;
}

double_linked_list::double_linked_list(std::string file) {
    std::string path = "../input_files/"+file;
    std::fstream f;
    int input;

    f.open(path);
    if (f.is_open()){
        f >> input;
        while (f >> input){
            add_back(input, rand()%999+1);
        }
        f.close();
    }else{
        std::cout << "Plik nie zostal otwarty poprawnie" << std::endl;
    }
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

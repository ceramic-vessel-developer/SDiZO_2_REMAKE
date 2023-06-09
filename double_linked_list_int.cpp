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

void double_linked_list_int::add_front(int item) {
    dll_elem* node = new dll_elem();

    node->value = item;
    node->previous = nullptr;
    node->next = first;

    //if there ia no nodes, set new one as first and last
    if (first){
        first->previous = node;
    }else{
        last = node;
    }
    first = node;
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

void double_linked_list_int::add(int item, int index) {
    dll_elem* node = new dll_elem();

    dll_elem* current_node = search(index);

    node->value = item;
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
}

int double_linked_list_int::pop_front() {
    if(first == nullptr)return -1;
    int value = first->value;
    dll_elem* temp = first->next;
    delete first;
    if (temp){
        temp->previous = nullptr;
    }
    first = temp;
    return value;
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

void double_linked_list_int::delete_with_index(int index) {
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

int* double_linked_list_int::select(int value) {
    dll_elem* temp = first;
    while (temp){
        if (temp->value == value){
            return &temp->value;
        }
        temp = temp->next;
    }
    return nullptr;
}

void double_linked_list_int::show() {
    dll_elem* temp = first;
    while(temp){
        std::cout << temp->value << " ";
        temp = temp->next;
    }
    std::cout << std::endl;
}

double_linked_list_int::dll_elem *double_linked_list_int::search(int index) {

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

double_linked_list_int::double_linked_list_int(std::string file) {
    std::string path = "../input_files/"+file;
    std::fstream f;
    int input;

    f.open(path);
    if (f.is_open()){
        f >> input;
        while (f >> input){
            add_back(input);
        }
        f.close();
    }else{
        std::cout << "Plik nie zostal otwarty poprawnie" << std::endl;
    }
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

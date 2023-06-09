//
// Created by kacpe on 18.03.2023.
//

#ifndef SDIZO_1_DOUBLE_LINKED_LIST_INT_H
#define SDIZO_1_DOUBLE_LINKED_LIST_INT_H


#include <string>

class double_linked_list_int {
private:
    struct dll_elem{
        int value;
        dll_elem* previous = nullptr;
        dll_elem* next = nullptr;
    };
    dll_elem* first = nullptr;
    dll_elem* last = nullptr;

    dll_elem* search(int index);
public:
    double_linked_list_int(int size);
    double_linked_list_int(std::string file);
    ~double_linked_list_int();

    void clear(dll_elem* elem);

    void add_front(int item);
    void add_back(int item);
    void add(int item, int index);

    int pop_front();
    int pop_back();
    void delete_with_index(int index);

    int* select(int value);

    void show();

    bool is_not_empty();


};


#endif //SDIZO_1_DOUBLE_LINKED_LIST_INT_H

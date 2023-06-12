cmake_minimum_required(VERSION 3.24)
project(SDiZO_2_REMAKE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(SDiZO_2_REMAKE main.cpp incidence_matrix.cpp incidence_matrix.h double_linked_list.cpp edge.cpp edge.h adjacency_list.cpp adjacency_list.h primVertex.cpp primVertex.h Menu_helper.cpp Menu_helper.h Sort.cpp Sort.h DijkstraHeap.cpp DijkstraHeap.h primHeap.cpp primHeap.h double_linked_list_int.h double_linked_list_int.cpp)

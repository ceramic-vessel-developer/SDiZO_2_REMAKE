//
// Created by kacpe on 04.06.2023.
//

#ifndef TEST_DIJKSTRAVERTEX_H
#define TEST_DIJKSTRAVERTEX_H


class dijkstraVertex {
public:
    int index;
    int distance;
    int parent;

    dijkstraVertex(int i, int d,int p) : index(i), distance(d), parent(p) {}
    bool operator>(const dijkstraVertex& other) const {
        return distance > other.distance;
    }
};


#endif //TEST_DIJKSTRAVERTEX_H

//
// Created by kacpe on 02.06.2023.
//

#ifndef SDIZO_2_PRIMVERTEX_H
#define SDIZO_2_PRIMVERTEX_H


class primVertex {
public:
    int key;
    int vertex;
    primVertex(int k, int v) : key(k), vertex(v) {}

    bool operator>(const primVertex& other) const {
        return key > other.key;
    }


};


#endif //SDIZO_2_PRIMVERTEX_H

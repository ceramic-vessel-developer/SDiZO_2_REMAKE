#include <iostream>
#include <windows.h>
#include <fstream>
#include "incidence_matrix.h"
#include "adjacency_list.h"
#include "Menu_helper.h"


void test();
void research();
void MST();
void path();
void prim();
void kruskal();
void dijkstra();
void bellman();

int main() {
    int choice;
    bool error = true;
    while (error) {
        std::cout << "Wybierz funkcjonalnosc programu:\n1)Badania\n2)Testowanie funkcjonalnosci" << std::endl;

        std::cin >> choice;
        std::cout << std::endl;

        switch (choice) {
            case 1:
                research();
                error = false;
                break;
            case 2:
                test();
                error = false;
                break;
            default:
                std::cout << "Bledna wartosc" << std::endl;
                break;

        }
    }
    return 0;
}

void test(){
    int choice;
    while (true) {
        std::cout << "Wybierz problem:\n1)MST\n2)Najkrotsza droga\n3)Wyjscie" << std::endl;

        std::cin >> choice;
        std::cout << std::endl;

        switch (choice) {
            case 1:
                MST();
                break;
            case 2:
                path();
                break;
            case 3:
                return;
            default:
                std::cout << "Bledna wartosc" << std::endl;
                break;

        }
    }
}

void MST(){
    int choice;
    std::string input;
    int input_n1,input_n2;
    menu_helper* menu = nullptr;
    while (true) {
        std::cout << "1)Wprowadz plik z grafem\n2)Wygeneruj graf losowo\n3)Algortym Primma\n4)Algorytm Kruskala\n 5)Wyswietl graf\n6)Wyjscie" << std::endl;

        std::cin >> choice;
        std::cout << std::endl;

        switch (choice) {
            case 1:
                if (!menu){
                    menu = new menu_helper();
                }
                std::cout << "Wprowadz nazwe pliku" << std::endl;
                std::cin >> input;
                menu->generate_from_file(input, false);
                break;
            case 2:
                if (!menu){
                    menu = new menu_helper();
                }
                std::cout << "Wprowadz liczbe wierzcholkow" << std::endl;
                std::cin >> input_n1;
                std::cout << "Wprowadz gestosc (liczba procentow)" << std::endl;
                std::cin >> input_n2;
                menu->generate_random(input_n1,input_n2, false);
                break;
            case 3:
                if (!menu){
                    std::cout << "Nie wprowadzono grafu!" << std::endl;
                    break;
                }
                menu->primm();
                break;
            case 4:
                if (!menu){
                    std::cout << "Nie wprowadzono grafu!" << std::endl;
                    break;
                }
                menu->kruskal();
                break;
            case 5:
                if (!menu){
                    std::cout << "Nie wprowadzono grafu!" << std::endl;
                    break;
                }
                menu->display_both();
                break;
            case 6:
                delete menu;
                return;
            default:
                std::cout << "Bledna wartosc" << std::endl;
                break;

        }
    }

}

void path(){
    int choice;

    std::string input;
    int input_n1,input_n2;
    menu_helper* menu = nullptr;
    while (true) {
        std::cout << "1)Wprowadz plik z grafem\n2)Wygeneruj graf losowo\n3)Algortym Dijkstry\n4)Algorytm Bellmana-Forda\n 5)Wyswietl graf\n6)Wyjscie" << std::endl;

        std::cin >> choice;
        std::cout << std::endl;

        switch (choice) {
            case 1:
                if (!menu){
                    menu = new menu_helper();
                }
                std::cout << "Wprowadz nazwe pliku" << std::endl;
                std::cin >> input;
                menu->generate_from_file(input, true);
                break;
            case 2:
                if (!menu){
                    menu = new menu_helper();
                }
                std::cout << "Wprowadz liczbe wierzcholkow" << std::endl;
                std::cin >> input_n1;
                std::cout << "Wprowadz gestosc (liczba procentow)" << std::endl;
                std::cin >> input_n2;
                menu->generate_random(input_n1,input_n2, true);
                break;
            case 3:
                if (!menu){
                    std::cout << "Nie wprowadzono grafu!" << std::endl;
                    break;
                }
                std::cout << "Wprowadz wierzhcolek poczatkowy" << std::endl;
                std::cin >> input_n1;
                std::cout << "Wprowadz wierzcholek koncowy" << std::endl;
                std::cin >> input_n2;
                menu->dijkstra(input_n1,input_n2);
                break;
            case 4:
                if (!menu){
                    std::cout << "Nie wprowadzono grafu!" << std::endl;
                    break;
                }
                std::cout << "Wprowadz wierzhcolek poczatkowy" << std::endl;
                std::cin >> input_n1;
                std::cout << "Wprowadz wierzcholek koncowy" << std::endl;
                std::cin >> input_n2;
                menu->bellman_ford(input_n1,input_n2);
                break;

            case 5:
                if (!menu){
                    std::cout << "Nie wprowadzono grafu!" << std::endl;
                    break;
                }
                menu->display_both();
                break;
            case 6:
                delete menu;
                return;
            default:
                std::cout << "Bledna wartosc" << std::endl;
                break;

        }
    }


}

void research(){
    prim();
    kruskal();
    dijkstra();
    bellman();

}

void prim(){
    const int number_of_tests = 100;
    int sizes []= {20,40,60,80,100};
    int densities[] = {25,50,75,99};
    int size,density;
    long double times[] = {0,0};
    long double elapsed_time_double;
    LARGE_INTEGER start_time, end_time, elapsed_time, frequency;
    std::fstream f;
    std::string path = "../txt_files/prim.txt";
    adjacency_list* list;
    incidence_matrix* matrix;

    QueryPerformanceFrequency(&frequency);
    f.open(path,std::ios::out);
    f.close();

    for (int i = 0; i < 5; ++i) {
        size = sizes[i];
        for (int j = 0; j < 4; ++j) {
            density = densities[j];
            for (int k = 0; k < number_of_tests; ++k) {
                std::cout<< "PRIM LIST " << i << ' '<< j <<' '<< k << std::endl;
                list = new adjacency_list(size,density,false);
                list->generate_graph(size);
                QueryPerformanceCounter(&start_time);
                list->prim();
                QueryPerformanceCounter(&end_time);

                elapsed_time.QuadPart = end_time.QuadPart - start_time.QuadPart;
                elapsed_time_double = static_cast<long double>(elapsed_time.QuadPart);
                elapsed_time_double *= 1000000; // result in nanoseconds
                elapsed_time_double /= frequency.QuadPart;
                delete list;
                times[0] += elapsed_time_double;
            }
            times[0] = times[0]/number_of_tests;
            for (int k = 0; k < number_of_tests; ++k) {
                std::cout<< "PRIM MATRIX " << i << ' '<< j <<' '<< k << std::endl;
                matrix = new incidence_matrix(size,density,false);
                matrix->generate_matrix(size);

                QueryPerformanceCounter(&start_time);
                matrix->prim();
                QueryPerformanceCounter(&end_time);

                elapsed_time.QuadPart = end_time.QuadPart - start_time.QuadPart;
                elapsed_time_double = static_cast<long double>(elapsed_time.QuadPart);
                elapsed_time_double *= 1000000; // result in nanoseconds
                elapsed_time_double /= frequency.QuadPart;
                delete matrix;
                times[1] += elapsed_time_double;
            }
            times[1] = times[1]/number_of_tests;
            f.open(path,std::ios::app);
            f << size << ' ' << density<< ' ' << times[0] << ' ' << times[1] << std::endl;
            f.close();
        }
    }
}

void kruskal(){
    const int number_of_tests = 100;
    int sizes []= {20,40,60,80,100};
    int densities[] = {25,50,75,99};
    int size,density;
    long double times[] = {0,0};
    long double elapsed_time_double;
    LARGE_INTEGER start_time, end_time, elapsed_time, frequency;
    std::fstream f;
    std::string path = "../txt_files/kruskal.txt";
    adjacency_list* list;
    incidence_matrix* matrix;

    QueryPerformanceFrequency(&frequency);
    f.open(path,std::ios::out);
    f.close();

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 4; ++j) {
            size = sizes[i];
            density = densities[j];
            for (int k = 0; k < number_of_tests; ++k) {
                std::cout<< "KRUSKAL LIST " << i << ' '<< j <<' '<< k << std::endl;
                list = new adjacency_list(size,density,false);
                list->generate_graph(size);

                QueryPerformanceCounter(&start_time);
                list->kruskal();
                QueryPerformanceCounter(&end_time);

                elapsed_time.QuadPart = end_time.QuadPart - start_time.QuadPart;
                elapsed_time_double = static_cast<long double>(elapsed_time.QuadPart);
                elapsed_time_double *= 1000000; // result in microseconds
                elapsed_time_double /= frequency.QuadPart;
                delete list;
                times[0] += elapsed_time_double;
            }
            times[0] = times[0]/number_of_tests;
            for (int k = 0; k < number_of_tests; ++k) {
                matrix = new incidence_matrix(size,density,false);
                std::cout<< "KRUSKAL MATRIX " << i << ' '<< j <<' '<< k << std::endl;
                matrix->generate_matrix(size);

                QueryPerformanceCounter(&start_time);
                matrix->kruskal();
                QueryPerformanceCounter(&end_time);

                elapsed_time.QuadPart = end_time.QuadPart - start_time.QuadPart;
                elapsed_time_double = static_cast<long double>(elapsed_time.QuadPart);
                elapsed_time_double *= 1000000; // result in microseconds
                elapsed_time_double /= frequency.QuadPart;
                delete matrix;
                times[1] += elapsed_time_double;
            }
            times[1] = times[1]/number_of_tests;
            f.open(path,std::ios::app);
            f << size << ' ' << density<< ' ' << times[0] << ' ' << times[1] << std::endl;
            f.close();
        }
    }

}

void dijkstra(){
    const int number_of_tests = 100;
    int sizes []= {20,40,60,80,100};
    int densities[] = {25,50,75,99};
    int size,density, start, end;
    long double times[] = {0,0};
    long double elapsed_time_double;
    LARGE_INTEGER start_time, end_time, elapsed_time, frequency;
    std::fstream f;
    std::string path = "../txt_files/dijkstra.txt";
    adjacency_list* list;
    incidence_matrix* matrix;

    QueryPerformanceFrequency(&frequency);
    f.open(path,std::ios::out);
    f.close();

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 4; ++j) {
            size = sizes[i];
            density = densities[j];
            for (int k = 0; k < number_of_tests; ++k) {
                list = new adjacency_list(size,density,true);
                std::cout<< "Disjkstra LIST " << i << ' '<< j <<' '<< k << std::endl;
                list->generate_graph(size);
                start = rand()%size;
                end = rand()%size;

                QueryPerformanceCounter(&start_time);
                list->dijkstra(start,end);
                QueryPerformanceCounter(&end_time);

                elapsed_time.QuadPart = end_time.QuadPart - start_time.QuadPart;
                elapsed_time_double = static_cast<long double>(elapsed_time.QuadPart);
                elapsed_time_double *= 1000000; // result in microseconds
                elapsed_time_double /= frequency.QuadPart;
                delete list;
                times[0] += elapsed_time_double;
            }
            times[0] = times[0]/number_of_tests;
            for (int k = 0; k < number_of_tests; ++k) {
                matrix = new incidence_matrix(size,density,true);
                matrix->generate_matrix(size);
                std::cout<< "Disjkstra MATRIX " << i << ' '<< j <<' '<< k << std::endl;
                start = rand()%size;
                end = rand()%size;

                QueryPerformanceCounter(&start_time);
                matrix->dijkstra(start,end);
                QueryPerformanceCounter(&end_time);

                elapsed_time.QuadPart = end_time.QuadPart - start_time.QuadPart;
                elapsed_time_double = static_cast<long double>(elapsed_time.QuadPart);
                elapsed_time_double *= 1000000; // result in microseconds
                elapsed_time_double /= frequency.QuadPart;
                delete matrix;
                times[1] += elapsed_time_double;
            }
            times[1] = times[1]/number_of_tests;
            f.open(path,std::ios::app);
            f << size << ' ' << density<< ' ' << times[0] << ' ' << times[1] << std::endl;
            f.close();
        }
    }


}

void bellman(){
    const int number_of_tests = 100;
    int sizes []= {20,40,60,80,100};
    int densities[] = {25,50,75,99};
    int size,density, start, end;
    long double times[] = {0,0};
    long double elapsed_time_double;
    LARGE_INTEGER start_time, end_time, elapsed_time, frequency;
    std::fstream f;
    std::string path = "../txt_files/bellman.txt";
    adjacency_list* list;
    incidence_matrix* matrix;

    QueryPerformanceFrequency(&frequency);
    f.open(path,std::ios::out);
    f.close();

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 4; ++j) {
            size = sizes[i];
            density = densities[j];
            for (int k = 0; k < number_of_tests; ++k) {
                list = new adjacency_list(size,density,true);
                std::cout<< "Bellman LIST " << i << ' '<< j <<' '<< k << std::endl;
                list->generate_graph(size);
                start = rand()%size;
                end = rand()%size;

                QueryPerformanceCounter(&start_time);
                list->bellman_ford(start,end);
                QueryPerformanceCounter(&end_time);

                elapsed_time.QuadPart = end_time.QuadPart - start_time.QuadPart;
                elapsed_time_double = static_cast<long double>(elapsed_time.QuadPart);
                elapsed_time_double *= 1000000; // result in microseconds
                elapsed_time_double /= frequency.QuadPart;
                delete list;
                times[0] += elapsed_time_double;
            }
            times[0] = times[0]/number_of_tests;
            for (int k = 0; k < number_of_tests; ++k) {
                matrix = new incidence_matrix(size,density, true);
                matrix->generate_matrix(size);
                std::cout<< "Bellman MATRIX " << i << ' '<< j <<' '<< k << std::endl;

                start = rand()%size;
                end = rand()%size;

                QueryPerformanceCounter(&start_time);
                matrix->bellman_ford(start,end);
                QueryPerformanceCounter(&end_time);

                elapsed_time.QuadPart = end_time.QuadPart - start_time.QuadPart;
                elapsed_time_double = static_cast<long double>(elapsed_time.QuadPart);
                elapsed_time_double *= 1000000; // result in microseconds
                elapsed_time_double /= frequency.QuadPart;
                delete matrix;
                times[1] += elapsed_time_double;
            }
            times[1] = times[1]/number_of_tests;
            f.open(path,std::ios::app);
            f << size << ' ' << density<< ' ' << times[0] << ' ' << times[1] << std::endl;
            f.close();
        }
    }



}



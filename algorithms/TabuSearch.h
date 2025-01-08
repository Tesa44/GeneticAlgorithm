//
// Created by Mateusz on 25.11.2024.
//

#ifndef TABUSEARCH_H
#define TABUSEARCH_H
#include <limits>
#include <string>


class TabuSearch {

private:
    int** distanceMatrix;   // Macierz odległości
    int** tabuList;         // Lista tabu (alokacja dynamiczna)
    int numCities = 0;          // Liczba miast
    int tabuTenure = 15;         // Czas trwania zakazu na liście tabu 55 (5-15) ; 170 (10-25) ; 358 (15-35)
    int maxNoImprove = 50;       // Maksymalna liczba iteracji bez poprawy 55 (30-50) ; 170 (50-100) ; 358 ( 100 - 200)
    double timeLimit = 10.0;       // Limit czasu (w sekundach)
    double bestFindTime = 0.0;

    int (TabuSearch::*neighborhoodFunc)(int*, int*) = &swapNeighborhood; // Wskaźnik na funkcję definiującą sąsiedztwo
    int calculateCost(int* route);
    int swapNeighborhood(int* currentRoute, int* bestNeighbor);
    int insertNeighborhood(int* currentRoute, int* bestNeighbor);
    int kSwapNeighborhood(int* currentRoute, int* bestNeighbor);
    void perturbRoute(int* route);
public:
    ~TabuSearch()
    {
        if (distanceMatrix) {
            for (int i = 0; i < numCities; ++i) delete[] distanceMatrix[i];
            delete[] distanceMatrix;
        }
        if (tabuList) {
            for (int i = 0; i < numCities; ++i) delete[] tabuList[i];
            delete[] tabuList;
        }
    }
    //funkcja glowna
    int* solve();
    //Settery i gettery
    void setNumCities(int newNumCities);
    void setNeighborhood(int choice);
    int getNumCities();
    void setDistanceMatrix(int** newMatrix);
    int** getDistanceMatrix();
    void setTenure(int newTenure);
    void setTimeLimit(double newTimeLimit);
};



#endif //TABUSEARCH_H

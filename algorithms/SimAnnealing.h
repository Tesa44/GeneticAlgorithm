//
// Created by Mateusz on 28.11.2024.
//

#ifndef SIMANNEALING_H
#define SIMANNEALING_H
#include <random>
#include <string>


class SimAnnealing {
private:
    int numCities = -1;            // Liczba miast
    int** distanceMatrix;  // Macierz odległości
    int* currentRoute;        // Obecna trasa
    int* bestRoute;           // Najlepsza trasa
    double temperature = 0;       // Bieżąca temperatura
    double coolingRate = 0.95;       // Współczynnik schładzania
    int coolingScheme = 1;        // Typ schładzania
    double timeLimit = 60.0;         // Limit czasu
    int maxIter = 100;         //Maksymalna liczba iteracji bez dywersyfikacji
    int currentCost = 0;          // Koszt obecnej trasy
    double bestFindTime = 0.0;  //Czas znalezienia najlepszej trasy

    void swapElements(int* neighbor, int numCities, int i, int j);
    void getRandomNeighbor(int* currentRoute,int* neighborRoute, std::mt19937& gen);
    void calculateInitialTemperature();
    int calculateCost(int* route);
public:
    ~SimAnnealing() {
        if (distanceMatrix) {
            for (int i = 0; i < numCities; ++i) {
                delete[] distanceMatrix[i];
            }
            delete[] distanceMatrix;
        }
        delete[] currentRoute;
        delete[] bestRoute;
    }
    //Funkcja glowna
    int* solve();
    //Settery i gettery
    void setCoolingScheme(int scheme);
    void setTimeLimit(double newTimeLimit);
    void setCoolingRate(double newCoolingRate);
    void setNumCities(int newNumCities);
    void setDistanceMatrix(int** newDistanceMatrix);
};




#endif //SIMANNEALING_H

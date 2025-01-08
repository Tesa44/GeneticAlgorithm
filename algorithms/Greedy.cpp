//
// Created by mateu on 15.12.2024.
//

#include "Greedy.h"

#include <algorithm>
#include <climits>

int* Greedy::solve(int** distanceMatrix, int numCities) {
    int* bestRoute = new int[numCities+1];
    int bestCost = INT_MAX;

    for (int startCity = 0; startCity < numCities; ++startCity) {
        // Tymczasowa tablica na aktualną trasy
        int* tempRoute = new int[numCities+1];
        bool* visited = new bool[numCities]{false};

        // Inicjalizacja ścieżki
        tempRoute[0] = startCity;
        visited[startCity] = true;

        // Budowanie trasy zachłannej
        for (int i = 1; i < numCities; ++i) {
            int lastCity = tempRoute[i - 1];
            int nearestCity = -1;
            int minDistance = INT_MAX;

            for (int j = 0; j < numCities; ++j) {
                if (!visited[j] && distanceMatrix[lastCity][j] < minDistance) {
                    minDistance = distanceMatrix[lastCity][j];
                    nearestCity = j;
                }
            }

            tempRoute[i] = nearestCity;
            visited[nearestCity] = true;
        }
        //Droga powrotna
        tempRoute[numCities] = startCity;
        // Obliczenie kosztu trasy
        int tempCost = calculateCost(tempRoute, distanceMatrix, numCities);

        // Aktualizacja najlepszego rozwiązania
        if (tempCost < bestCost) {
            bestCost = tempCost;
            std::copy(tempRoute, tempRoute + numCities+1, bestRoute);
        }

        delete[] tempRoute;
        delete[] visited;
    }

    //saveResultToFile("wynik.txt", bestRoute, numCities);
    //std::cout << calculateCostFromFile("wynik.txt",distanceMatrix, numCities) << std::endl;
    //Przypisanie najlepszego rozwiązania do finalnej trasy
    //std::copy(bestRoute, bestRoute + numCities, route);
    //delete[] bestRoute;
    return bestRoute;
}


int Greedy::calculateCost(int* route, int** distanceMatrix, int numCities) {
    int cost = 0;
    for (int i = 0; i < numCities - 1; ++i) {
        cost += distanceMatrix[route[i]][route[i + 1]];
    }
    cost += distanceMatrix[route[numCities - 1]][route[0]];
    return cost;
}

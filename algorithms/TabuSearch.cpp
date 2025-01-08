//
// Created by Mateusz on 25.11.2024.
//

#include "TabuSearch.h"

#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <climits>
#include <chrono>

#include "Greedy.h"

using namespace std;

int TabuSearch::calculateCost(int* route) {
        int cost = 0;
        for (int i = 0; i < numCities - 1; ++i) {
            cost += distanceMatrix[route[i]][route[i + 1]];
        }
        cost += distanceMatrix[route[numCities - 1]][route[0]];
        return cost;
}

int TabuSearch::swapNeighborhood(int* currentRoute, int* bestNeighbor) {
    int bestCost = INT_MAX;
    int* tempRoute = new int[numCities];

    for (int i = 1; i < numCities - 1; ++i) {
        for (int j = i + 1; j < numCities; ++j) {
            std::copy(currentRoute, currentRoute + numCities, tempRoute);
            std::swap(tempRoute[i], tempRoute[j]); // Zamiana miast

            int cost = calculateCost(tempRoute);

            if ((tabuList[i][j] == 0 || cost < bestCost) && cost < bestCost) {
                bestCost = cost;
                std::copy(tempRoute, tempRoute + numCities, bestNeighbor);
            }
        }
    }

    delete[] tempRoute;
    return bestCost;
}

int TabuSearch::insertNeighborhood(int* currentRoute, int* bestNeighbor) {
    int bestCost = INT_MAX;
    int* tempRoute = new int[numCities];

    for (int i = 1; i < numCities; ++i) {
        for (int j = 0; j < numCities; ++j) {
            if (i == j) continue;

            std::copy(currentRoute, currentRoute + numCities, tempRoute);

            int temp = tempRoute[i];
            if (i < j) {
                for (int k = i; k < j; ++k) tempRoute[k] = tempRoute[k + 1];
            } else {
                for (int k = i; k > j; --k) tempRoute[k] = tempRoute[k - 1];
            }
            tempRoute[j] = temp;

            int cost = calculateCost(tempRoute);

            if ((tabuList[i][j] == 0 || cost < bestCost) && cost < bestCost) {
                bestCost = cost;
                std::copy(tempRoute, tempRoute + numCities, bestNeighbor);
            }
        }
    }

    delete[] tempRoute;
    return bestCost;
}

int TabuSearch::kSwapNeighborhood(int* currentRoute, int* bestNeighbor)
{
    int bestCost = INT_MAX;
    int* tempRoute = new int[numCities];

    for (int i = 1; i < numCities - 2; ++i) {
        for (int j = i + 1; j < numCities - 1; ++j) {
            for (int k = j + 1; k < numCities; ++k) {
                std::copy(currentRoute, currentRoute + numCities, tempRoute);

                // Przeprowadź zamianę pozycji trzech miast
                std::swap(tempRoute[i], tempRoute[j]);
                std::swap(tempRoute[j], tempRoute[k]);

                int cost = calculateCost(tempRoute);

                if ((tabuList[i][j] == 0 || cost < bestCost) && cost < bestCost) {
                    bestCost = cost;
                    std::copy(tempRoute, tempRoute + numCities, bestNeighbor);
                }
            }
        }
    }
    delete[] tempRoute;
    return bestCost;
}

void TabuSearch::setNeighborhood(int choice) {
    switch (choice) {
    case 1:
        neighborhoodFunc = &TabuSearch::swapNeighborhood;
        break;
    case 2:
        neighborhoodFunc = &TabuSearch::insertNeighborhood;
        break;
    case 3:
        neighborhoodFunc = &TabuSearch::kSwapNeighborhood;
        break;
    default:
        throw std::invalid_argument("Niepoprawny wybór sąsiedztwa");
    }
}

int* TabuSearch::solve() {
    tabuList = new int*[numCities];
    for (int i = 0; i < numCities; ++i) {
        tabuList[i] = new int[numCities]();
    }
    Greedy greedy;
    int* bestRoute = new int[numCities + 1];
    int* currentRoute = greedy.solve(distanceMatrix, numCities);
    std::copy(currentRoute, currentRoute + numCities, bestRoute);
    int bestCost = calculateCost(bestRoute);

    auto startTime = std::chrono::steady_clock::now();
    int noImprove = 0;

    while (true) {
        auto currentTime = std::chrono::steady_clock::now();
        double elapsedTime = std::chrono::duration<double>(currentTime - startTime).count();
        if (elapsedTime > timeLimit) break;

        int* bestNeighbor = new int[numCities];
        int neighborCost = (this->*neighborhoodFunc)(currentRoute, bestNeighbor);

        if (neighborCost < bestCost) {
            bestCost = neighborCost;
            std::copy(bestNeighbor, bestNeighbor + numCities, bestRoute);
            noImprove = 0;
            bestFindTime = elapsedTime;
        } else {
            noImprove++;
        }

        // Aktualizacja listy tabu
        for (int i = 0; i < numCities; ++i) {
            for (int j = 0; j < numCities; ++j) {
                if (tabuList[i][j] > 0) tabuList[i][j]--;
            }
        }

        // Oznaczenie ostatniego ruchu jako tabu
        for (int i = 1; i < numCities - 1; ++i) {
            for (int j = i + 1; j < numCities; ++j) {
                if (currentRoute[i] != bestNeighbor[i] || currentRoute[j] != bestNeighbor[j]) {
                    tabuList[i][j] = tabuTenure;
                }
            }
        }

        std::copy(bestNeighbor, bestNeighbor + numCities, currentRoute);

        // Dywersyfikacja w przypadku braku poprawy
        if (noImprove >= maxNoImprove) {
            perturbRoute(currentRoute); // Modyfikujemy trasę zamiast losować nową
            noImprove = 0;
        }

        delete[] bestNeighbor;
    }
    bestRoute[numCities] = bestRoute[0]; // Powrót do pierwszego miasta
    cout << "Czas znalezienia najlepszego rozwiazania: " << bestFindTime << " [s]" << endl;
    delete[] currentRoute;
    return bestRoute;
}

void TabuSearch::perturbRoute(int* route) {
    // Wprowadzenie subtelnych zmian w trasie
    int numPerturbations = numCities / 10; // Zmieniamy 10% trasy
    for (int k = 0; k < numPerturbations; ++k) {
        int i = rand() % (numCities - 1) + 1; // Pomijamy miasto startowe
        int j = rand() % (numCities - 1) + 1;
        if (i != j) {
            std::swap(route[i], route[j]); // Zamiana dwóch losowych miast
        }
    }
}

int TabuSearch::getNumCities()
{
    return numCities;
}

int** TabuSearch::getDistanceMatrix()
{
    return distanceMatrix;
}

void TabuSearch::setDistanceMatrix(int** newMatrix) {
    distanceMatrix = newMatrix;
}

void TabuSearch::setNumCities(int newNumCities) {
    numCities = newNumCities;
}

void TabuSearch::setTenure(int newTenure)
{
    tabuTenure = newTenure;
}

void TabuSearch::setTimeLimit(double newTimeLimit)
{
    timeLimit = newTimeLimit;
}
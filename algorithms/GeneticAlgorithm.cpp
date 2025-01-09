//
// Created by Mateusz on 08.01.2025.
//

#include "GeneticAlgorithm.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <limits>
#include <cstring>
#include <random>
#include <chrono>
#include <conio.h>

using namespace std;

// Konstruktor
// GeneticAlgorithm::GeneticAlgorithm(double** distances, int numCities, int populationSize, int generations, double mutationRate, int tournamentSize)
    // : distances(distances), numCities(numCities), populationSize(populationSize),
      // generations(generations), mutationRate(mutationRate), tournamentSize(tournamentSize),
      // gen(std::random_device{}()) {}

// Destruktor

// Funkcja kosztu - dystans pomiędzy miastami
int GeneticAlgorithm::calculateDistance(int* route) {
    int cost = 0;
    for (int i = 0; i < numCities - 1; ++i) {
        cost += distances[route[i]][route[i + 1]];
    }
    cost += distances[route[numCities - 1]][route[0]]; // Powrót do pierwszego miasta
    return cost;
}

// Generowanie losowego rozwiązania
void GeneticAlgorithm::generateRandomRoute(int* route) {
    for (int i = 0; i < numCities; ++i) {
        route[i] = i;
    }
    shuffle(route, route + numCities, gen);
}

// Selekcja turniejowa
void GeneticAlgorithm::tournamentSelection(int** population, int* fitness, int* selected) {
    uniform_int_distribution<> dist(0, populationSize - 1);
    int bestIndex = dist(gen);
    int bestFitness = fitness[bestIndex];

    for (int i = 1; i < tournamentSize; ++i) {
        int candidateIndex = dist(gen);
        if (fitness[candidateIndex] < bestFitness) {
            bestIndex = candidateIndex;
            bestFitness = fitness[candidateIndex];
        }
    }
    memcpy(selected, population[bestIndex], numCities * sizeof(int));
}

void GeneticAlgorithm::tournamentSelectionDistinct(int** population, int* fitness, int* parent1, int* parent2) {
    uniform_int_distribution<> dist(0, populationSize - 1);

    // Wybór parent1
    int bestIndex1 = dist(gen);
    int bestFitness1 = fitness[bestIndex1];

    for (int i = 1; i < tournamentSize; ++i) {
        int candidateIndex = dist(gen);
        if (fitness[candidateIndex] < bestFitness1) {
            bestIndex1 = candidateIndex;
            bestFitness1 = fitness[candidateIndex];
        }
    }

    // Kopiowanie parent1
    memcpy(parent1, population[bestIndex1], numCities * sizeof(int));

    // Wybór parent2, upewniamy się, że różni się od parent1
    int bestIndex2;
    do {
        bestIndex2 = dist(gen);
    } while (bestIndex2 == bestIndex1); // Gwarancja, że indeksy się różnią

    int bestFitness2 = fitness[bestIndex2];

    for (int i = 1; i < tournamentSize; ++i) {
        int candidateIndex = dist(gen);
        if (candidateIndex != bestIndex1 && fitness[candidateIndex] < bestFitness2) {
            bestIndex2 = candidateIndex;
            bestFitness2 = fitness[candidateIndex];
        }
    }
    // Kopiowanie parent2
    memcpy(parent2, population[bestIndex2], numCities * sizeof(int));

    // cout << "p1: " << bestIndex1 << " p2: " << bestIndex2 << endl;
}

void GeneticAlgorithm::chooseParents(int** population, int* fitness, int* parent1, int* parent2, int* areParents)
{
    uniform_int_distribution<> dist(0, populationSize - 1);
    int index1 = dist(gen);
    while (isParent(areParents,index1)) {
        index1 = dist(gen);
    }
    // Kopiowanie parent1
    memcpy(parent1, population[index1], numCities * sizeof(int));
    areParents[index1] = 1;

    int index2 = dist(gen);
    while (isParent(areParents,index2)) {
        index2 = dist(gen);
    }
    // Kopiowanie parent2
    memcpy(parent2, population[index2], numCities * sizeof(int));
    areParents[index2] = 1;
}


// Krzyżowanie jednopunktowe
void GeneticAlgorithm::crossover(int* parent1, int* parent2, int* child) {
    uniform_int_distribution<> dist(0, numCities - 1);
    int start = dist(gen);
    int end = dist(gen);

    if (start > end) swap(start, end);

    bool* used = new bool[numCities]();
    for (int i = start; i <= end; ++i) {
        child[i] = parent1[i];
        used[parent1[i]] = true;
    }

    int currentIdx = (end + 1) % numCities;
    for (int i = 0; i < numCities; ++i) {
        int city = parent2[(end + 1 + i) % numCities];
        if (!used[city]) {
            child[currentIdx] = city;
            currentIdx = (currentIdx + 1) % numCities;
        }
    }

    delete[] used;
}

void GeneticAlgorithm::pmxCrossover(int* parent1, int* parent2, int* child) {
    {
        // Losowy wybór dwóch punktów podziału
        std::uniform_int_distribution<> dist(0, numCities - 1);

        int start = dist(gen);
        int end = dist(gen);
        while (start == end) {
            start = dist(gen);
            end = dist(gen);
        }
        if (start > end) std::swap(start, end);

        int segmentSize = end - start + 1;
        int* segment = new int[segmentSize];
        memcpy(segment, parent1 + start, segmentSize * sizeof(int));
        // Inicjalizacja dziecka
        for (int i = 0; i < numCities; ++i) {
            child[i] = -1; // Wartość -1 oznacza puste miejsce
        }

        // Skopiowanie segmentu z parent1 do dziecka
        for (int i = start; i <= end; ++i) {
            child[i] = parent1[i];
        }

        //Znajdowanie elementów w P2, które nie zostały skopiowane
        for (int i = start; i <= end; ++i)
        {
            if (!inArr(segment, segmentSize, parent2[i]))
            {
                int pos = findEl(parent2, numCities, parent1[i]);
                // Rekurencyjne poszukiwanie wolnej pozycji
                while (child[pos] != -1)
                {
                    int numOnPos = parent1[pos]; // Sprawdzam jaka liczba jest na pozycji, którą obecnie sprawdzamy
                    pos = findEl(parent2, numCities, numOnPos); // Sprawdzam, gdzie znajduje się ta liczba w Parent 2
                }
                child[pos] = parent2[i];
            }
        }
        delete[] segment;
        // Kopiowanie pozostałych wierzchołków z Parent 2
        for (int i = 0; i < numCities; ++i)
        {
            if (child[i] == -1)
            {
                child[i] = parent2[i];
            }
        }
    }
}

// Mutacja - zamiana dwóch miast
void GeneticAlgorithm::mutate(int* route) {
    uniform_int_distribution<> dist(0, numCities - 1);
    int i = dist(gen);
    int j = dist(gen);
    swap(route[i], route[j]);
}


int* GeneticAlgorithm::solve() {
    // Inicjalizacja populacji
    int** population = new int*[populationSize];
    for (int i = 0; i < populationSize; ++i) {
        population[i] = new int[numCities];
        generateRandomRoute(population[i]);
    }
    // for (int i = 0; i < populationSize; ++i)
    // {
        // displayArray(population[i],numCities);
    // }

    int* fitness = new int[populationSize];

    int elitismCount = static_cast<int>(elitismRate * populationSize);

    // Ewolucja
    for (int generation = 0; generation < generations; ++generation) {
        // Obliczanie przystosowania
        for (int i = 0; i < populationSize; ++i) {
            fitness[i] = calculateDistance(population[i]);
        }
        // Tworzenie nowej populacji
        int** newPopulation = new int*[populationSize];

        // Elitarne osobniki - kopiowanie najlepszych do nowej populacji
        for (int i = 0; i < elitismCount; ++i) {
            newPopulation[i] = new int[numCities];
            int bestIndex = min_element(fitness, fitness + populationSize) - fitness;
            memcpy(newPopulation[i], population[bestIndex], numCities * sizeof(int));
            fitness[bestIndex] = numeric_limits<int>::max(); // Wyklucz najlepszy z kolejnych wyborów

        }
        //Inicjalizacja tablicy z informacjami, ktora populacja była już parentem
        int* areParents = new int[populationSize];
        for (int i = 0; i < populationSize; ++i) {
            areParents[i] = 0;
        }
        // Reszta populacji - krzyżowanie i mutacja
        for (int i = elitismCount; i < populationSize; ++i) {
            newPopulation[i] = new int[numCities];
            int* parent1 = new int[numCities];
            int* parent2 = new int[numCities];
            //Wybieranie rodziców
            tournamentSelectionDistinct(population,fitness,parent1,parent2);
            // Krzyżowanie z prawdopodobieństwem crossoverRate
            if (uniform_real_distribution<>(0.0, 1.0)(gen) < crossoverRate) {
                pmxCrossover(parent1, parent2, newPopulation[i]);
            } else {
                // Brak krzyżowania, kopiowanie rodzica
                memcpy(newPopulation[i], parent1, numCities * sizeof(int));
            }
            // Mutacja
            if (uniform_real_distribution<>(0.0, 1.0)(gen) < mutationRate) {
                mutate(newPopulation[i]);
            }
            delete[] parent1;
            delete[] parent2;
        }
        // Zamiana starej populacji na nową
        for (int i = 0; i < populationSize; ++i) {
            delete[] population[i];
        }
        delete[] population;

        population = newPopulation;
    }

    // Znalezienie najlepszego rozwiązania
    int* bestRoute = new int[numCities + 1];
    int bestCost = numeric_limits<int>::max();

    for (int i = 0; i < populationSize; ++i) {
        int cost = calculateDistance(population[i]);
        if (cost < bestCost) {
            bestCost = cost;
            memcpy(bestRoute, population[i], numCities * sizeof(int));
            bestRoute[numCities] = bestRoute[0];
        }
    }

    // Czyszczenie pamięci
    for (int i = 0; i < populationSize; ++i) {
        delete[] population[i];
    }
    delete[] population;
    delete[] fitness;

    return bestRoute;
}


void GeneticAlgorithm::setDistanceMatrix(int **newDistanceMatrix) {
    distances = newDistanceMatrix;
}

void GeneticAlgorithm::setNumCities(int newNumCities) {
    numCities = newNumCities;
}

void GeneticAlgorithm::displayMatrix(int** matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%-5d",matrix[i][j]);
        }
        printf("\n");
    }
}

void GeneticAlgorithm::displayArray(int* arr, int n) {
    cout << "[ ";
    for (int i = 0 ; i < n ; i++)
    {
        cout << arr[i] << " ";
    }
    cout << "]" << endl;
}

bool GeneticAlgorithm::isParent(int* parents, int target) {
    if (parents[target] == 1)
    {
        return true;
    }
    return false;
}

int GeneticAlgorithm::findEl(int* arr, int size, int target)
{
    for (int i = 0; i < size; ++i) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}

bool GeneticAlgorithm::inArr(int* arr, int size, int target)
{
    for (int i = 0; i < size; ++i)
    {
        if (arr[i] == target)
        {
            return true;
        }
    }
    return false;
}

bool GeneticAlgorithm::isUnique(int* arr, int size) {
    int *counter = new int[size];
    for (int i = 0; i < size; ++i){
        counter[i] = 0;
    }
    for (int i = 0; i < size; ++i){
        counter[arr[i]]++;
    }
    for (int i = 0; i < size; ++i)
    {
        if (counter[i] != 1){
            delete[] counter;
            return false;
        }
    }
    delete[] counter;
    return true;
}
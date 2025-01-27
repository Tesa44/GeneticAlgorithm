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

//Selekcja turniejowa
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
}


void GeneticAlgorithm::orderCrossover(int* parent1, int* parent2, int* child) {
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
    // Inicjalizacja dziecka
    for (int i = 0; i < numCities; ++i) {
        child[i] = -1;
    }
    // Kopiowanie segmentu P1 zachowując względną kolejność miast w cyklu
    memcpy(child + start, parent1 + start, segmentSize * sizeof(int));

    int p2Index = end;
    int childIndex = end;
    int p2El;
    for (int i = 0 ; i < numCities; i++) {
        p2Index = ++p2Index % numCities;
        p2El = parent2[p2Index];
        if (!inArr(child, numCities, p2El)) {
            childIndex = ++childIndex % numCities;
            child[childIndex] = p2El;
        }
    }
}

void GeneticAlgorithm::pmxCrossover(int* parent1, int* parent2, int* child) {
    {
        // Losowy wybór dwóch punktów podziału
        std::uniform_int_distribution<> dist(0, numCities - 1);

        int start = dist(gen);
        int end = dist(gen);
        while (start == end) {
            start = dist(gen);
        }
        if (start > end) std::swap(start, end);
        // cout << "start: " << start << " end: " << end << endl;
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

// Mutacja - inversion Mutation
void GeneticAlgorithm::mutate(int* route) {
    uniform_int_distribution<> dist(0, numCities - 1);
    int i = dist(gen);
    int j = dist(gen);
    while (i == j) {
        j = dist(gen);
    }
    if (i > j) {
        swap(i, j);
    }
    std::reverse(route + i, route + j + 1);
}


int* GeneticAlgorithm::solve() {
    auto startTime = std::chrono::steady_clock::now();
    int* bestRoute = new int[numCities + 1];
    int bestCost = std::numeric_limits<int>::max();
    double bestFindTime = 0;
    //Parametry do liczenia średniego względnego błedu
    int opt = 2755;
    double interval = 10.0; //Interwal co ile mierzymy sredni błąd względny
    double lastMeasurementTime = 0.0;
    int measurementSize = 24;
    double* measurement = new double[measurementSize];
    int measurementIndex = 0;

    // Inicjalizacja populacji
    int** population = new int*[populationSize];
    for (int i = 0; i < populationSize; ++i) {
        population[i] = new int[numCities];
        generateRandomRoute(population[i]);
    }

    int* fitness = new int[populationSize];
    int elitismCount = static_cast<int>(elitismRate * populationSize);
    while (true) {
        auto currentTime = std::chrono::steady_clock::now();
        double elapsedTime = std::chrono::duration<double>(currentTime - startTime).count();
        if (elapsedTime > timeLimit)
        {
            break;
        };
        // Obliczanie przystosowania
        for (int i = 0; i < populationSize; ++i) {
            fitness[i] = calculateDistance(population[i]);
        }

        if (std::chrono::duration<double>(elapsedTime - lastMeasurementTime).count() > interval)
        {
            double avgResult = countAvgResult(fitness);
            double avgError = (avgResult - opt) / opt;
            measurement[measurementIndex++] = avgError;
            lastMeasurementTime = elapsedTime;
        }

        // Tworzenie nowej populacji
        int** newPopulation = new int*[populationSize];

        // Elitarne osobniki - kopiowanie najlepszych do nowej populacji
        for (int i = 0; i < elitismCount; i++) {
            newPopulation[i] = new int[numCities];
            int bestIndex = std::min_element(fitness, fitness + populationSize) - fitness;
            memcpy(newPopulation[i], population[bestIndex], numCities * sizeof(int));
            fitness[bestIndex] = std::numeric_limits<int>::max(); // Wyklucz najlepszy z kolejnych wyborów
        }

        // Reszta populacji - krzyżowanie i mutacja
        for (int i = elitismCount; i < populationSize; i += 2) {
            newPopulation[i] = new int[numCities];
            newPopulation[i + 1] = new int[numCities];
            int* parent1 = new int[numCities];
            int* parent2 = new int[numCities];

            // Wybieranie rodziców
            tournamentSelectionDistinct(population, fitness, parent1, parent2);

            //Krzyżowanie
            if (uniform_real_distribution<>(0.0, 1.0)(gen) < crossoverRate) {
                (this->*crossoverFunc)(parent1, parent2, newPopulation[i]);
                (this->*crossoverFunc)(parent2, parent1, newPopulation[i + 1]);
            } else {
                // Brak krzyżowania, kopiowanie rodzica
                memcpy(newPopulation[i], parent1, numCities * sizeof(int));
                memcpy(newPopulation[i + 1], parent2, numCities * sizeof(int));
            }

            // Mutacja
            if (uniform_real_distribution<>(0.0, 1.0)(gen) < mutationRate) {
                mutate(newPopulation[i]);
            }
            if (uniform_real_distribution<>(0.0, 1.0)(gen) < mutationRate) {
                mutate(newPopulation[i + 1]);
            }

            delete[] parent1;
            delete[] parent2;
        }

        // Zamiana starej populacji na nową (sukcesja)
        for (int i = 0; i < populationSize; ++i) {
            delete[] population[i];
        }
        delete[] population;

        population = newPopulation;

        // Znalezienie najlepszego rozwiązania w obecnej populacji
        for (int i = 0; i < populationSize; ++i) {
            int cost = calculateDistance(population[i]);
            if (cost < bestCost) {
                bestCost = cost;
                memcpy(bestRoute, population[i], numCities * sizeof(int));
                bestRoute[numCities] = bestRoute[0];
                bestFindTime = elapsedTime;
            }
        }
    }
    //Ostatni sredni blad wzgledny
    for (int i = 0; i < populationSize; ++i) {
        fitness[i] = calculateDistance(population[i]);
    }
    double avgResult = countAvgResult(fitness);
    double avgError = (avgResult - opt) / opt;
    measurement[measurementIndex] = avgError;

    // Czyszczenie pamięci
    for (int i = 0; i < populationSize; ++i) {
        delete[] population[i];
    }
    delete[] population;
    delete[] fitness;

    std::cout << "Czas znalezienia najlepszego rozwiązania: " << bestFindTime << " [s]" << std::endl;
    // cout << "Srednie bledy wzgledne: "<< endl;
    // displayArraydouble(measurement,measurementSize);
    delete[] measurement;
    return bestRoute;
}


void GeneticAlgorithm::setCrossover(int choice) {
    switch (choice) {
    case 1:
        crossoverFunc = &GeneticAlgorithm::pmxCrossover;
        break;
    case 2:
        crossoverFunc = &GeneticAlgorithm::orderCrossover;
        break;
    default:
        throw std::invalid_argument("Niepoprawny wybór sąsiedztwa");
    }
}


void GeneticAlgorithm::setDistanceMatrix(int **newDistanceMatrix) {
    distances = newDistanceMatrix;
}

void GeneticAlgorithm::setNumCities(int newNumCities) {
    numCities = newNumCities;
}

void GeneticAlgorithm::displayArraydouble(double* arr, int n) {
    cout << "[ ";
    for (int i = 0 ; i < n ; i++)
    {
        cout << arr[i] << " ";
    }
    cout << "]" << endl;
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

void GeneticAlgorithm::setTimeLimit(double timeLimit)
{
    this->timeLimit = timeLimit;
}

double GeneticAlgorithm::countAvgResult(int* fitness)
{
    int sum = 0;
    for (int i = 0; i < populationSize; ++i)
    {
        sum += fitness[i];
    }
    return sum / populationSize;
}

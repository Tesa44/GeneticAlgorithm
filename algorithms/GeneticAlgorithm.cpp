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

// Mutacja - zamiana dwóch miast
void GeneticAlgorithm::mutate(int* route) {
    uniform_int_distribution<> dist(0, numCities - 1);
    int i = dist(gen);
    int j = dist(gen);
    swap(route[i], route[j]);
}

// Główna metoda algorytmu genetycznego
int* GeneticAlgorithm::solve() {
    // Inicjalizacja populacji
    int** population = new int*[populationSize];
    for (int i = 0; i < populationSize; ++i) {
        population[i] = new int[numCities];
        generateRandomRoute(population[i]);
    }

    int* fitness = new int[populationSize];

    // Ewolucja
    for (int generation = 0; generation < generations; ++generation) {
        cout << "generacja: " << generation << endl;
        // Obliczanie przystosowania
        for (int i = 0; i < populationSize; ++i) {
            fitness[i] = calculateDistance(population[i]);
        }
        // cout << "Po fitness" << endl;
        // Tworzenie nowej populacji
        int** newPopulation = new int*[populationSize];
        for (int i = 0; i < populationSize; ++i) {
            newPopulation[i] = new int[numCities];
            // cout << "Po newPopulation: " << i << endl;
            int* parent1 = new int[numCities];
            int* parent2 = new int[numCities];
            tournamentSelection(population, fitness, parent1);
            tournamentSelection(population, fitness, parent2);
            // cout << "Po tournament: " << i << endl;
            crossover(parent1, parent2, newPopulation[i]);
            // cout << "Po crossover: " << i << endl;
            if (uniform_real_distribution<>(0.0, 1.0)(gen) < mutationRate) {
                mutate(newPopulation[i]);
            }
            // cout << "Po mutation: " << i << endl;

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
    int* bestRoute = new int[numCities];
    int bestCost = numeric_limits<int>::max();

    for (int i = 0; i < populationSize; ++i) {
        int cost = calculateDistance(population[i]);
        if (cost < bestCost) {
            bestCost = cost;
            memcpy(bestRoute, population[i], numCities * sizeof(int));
            cout << bestCost << endl;
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

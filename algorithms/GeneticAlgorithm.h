//
// Created by Mateusz on 08.01.2025.
//

#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include <random>

class GeneticAlgorithm {
public:
    // Konstruktor
    // GeneticAlgorithm(double** distances, int numCities, int populationSize, int generations, double mutationRate, int tournamentSize);

    // Metoda do uruchomienia algorytmu
    int* solve();

    // Destruktor
    // ~GeneticAlgorithm();

    //Settey
    void setNumCities(int newNumCities);
    void setDistanceMatrix(int** newDistanceMatrix);
    void setCrossover(int choice);

private:
    // Dane wejściowe
    int** distances;
    int numCities = 0;
    int populationSize = 200; // 50 / 100 / 200
    int generations = 2000; // 1000 - 2000
    double mutationRate = 0.01;
    int tournamentSize = 3;
    double elitismRate = 0.4;   // Współczynnik elityzmu, np. 0.2
    double crossoverRate = 0.8; // Prawdopodobieństwo krzyżowania, np. 0.8
    double timeLimit = 10.0;

    // Generator liczb losowych
    std::mt19937 gen;

    // Funkcje pomocnicze
    int calculateDistance(int* route);
    void generateRandomRoute(int* route);
    void tournamentSelection(int** population, int* fitness, int* selected);
    void tournamentSelectionDistinct(int** population, int* fitness, int* parent1, int* parent2);
    void chooseParents(int** population, int* fitness, int* parent1, int* parent2, int* areParents);
    void crossover(int* parent1, int* parent2, int* child);
    void pmxCrossover(int* parent1, int* parent2, int* child);
    void orderCrossover(int* parent1, int* parent2, int* child);
    void (GeneticAlgorithm::*crossoverFunc)(int*, int*, int*) = &pmxCrossover; // Wskaźnik na funkcję definiującą sąsiedztwo
    void mutate(int* route);
    void displayMatrix(int** matrix, int n);
    void displayArray(int* arr, int n);
    bool isParent(int* parents, int target);
    int findEl(int* arr, int size, int target);
    bool isUnique(int* arr, int size);
    bool inArr(int* arr, int size, int target);
};

#endif // GENETIC_ALGORITHM_H

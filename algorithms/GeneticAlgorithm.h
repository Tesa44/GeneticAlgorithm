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

private:
    // Dane wejściowe
    int** distances;
    int numCities = 0;
    int populationSize = 100;
    int generations = 1500;
    double mutationRate = 0.02;
    int tournamentSize = 3;


    // Generator liczb losowych
    std::mt19937 gen;

    // Funkcje pomocnicze
    int calculateDistance(int* route);
    void generateRandomRoute(int* route);
    void tournamentSelection(int** population, int* fitness, int* selected);
    void crossover(int* parent1, int* parent2, int* child);
    void mutate(int* route);
    void displayMatrix(int** matrix, int n);
};

#endif // GENETIC_ALGORITHM_H

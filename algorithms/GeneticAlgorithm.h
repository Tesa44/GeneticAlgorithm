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
    void setTimeLimit(double timeLimit);

private:
    // Dane wejściowe
    int** distances;
    int numCities = 0;
    int populationSize = 1000; // 50 / 100 / 200
    int generations = 2000; // 1000 - 2000
    double mutationRate = 0.01;
    int tournamentSize = 3;
    double elitismRate = 0.2;   // Współczynnik elityzmu, np. 0.2
    double crossoverRate = 0.8; // Prawdopodobieństwo krzyżowania, np. 0.8
    double timeLimit = 1000.0;

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
    void crossoverPMX(int* parent1, int* parent2, int* child);
    void orderCrossover(int* parent1, int* parent2, int* child);
    void (GeneticAlgorithm::*crossoverFunc)(int*, int*, int*) = &orderCrossover; // Wskaźnik na funkcję definiującą sąsiedztwo
    void mutate(int* route);
    void displayMatrix(int** matrix, int n);
    void displayArray(int* arr, int n);
    bool isParent(int* parents, int target);
    int findEl(int* arr, int size, int target);
    bool isUnique(int* arr, int size);
    bool inArr(int* arr, int size, int target);
    double countAvgResult(int* fitness);
    void displayArraydouble(double* arr, int n);
};

#endif // GENETIC_ALGORITHM_H

//Pseudokod algorytmu Genetycznego
// geneticAlgorithm()
//     population = generateRandomPopulation()
//     fitness = initFitness()
//     bestRoute = initBestRoute()
//     while not timeExceed():
//         //Obliczanie przystosowania
//         fitness = calculateFitness(population)
//         //Tworzenie nowej populacji
//         newPopulation = []
//         //Elityzm
//         copyBestChromosomes(populatio, newPopulation)
//         //Reszta populacji - krzyżowanie i mutacja
//         while not populationCompleted(newPopulation):
//             //Selekcja rodziców
//             parent1 = []
//             parent2 = []
//             selection(population,fitness,parent1, parent2)
//             //Krzyzowanie
//             child1 = []
//             child2 = []
//             if (corssoverProbability()):
//                 crossover(parent1,parent2, child1)
//                 crossover(parent2,parent1, child2)
//             else:
//                 child1 = parent1
//                 child2 = parent2
//             //Mutacja
//             if (mutationProbability()):
//                 mutate(child1)
//             if (mutationProbability()):
//                 mutate(child2)
//             addChildrenToPopulation(newPopulation,child1,child2)
//
//             //Sukcesja
//             population = newPopulation
//
//             bestRoute = findBestRoute()
//
//         return bestRoute

//Pseudokod orderCrossover
// oderCrossover(parent1, parent2, child):
//     //Losowanie dwóch różnych indeksów
//     start, end = generateRandomGap()
//     //Kopiowanie wylosowanego fragmentu z parent1 do child
//     child[start:end] = parent1[start:end]
//     //Kopiowanie reszty miast do child w kolejności parent2
//     parent2Pos = end
//     childPos = end
//     while not childCompleted():
//         parent2Pos = ++parent2Pos % parent2.size()
//         if parent2[parent2Pos] not in child:
//             childPos = ++childPos % parent2.size()
//             child[childPos] = parent2[parent2Pos]

//Pseudokod pmxCrossover
// pmxCrossover(parent1, parent2, child):
//     //Losowanie dwóch róznych indeksów
//     start, end = generateRandomGap()
//     //Kopiowanie wylosowanego fragmentu z parent1 do child
//     fragment = parent[start:end]
//     child[start:end] = fragment
//     //Kopiowanie elementów z parent2, które są w fragmencie do child
//     for  i in range(start, end):
//         //Patrzymy na analogiczny fragment w P2 i szukamy elementów
//         //które nie zostały skopiowane z p1
//         if not parent2[i] in fragment:
//             //Szukamy pozycji elementu w p2, który znajduje się
//             //Na pozycji i w parent1
//             elPos = findEl(parent1[i],parent2)  //(target, array)
//             //Rekurencyjne przeszukiwanie wolnej pozycji w child
//             while child[pos] not free:
//                 //sprawdzamy jaka liczba jest na pozycji elPos
//                 //I sprawdzamy, gdzie znajduje się ta liczba w parent2
//                 elPos = findEl(parent1[elPos], parent2)
//             child[elPos] = parent2[i]
//
//     //Kopiowanie reszty elementów z parent2
//     for i in range(parent2.size()):
//         if child[i] is free:
//             child[i] = parent2[i]

//Pseudokod mutation
// mutation(child):
//     //Losowanie dwóch różnych indeksów
//     start, end = generateRandomGap()
//     reverse(child[start:end])

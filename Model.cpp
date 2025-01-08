#include "Model.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

int Model::calculateCostFromFile(const std::string& filename, int** distanceMatrix) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Nie można otworzyć pliku do odczytu: " + filename);
    }

    // Wczytaj trasę z pliku
    int cities = 0;
    int* route = new int[numCities + 1];

    file >> cities;

    for (int i = 0; i < cities + 1; ++i) {
        file >> route[i];
    }
    file.close();

    // Oblicz koszt trasy
    int totalCost = 0;
    for (int i = 0; i < cities; ++i) {
        totalCost += distanceMatrix[route[i]][route[i + 1]];
    }

    delete[] route; // Zwolnij pamięć
    return totalCost;
}


void Model::saveResultToFile(const std::string& filename, int* route, int numCities) {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Nie można otworzyć pliku do zapisu: " + filename);
    }
    file << numCities << "\n";
    for (int i = 0; i < numCities + 1; ++i) {
        file << route[i] << " ";
    }
    file.close();
}


void Model::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) throw std::runtime_error("Nie można otworzyć pliku: " + filename);

    std::string line;
    while (std::getline(file, line)) {
        //Usuwanie białych znaków z początku i końca linii
        line.erase(0, line.find_first_not_of(" \t\n\r"));
        line.erase(line.find_last_not_of(" \t\n\r") + 1);

        if (line.empty() || line[0] == '#') {
            continue; // Ignoruj puste linie i komentarze
        }
        if (line.find("DIMENSION") != std::string::npos) {
            std::string temp;
            std::istringstream iss;
            iss.str(line);
            iss >> temp; // DIMENSION
            iss >> numCities;   //Liczba miast
        } else if (line.find("EDGE_WEIGHT_SECTION") != std::string::npos) {
            break;
        }
    }
    distanceMatrix = new int*[numCities];
    for (int i = 0; i < numCities; ++i) {
        distanceMatrix[i] = new int[numCities];
        for (int j = 0; j < numCities; ++j) {
            //Wczytywanie wartości macierzy z pliku
            file >> distanceMatrix[i][j];
        }
    }
}

int Model::getNumCities() {
    return numCities;
}

int **Model::getDistanceMatrix() {
    return distanceMatrix;
}



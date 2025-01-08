//
// Created by Mateusz on 25.11.2024.
//

#ifndef MODEL_H
#define MODEL_H
#include <string>


class Model {
private:
    int numCities = -1;
    int** distanceMatrix;
public:
    void loadFromFile(const std::string& fileName);
    void saveResultToFile(const std::string& filename, int* route, int numCities);
    int calculateCostFromFile(const std::string& filename, int** distanceMatrix);
    int getNumCities();
    int** getDistanceMatrix();
};



#endif //MODEL_H

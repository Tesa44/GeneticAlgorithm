//
// Created by mateu on 15.12.2024.
//

#ifndef GREEDY_H
#define GREEDY_H



class Greedy {
public:
    int* solve(int** distanceMatrix, int numCities);
private:
    int calculateCost(int* route, int** distanceMatrix, int numCities);
};



#endif //GREEDY_H

//
// Created by Mateusz on 25.11.2024.
//

#include "Helpers.h"

#include <iostream>

void Helpers::displayMatrix(int** matrix, int n)
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

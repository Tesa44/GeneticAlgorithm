//
// Created by Mateusz on 25.11.2024.
//

#include "App.h"

#include <iostream>
#include <sstream>
#include <conio.h>

#include "algorithms/SimAnnealing.h"



void App::run()
{
    //Inicjalizacja
    bool isMatrix = false;
    string path;
    double timeLimit;
    int tsChoice;
    int smChoice;
    int gaChoice;
    bool exit = false;

    while (!exit)
    {
        menu.mainMenu();

        switch (menu.mainChoice) {
        case '1':
            if (isMatrix) {
                runAlgorithms();
            }
            else {
                cout << "Nie masz zadnej zapisanej macierzy!" << endl;
            }
            break;
        case '2':
            // path = menu.inputPath();
                path = "ftv55.atsp";
            model.loadFromFile(path);
            ts.setDistanceMatrix(model.getDistanceMatrix());
            ts.setNumCities(model.getNumCities());
            sw.setDistanceMatrix(model.getDistanceMatrix());
            sw.setNumCities(model.getNumCities());

            ga.setDistanceMatrix(model.getDistanceMatrix());
            ga.setNumCities(model.getNumCities());

            isMatrix = true;
            cout << "Wczytano macierz" << endl;
            break;
        case '3':
            timeLimit = menu.inputStop();
            ts.setTimeLimit(timeLimit);
            sw.setTimeLimit(timeLimit);
            cout << "Zmieniono kryterium stopu" << endl;
            break;
        case '4':
            menu.tsMenu();
            tsChoice = menu.tsChoice - '0';
            ts.setNeighborhood(tsChoice);
            cout << "Zmieniono metode sasiedztwa" << endl;
            break;
        case '5':
            menu.swMenu();
            smChoice = menu.swChoice - '0';
            sw.setCoolingScheme(smChoice);
            cout << "Zmieniono schemat schladzania" << endl;
            break;
        case '6':
            sw.setCoolingRate(menu.inputFactorA());
            cout << "Zmieniono wspolczynnik schladzania" << endl;
            break;
        case '7':
            menu.gaMenu();
            gaChoice = menu.gaChoice - '0';
            ga.setNumCities(gaChoice);
            cout << "Zmieniono metode krzyzowania" << endl;
            break;
        case '8':
            if (isMatrix)
            {
                solution = greedy.solve(model.getDistanceMatrix(), model.getNumCities());
                model.saveResultToFile(filepath,solution,model.getNumCities());
                cout << "Wynik z metody zachlannej: " << model.calculateCostFromFile(filepath,model.getDistanceMatrix()) << endl;
                cout << "Kliknij dowolny klawisz aby wrocic do menu" << endl;
                getch();
                delete[] solution;
            }
            else
            {
                cout << "Nie masz zadnej zapisane macierzy!" << endl;
            }
            break;
        case '0':
            exit = true;
            break;
        }
    }
}

void App::runAlgorithms()
{
        menu.algorithmsMenu();
        srand(time(NULL));
        switch (menu.algorithmChoice) {
        case '1':
                solution = ts.solve();
            break;
        case '2':
            solution = sw.solve();
            break;
        case '3':
            solution = ga.solve();
            break;
            default:
                cout << "Error" << endl;
        }
        model.saveResultToFile(filepath,solution,model.getNumCities());
        cout << "Najkrotsza sciezka: " << model.calculateCostFromFile(filepath,model.getDistanceMatrix()) << endl;

        delete[] solution;

        cout << "Kliknij dowolny klawisz aby wrocic do menu" << endl;
        getch();
}


//
// Created by Mateusz on 25.11.2024.
//

#ifndef MENU_H
#define MENU_H
#include <string>
using namespace std;

class Menu {
public:
    char mainChoice;
    char algorithmChoice;
    char exitConfirmChoice;
    char tsChoice;
    char swChoice;
    void mainMenu();
    void algorithmsMenu();
    double inputStop();
    double inputFactorA();
    void tsMenu();
    void swMenu();
    int n;
    bool checkChoices(char choice, char choiceQuantity);
    string inputPath();
    void exitConfirmMenu();
};



#endif //MENU_H

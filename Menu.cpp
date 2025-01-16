//
// Created by Mateusz on 25.11.2024.
//

#include "Menu.h"

#include <iostream>
using namespace std;


void Menu::mainMenu() {
    while (true) {
        cout << endl;
        cout << "1. Przejdz do algorytmow" << endl;
        cout << "2. Wczytaj macierz z pliku" << endl;
        cout << "3. Podaj kryterium stopu" << endl;
        cout << "4. Wybierz metode sasiedztwa dla TS" << endl;
        cout << "5. Wybierz metode schladzania dla SW" << endl;
        cout << "6. Ustaw wspolczynnik a dla SW" << endl;
        cout << "7. Wybierz metode krzyzowania dla GA" << endl;
        cout << "8. Rozwiaz metoda zachlanna" << endl;
        cout << "0. Wyjdz z programu" << endl;
        cin >> mainChoice;
        if (checkChoices(mainChoice, '8'))
            return;
    }
}

void Menu::tsMenu()
{
    while (true) {
        cout << "Wybierz jedna z metod wyboru sasiedztwa dla TS" << endl;
        cout << "1. SWAP - zamiana dwoch miast w trasie" << endl;
        cout << "2. INSERT - przeniesienie jednego miasta w innej miejsce" << endl;
        cout << "3. KSWAP - zmiana trzech miast w trasie" << endl;
        cin >> tsChoice;
        if (checkChoices(tsChoice, '3')) return;
    }
}
void Menu::swMenu()
{
    while (true) {
        cout << "Wybierz jedna z metod schladzania dla SW" << endl;
        cout << "1. T(i+1) = a * T(i)" << endl;
        cout << "2. T(i+1) = T(i) / (1 + 0.001 * T(i))" << endl;
        cout << "3. T(i+1) = T(i) / log(i + 2)" << endl;
        cin >> swChoice;
        if (checkChoices(swChoice, '3')) return;
    }
}

void Menu::gaMenu()
{
    while (true) {
    cout << "Wybierz jedna z metod krzyzowania dla GA" << endl;
    cout << "1. PMX Crossover" << endl;
    cout << "2. Order Crossover" << endl;
    cin >> gaChoice;
    if (checkChoices(gaChoice, '2')) return;
    }
}

string Menu::inputPath() {
    string path;
    cout << "Podaj sciezke do pliku atsp " << endl;
    cin >> path;
    return path;
}


//Metoda służy do sprawdzania czy wybór wpisany przez użytkownika jest zgodny z jakimkolwiek wyborem

//char choice - znak podany przez użytkownika określający wybór

//char choiceQuantity - ile posiadamy wyborów w menu

bool Menu::checkChoices(char choice, char choiceQuantity) {
    if (choice >= '1' && choice <= choiceQuantity)
        return true;
    cout << "Niepoprawny wybor!" << endl;
    return false;
}


void Menu::exitConfirmMenu() {
    while (true) {
        cout << "Czy chcesz wyjsc z programu?" << endl;
        cout << "1. Wroc do algorytmow" << endl;
        cout << "2. Zamknij program" << endl;
        cin >> exitConfirmChoice;
        if (checkChoices(exitConfirmChoice, '2'))
            return;
    }
}

void Menu::algorithmsMenu() {
    cout << "Wybierz algorytm do problemu ATSP" << endl;
    cout << "1. Algorytm Tabu Search" << endl;
    cout << "2. Algorytm Symulowanego Wyzarzania" << endl;
    cout << "3. Algorytm Genetyczny" << endl;
    cin >> algorithmChoice;
    if (checkChoices(algorithmChoice, '3'))
        return;
}

double Menu::inputStop() {
    string input;
    bool guard;
    while (true) {
        guard = true;
        cout << "Podaj kryterium stopu " << endl;
        cin >> input;
        for (int i = 0; i < input.length(); i++) {
            if (isdigit(input[i] == false))
                guard = false;
        }
        if (guard) {
            double n = stod(input);
            if (n > 0)
                return n;
        }
        cout << "To nie jest liczba wieksza od 0" << endl;
    }
}


double Menu::inputFactorA() {
    string input;
    bool guard;
    while (true) {
        guard = true;
        cout << "Podaj wspolczynnik a (0 - 1)" << endl;
        cin >> input;
        for (int i = 0; i < input.length(); i++) {
            if (isdigit(input[i] == false))
                guard = false;
        }
        if (guard) {
            double n = stod(input);
            if (n > 0 && n < 1)
                return n;
        }
        cout << "To nie jest liczba z zakresu 0 - 1" << endl;
    }
}
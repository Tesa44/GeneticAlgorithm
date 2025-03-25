# Genetic algorithm solving ATSP
 eng below
## Wstęp
Asymetryczny problem komiwojażera (ATSP, z ang. Asymmetric Traveling 
Salesman Problem) stanowi jedno z kluczowych wyzwań optymalizacyjnych 
w teorii grafów oraz badań operacyjnych. Zadanie to polega na wyznaczeniu 
najkrótszej możliwej trasy, która pozwala komiwojażerowi odwiedzić 
wszystkie zadane miasta dokładnie raz i powrócić do punktu początkowego, 
przy czym odległości między miastami mogą się różnić w zależności od 
kierunku przejścia (asymetria). ATSP znajduje szerokie zastosowanie w 
logistyce, planowaniu tras oraz optymalizacji produkcji, a także w systemach 
komunikacji miejskiej i transporcie.
Ze względu na swoją złożoność obliczeniową (problem należy do klasy NPtrudnych), rozwiązywanie ATSP wymaga zastosowania specjalistycznych 
algorytmów, takich jak algorytmy przeglądu zupełnego, podziału i ograniczeń 
lub programowania dynamicznego. Celem niniejszego badania jest ocena 
efektywności wybranych algorytmów stosowanych do rozwiązywania ATSP, 
poprzez analizę ich wyników w kontekście jakości uzyskanych rozwiązań.

## Genetic algorithm
Algorytm genetyczny to metaheurystyczna metoda optymalizacji inspirowana 
procesami ewolucyjnymi, która polega na iteracyjnym poszukiwaniu 
najlepszego rozwiązania problemu poprzez symulację mechanizmów takich 
jak selekcja, krzyżowanie i mutacja. Proces rozpoczyna się od losowego 
wygenerowania populacji potencjalnych rozwiązań, z których każde oceniane 
jest za pomocą funkcji przystosowania określającej jego jakość. Następnie na 
podstawie wartości przystosowania wybierane są rozwiązania do kolejnego 
pokolenia (selekcja), przy czym lepsze osobniki mają większe szanse na 
wybór. Wybrane rozwiązania są następnie modyfikowane – poprzez wymianę 
informacji genetycznej w procesie krzyżowania oraz losowe zmiany w 
procesie mutacji – co prowadzi do powstania nowej populacji. Algorytm 
iteruje ten proces, dążąc do znalezienia rozwiązań o jak najwyższej jakości. 
Działanie algorytmu kończy się po osiągnięciu określonego kryterium, np. 
ustalonej liczby iteracji, odpowiedniego poziomu jakości rozwiązania lub 
osiągnieciu limitu czasu.

### Uruchomienie programu
1. Sklonuj repozytorium:
    ```sh
   git clone https://github.com/Tesa44/GeneticAlgorithm.git
2. Otwórz projekt w CLion
3. Skonfiguruj kompilator
4. Zbuduj projekt
5. Uruchom program

## Dokumentacja
Pełny przebieg badania znajduje się w pliku `PEA-projekt-3.pdf`

## Introduction
The Asymmetric Traveling Salesman Problem (ATSP) is one of the key optimization challenges in graph theory and operations research. The task involves determining the shortest possible route that allows a salesman to visit all given cities exactly once and return to the starting point, with distances between cities varying depending on the direction of travel (asymmetry). ATSP has wide applications in logistics, route planning, production optimization, and urban transportation systems. Due to its computational complexity (the problem belongs to the NP-hard class), solving ATSP requires specialized algorithms, such as exhaustive search, branch and bound, or dynamic programming. The goal of this study is to evaluate the efficiency of selected algorithms used to solve ATSP by analyzing their results in terms of solution quality.

## Genetic Algorithm
The Genetic Algorithm is a metaheuristic optimization method inspired by evolutionary processes. It iteratively searches for the best solution to a problem by simulating mechanisms such as selection, crossover, and mutation. The process begins with the random generation of a population of potential solutions, each evaluated using a fitness function that determines its quality. Based on fitness values, solutions are selected for the next generation (selection), with better individuals having a higher probability of being chosen. The selected solutions are then modified—by exchanging genetic information in the crossover process and introducing random changes through mutation—leading to the creation of a new population. The algorithm iterates this process, striving to find solutions of the highest possible quality. The execution of the algorithm ends when a predefined criterion is met, such as a set number of iterations, a satisfactory solution quality level, or a time limit.

### Startup
1. Clone the repo
   ```sh
   git clone https://github.com/Tesa44/GeneticAlgorithm.git
2. Open project in CLion
3. Configure compiler
4. Build the project
5. Run main.cpp

## Documentation
The full research process is documented in the file `PEA-projekt-3.pdf`

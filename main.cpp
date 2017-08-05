#include <iostream>

#include "field.hpp"
#include "types.hpp"

int main(int argc, char **argv) {
    bool gameOver = false;
    Field field(Difficulty::Easy());

    while (!gameOver) {
        field.PrintData();
        printf("\n");
        field.Print();

        int x, y;
        std::cout << "x: ";
        std::cin >> x;
        std::cout << "y: ";
        std::cin >> y;

        if (!field.TouchPosition(Position(x, y))) {
            std::cout << "Hit bomb! Game Over!" << std::endl;
            break;
        }
    }

    return 0;
}

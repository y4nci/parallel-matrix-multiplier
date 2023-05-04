#include "Inputs.h"
#include "Matrix.h"
#include <iostream>

#define MATRIX_COUNT 4

int main() {
    std::vector<Matrix> matrices;

    getInputs(matrices);

    for (int matrix = 0; matrix < MATRIX_COUNT; matrix++) {
        std::cout << "Matrix " << matrix + 1 << ":" << std::endl;
        for (int row = 0; row < matrices[matrix].getRowCount(); row++) {
            for (int column = 0; column < matrices[matrix].getColumnCount(); column++) {
                std::cout << matrices[matrix][row][column] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    return 0;
}

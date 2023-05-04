#include "Inputs.h"

#include <iostream>

/**
 * @brief This function gets input from stdin, parses and stores the data in the matrices array.
 */
void getInputs(std::vector<Matrix>& matrices) {
    for (int matrix = 0; matrix < 4; matrix++) {
        MatrixValues values;
        int rowCount, columnCount;

        std::cin >> rowCount >> columnCount;

        for (int r = 0; r < rowCount; r++) {
            std::vector<int> row;
            for (int c = 0; c < columnCount; c++) {
                int value;
                std::cin >> value;
                row.push_back(value);
            }
            values.push_back(row);
        }

        matrices.push_back(Matrix(rowCount, columnCount, values));
    }
}

#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

typedef std::vector<std::vector<int>> MatrixValues;

class Matrix {
private:
    /**
     * @brief the row count of the matrix.
     */
    int rowCount;

    /**
     * @brief the column count of the matrix.
     */
    int columnCount;

    /**
     * @brief the values of the matrix.
     */
    MatrixValues matrixValues;
public:
    Matrix();
    Matrix(int row, int column, MatrixValues values);
    ~Matrix();

    // Getters
    int getRowCount() const;
    int getColumnCount() const;
    MatrixValues getMatrixValues() const;
    std::vector<int> getRow(int row) const;
    std::vector<int> getColumn(int column) const;
    std::vector<int>& operator[](int);

    // Setters
    void setRowCount(int row);
    void setColumnCount(int column);
    void setMatrixValues(MatrixValues values);
    void setRow(int rowIndex, std::vector<int> values);
    void setColumn(int columnIndex, std::vector<int> values);
};

#endif

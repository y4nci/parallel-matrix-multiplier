#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

typedef std::vector<std::vector<int>> MatrixValues;

class Matrix {
private:
    /**
     * @brief the row count of the matrix.
     */
    unsigned rowCount;

    /**
     * @brief the column count of the matrix.
     */
    unsigned columnCount;

    /**
     * @brief the values of the matrix.
     */
    MatrixValues matrixValues;
public:
    Matrix();
    Matrix(unsigned row, unsigned column, MatrixValues values);
    ~Matrix();

    // Getters
    unsigned getRowCount() const;
    unsigned getColumnCount() const;
    MatrixValues getMatrixValues() const;
    std::vector<int> getRow(unsigned row) const;
    std::vector<int> getColumn(unsigned column) const;
    std::vector<int>& operator[](unsigned index);

    // Setters
    void setRowCount(unsigned row);
    void setColumnCount(unsigned column);
    void setMatrixValues(MatrixValues values);
    void setRow(unsigned rowIndex, std::vector<int> values);
    void setColumn(unsigned columnIndex, std::vector<int> values);
};

#endif

#include "Matrix.h"

Matrix::Matrix() {
    this->rowCount = 0;
    this->columnCount = 0;
    this->matrixValues = {};
}

Matrix::Matrix(int row, int column, MatrixValues values) {
    this->rowCount = row;
    this->columnCount = column;
    this->matrixValues = values;
}

Matrix::~Matrix() {
    this->matrixValues.clear();
}

int Matrix::getRowCount() const {
    return this->rowCount;
}

int Matrix::getColumnCount() const {
    return this->columnCount;
}

MatrixValues Matrix::getMatrixValues() const {
    return this->matrixValues;
}

std::vector<int> Matrix::getRow(int row) const {
    return this->matrixValues[row];
}

std::vector<int> Matrix::getColumn(int column) const {
    std::vector<int> columnValues;

    for (int i = 0; i < this->rowCount; i++) {
        columnValues.push_back(this->matrixValues[i][column]);
    }

    return columnValues;
}

std::vector<int>& Matrix::operator[](int index) {
    return this->matrixValues[index];
}

void Matrix::setRowCount(int row) {
    this->rowCount = row;
}

void Matrix::setColumnCount(int column) {
    this->columnCount = column;
}

void Matrix::setMatrixValues(MatrixValues values) {
    this->matrixValues = values;
}

void Matrix::setRow(int rowIndex, std::vector<int> values) {
    this->matrixValues[rowIndex] = values;
}

void Matrix::setColumn(int columnIndex, std::vector<int> values) {
    for (int i = 0; i < this->rowCount; i++) {
        this->matrixValues[i][columnIndex] = values[i];
    }
}

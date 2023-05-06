#include "Matrix.h"

Matrix::Matrix() {
    this->rowCount = 0;
    this->columnCount = 0;
    this->matrixValues = {};
}

Matrix::Matrix(unsigned int row, unsigned int column, int value) {
    this->rowCount = row;
    this->columnCount = column;
    this->matrixValues = {};

    for (int r = 0; r < row; r++) {
        std::vector<int> rowValues;
        for (int c = 0; c < column; c++) {
            rowValues.push_back(value);
        }
        this->matrixValues.push_back(rowValues);
    }
}

Matrix::Matrix(unsigned row, unsigned column, MatrixValues values) {
    this->rowCount = row;
    this->columnCount = column;
    this->matrixValues = values;
}

Matrix::~Matrix() {
    this->matrixValues.clear();
}

unsigned Matrix::getRowCount() const {
    return this->rowCount;
}

unsigned Matrix::getColumnCount() const {
    return this->columnCount;
}

MatrixValues& Matrix::getMatrixValues() {
    return this->matrixValues;
}

std::vector<int> Matrix::getRow(unsigned row) const {
    return this->matrixValues[row];
}

std::vector<int> Matrix::getColumn(unsigned column) const {
    std::vector<int> columnValues;

    for (int i = 0; i < this->rowCount; i++) {
        columnValues.push_back(this->matrixValues[i][column]);
    }

    return columnValues;
}

void Matrix::setRowCount(unsigned row) {
    this->rowCount = row;
}

void Matrix::setColumnCount(unsigned column) {
    this->columnCount = column;
}

void Matrix::setMatrixValues(MatrixValues values) {
    this->matrixValues = values;
}

void Matrix::setRow(unsigned rowIndex, std::vector<int> values) {
    this->matrixValues[rowIndex] = values;
}

void Matrix::setColumn(unsigned columnIndex, std::vector<int> values) {
    for (int i = 0; i < this->rowCount; i++) {
        this->matrixValues[i][columnIndex] = values[i];
    }
}

void Matrix::set(unsigned rowIndex, unsigned columnIndex, int value) {
    this->matrixValues[rowIndex][columnIndex] = value;
}

std::vector<int>& Matrix::operator[](unsigned index) {
    return this->matrixValues[index];
}

Matrix Matrix::operator=(const Matrix& matrix) {
    this->rowCount = matrix.rowCount;
    this->columnCount = matrix.columnCount;
    this->matrixValues = matrix.matrixValues;

    return *this;
}

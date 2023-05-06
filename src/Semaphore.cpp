#include "Semaphore.h"

// TODO: wait for the add to finish before multiplying that row

void* addRows(void* args) {
    Args* arguments = (Args*) args;
    sem_t* semaphore = arguments->semaphore;
    int rowNumber = arguments->rowNumber;
    std::pair<Matrix*, Matrix*> inputMatrices = arguments->inputMatrices;
    Matrix* outputMatrix = arguments->outputMatrix;

    sem_wait(semaphore);
    std::vector<int> row1 = inputMatrices.first->getRow(rowNumber);
    std::vector<int> row2 = inputMatrices.second->getRow(rowNumber);

    for (int i = 0; i < row1.size(); i++) {
        outputMatrix->getMatrixValues()[rowNumber][i] = row1[i] + row2[i];
    }
    sem_post(semaphore);

    pthread_exit(NULL);
}

void* multiplyRowWithColumn(void* args) {
    Args* arguments = (Args*) args;
    sem_t* semaphore = arguments->semaphore;
    int rowNumber = arguments->rowNumber;
    std::pair<Matrix*, Matrix*> inputMatrices = arguments->inputMatrices;
    Matrix* outputMatrix = arguments->outputMatrix;

    sem_wait(semaphore);
    std::vector<int> row = inputMatrices.first->getRow(rowNumber);
    std::vector<int> column = inputMatrices.second->getColumn(rowNumber);

    for (int i = 0; i < row.size(); i++) {
        outputMatrix->getMatrixValues()[rowNumber][i] = row[i] * column[i];
    }
    sem_post(semaphore);

    pthread_exit(NULL);
}

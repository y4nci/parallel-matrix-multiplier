#include "Semaphore.h"

void* addRows(void* args) {
    AdderArgs* arguments = (AdderArgs*) args;
    sem_t* semaphore = arguments->semaphore;
    int rowNumber = arguments->rowNumber;
    std::pair<Matrix*, Matrix*> inputMatrices = arguments->inputMatrices;
    Matrix* outputMatrix = arguments->outputMatrix;
    WrittenInfoMatrix* writtenInfoMatrix = arguments->writtenInfoMatrix;

    /* TODO : rewrite
    sem_wait(semaphore);
    std::vector<int> row1 = inputMatrices.first->getRow(rowNumber);
    std::vector<int> row2 = inputMatrices.second->getRow(rowNumber);

    for (int i = 0; i < row1.size(); i++) {
        outputMatrix->getMatrixValues()[rowNumber][i] = row1[i] + row2[i];
    }
    sem_post(semaphore);
     */

    pthread_exit(NULL);
}

void* multiplyRowWithColumn(void* args) {
    MultiplierArgs * arguments = (MultiplierArgs*) args;
    sem_t* multiplierSemaphore = arguments->multiplierSemaphore;
    sem_t* rowColumnSemaphore = arguments->rowColumnSemaphore;
    int rowNumber = arguments->rowNumber;
    std::pair<Matrix*, Matrix*> inputMatrices = arguments->inputMatrices;
    Matrix* outputMatrix = arguments->outputMatrix;
    WrittenInfoMatrix* writtenInfoMatrix1_2 = arguments->writtenSumsInfoMatrices.first;
    WrittenInfoMatrix* writtenInfoMatrix3_4 = arguments->writtenSumsInfoMatrices.second;

    /*
     * TODO: rewrite
     *
    sem_wait(semaphore);
    std::vector<int> row = inputMatrices.first->getRow(rowNumber);
    std::vector<int> column = inputMatrices.second->getColumn(rowNumber);

    for (int i = 0; i < row.size(); i++) {
        outputMatrix->getMatrixValues()[rowNumber][i] = row[i] * column[i];
    }
    sem_post(semaphore);
     */

    pthread_exit(NULL);
}

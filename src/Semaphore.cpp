#include "Semaphore.h"

void* addRows(void* args) {
    SemArgs * arguments = (SemArgs *) args;

    SemType semType = arguments->semType;
    int rowNumber = arguments->rowNumber;
    std::pair<Matrix*, Matrix*> inputMatrices = arguments->inputMatrices;
    Matrix* outputMatrix = arguments->outputMatrix;

    SemaphoreArray semaphoreArray = semType == ADDER1_2
            ? S1_2
            : S3_4;

    std::vector<int> row1 = inputMatrices.first->getRow(rowNumber);
    std::vector<int> row2 = inputMatrices.second->getRow(rowNumber);

    for (int i = 0; i < row1.size(); i++) {
        outputMatrix->getMatrixValues()[rowNumber][i] = row1[i] + row2[i];
        sem_post(semaphoreArray[rowNumber][i]);
    }

    pthread_exit(NULL);
}

void* multiplyRowWithColumn(void* args) {
    SemArgs * arguments = (SemArgs *) args;

    int rowNumber = arguments->rowNumber;
    std::pair<Matrix*, Matrix*> inputMatrices = arguments->inputMatrices;
    Matrix* outputMatrix = arguments->outputMatrix;

    unsigned // N = inputMatrices.first->getRowCount(),
        M = inputMatrices.first->getColumnCount(),
        K = inputMatrices.second->getColumnCount();

    for (int k = 0; k < K; k++) {
        for (int m = 0; m < M; m++) {
            sem_wait(S1_2[rowNumber][k + m]);
            sem_wait(S3_4[rowNumber + m][k]);

            outputMatrix->getMatrixValues()[rowNumber][k] +=
                    inputMatrices.first->getMatrixValues()[rowNumber][m] *
                    inputMatrices.second->getMatrixValues()[m][k];
        }
    }

    pthread_exit(NULL);
}

void initialiseSemaphoreArray(char *key, SemaphoreArray& semaphoreArray, unsigned R, unsigned C) {
    for (int i = 0; i < R; i++) {
        std::vector<sem_t *> row;
        for (int j = 0; j < C; j++) {
            char *semaphoreName = new char[32];
            sem_t *semaphore;

            snprintf(semaphoreName, 32, "%s_%d_%d", key, i, j);

            sem_unlink(semaphoreName);

            semaphore = sem_open(semaphoreName, O_CREAT, 0644, 1);

            row.push_back(semaphore);

            delete [] semaphoreName;
        }
        semaphoreArray.push_back(row);
    }
}

void destroySemaphoreArray(SemaphoreArray& semaphoreArray, unsigned R, unsigned C) {
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            sem_close(semaphoreArray[i][j]);
        }
    }
}

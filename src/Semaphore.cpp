#include "Semaphore.h"

void* addRows(void* args) {
    SemArgs * arguments = (SemArgs *) args;

    // also acts as matrix_id for hw2_write_output
    SemType semType = arguments->semType;
    int rowNumber = arguments->rowNumber;
    std::pair<Matrix*, Matrix*> inputMatrices = arguments->inputMatrices;
    Matrix* outputMatrix = arguments->outputMatrix;

    SemaphoreArray semaphoreArray = semType == ADDER1_2
            ? S1_2
            : S3_4;

    std::vector<int> row1 = inputMatrices.first->getRow(rowNumber);
    std::vector<int> row2 = inputMatrices.second->getRow(rowNumber);

    unsigned N = inputMatrices.first->getRowCount();

    for (size_t i = 0; i < row1.size(); i++) {
        outputMatrix->getMatrixValues()[rowNumber][i] = row1[i] + row2[i];

        // output
        hw2_write_output(semType, rowNumber, i, row1[i] + row2[i]);

        for (unsigned n = 0; n < N; n++)
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

    for (unsigned k = 0; k < K; k++) {
        for (unsigned m = 0; m < M; m++) {
            sem_wait(S1_2[rowNumber][m]);
            sem_wait(S3_4[m][k]);

            outputMatrix->getMatrixValues()[rowNumber][k] +=
                    inputMatrices.first->getMatrixValues()[rowNumber][m] *
                    inputMatrices.second->getMatrixValues()[m][k];
        }

        // output
        hw2_write_output(MULTIPLIER, rowNumber, k, outputMatrix->getMatrixValues()[rowNumber][k]);
    }

    pthread_exit(NULL);
}

void initialiseSemaphoreArray(char *key, SemaphoreArray& semaphoreArray, unsigned R, unsigned C) {
    for (unsigned i = 0; i < R; i++) {
        std::vector<sem_t *> row;
        for (unsigned j = 0; j < C; j++) {
            char *semaphoreName = new char[32];
            sem_t *semaphore;

            snprintf(semaphoreName, 32, "%s_%d_%d", key, i, j);

            sem_unlink(semaphoreName);

            semaphore = sem_open(semaphoreName, O_CREAT, 0644, 0);

            row.push_back(semaphore);

            delete [] semaphoreName;
        }
        semaphoreArray.push_back(row);
    }
}

void destroySemaphoreArray(SemaphoreArray& semaphoreArray, unsigned R, unsigned C) {
    for (unsigned i = 0; i < R; i++) {
        for (unsigned j = 0; j < C; j++) {
            sem_close(semaphoreArray[i][j]);
        }
    }
}

#include "Semaphore.h"

void* addRows1_2(void* args) {
    /**
     * in add1_2 threads, we generate a row. we don't need to check if a new row is made ready.
     * however, if there is an unprocessed column, the multiplier thread should be informed,
     * so that it can process the existing ready column with the new row.
     */

    bool signalMultiplier = false;
    AdderArgs* arguments = (AdderArgs*) args;
    sem_t* adderSemaphore = adder1_2semaphore;
    int rowNumber = arguments->rowNumber;
    std::pair<Matrix*, Matrix*> inputMatrices = arguments->inputMatrices;
    Matrix* outputMatrix = arguments->outputMatrix;
    CellInfoMatrix* cellInfoMatrix1_2 = arguments->cellInfoMatrices.first,
                  * cellInfoMatrix3_4 = arguments->cellInfoMatrices.second;


    ////////////////////////////
    sem_wait(adderSemaphore);
    ////////////////////////////

    std::vector<int> row1 = inputMatrices.first->getRow(rowNumber);
    std::vector<int> row2 = inputMatrices.second->getRow(rowNumber);

    for (int i = 0; i < row1.size(); i++) {
        outputMatrix->getMatrixValues()[rowNumber][i] = row1[i] + row2[i];
        cellInfoMatrix1_2->at(rowNumber)[i] = READY;
    }

    /**
     * we only have to check for ready columns in 3_4, because we already know that we
     * just completed a row in 1_2. so, we don't need to check for ready rows in 1_2.
     */
    signalMultiplier = checkForReadyColumns(cellInfoMatrix3_4);

    if (signalMultiplier) {
        // send signal to multiplier thread, it will take care of the rest
        sem_post(rowColumnSemaphore);

        // wait for multiplier thread to finish
        sem_wait(multiplierSemaphore);
    }

    ////////////////////////////
    sem_post(adderSemaphore);
    ////////////////////////////

    pthread_exit(NULL);
}

void* addRows3_4(void* args) {
    /**
     * in add3_4 threads, we generate a row again. however, adder3_4 threads generate the matrix
     * which will be used column by column by the multiplier thread. so, if we filled a column,
     * we need to check if there is an unprocessed row, and if there is, we need to inform the
     * multiplier thread. so that it can multiply the new column with the existing ready row.
     * anyway, the only thread which can generate new columns in adder3_4 is the last thread.
     */

    bool signalMultiplier = false;
    AdderArgs* arguments = (AdderArgs*) args;
    sem_t* adderSemaphore = adder3_4semaphore;
    int rowNumber = arguments->rowNumber;
    std::pair<Matrix*, Matrix*> inputMatrices = arguments->inputMatrices;
    Matrix* outputMatrix = arguments->outputMatrix;
    CellInfoMatrix* cellInfoMatrix3_4 = arguments->cellInfoMatrices.second,
                  * cellInfoMatrix1_2 = arguments->cellInfoMatrices.first;


    ////////////////////////////
    sem_wait(adderSemaphore);
    ////////////////////////////

    std::vector<int> row1 = inputMatrices.first->getRow(rowNumber);
    std::vector<int> row2 = inputMatrices.second->getRow(rowNumber);

    for (int i = 0; i < row1.size(); i++) {
        outputMatrix->getMatrixValues()[rowNumber][i] = row1[i] + row2[i];
        cellInfoMatrix3_4->at(rowNumber)[i] = READY;

        /**
         * unlike addRows1_2, we have to check for both ready columns in 3_4
         * and ready rows in 1_2, and we have to do it at every iteration.
         * the reason is, we can create new columns ready for multiplication
         * at every iteration, so we have to check for them.
         */
        signalMultiplier = checkForReadyRows(cellInfoMatrix1_2)
                && checkForReadyColumns(cellInfoMatrix3_4);

        if (signalMultiplier) {
            // send signal to multiplier thread, it will take care of the rest
            sem_post(rowColumnSemaphore);

            // wait for multiplier thread to finish
            sem_wait(multiplierSemaphore);
        }
    }

    ////////////////////////////
    sem_post(adderSemaphore);
    ////////////////////////////

    pthread_exit(NULL);
}

void* multiplyRowWithColumn(void* args) {
    MultiplierArgs * arguments = (MultiplierArgs*) args;
    std::pair<Matrix*, Matrix*> inputMatrices = arguments->inputMatrices;
    Matrix* outputMatrix = arguments->outputMatrix;
    CellInfoMatrix* writtenInfoMatrix1_2 = arguments->cellInfoMatrices.first;
    CellInfoMatrix* writtenInfoMatrix3_4 = arguments->cellInfoMatrices.second;

    // wait until a new row and column is ready
    sem_wait(rowColumnSemaphore);

    // TODO: detect which row and column is ready

    // multiply the row with the column

    // write the result to the output matrix

    // mark the cell as written

    // signal the adder thread

    pthread_exit(NULL);
}


// utils


bool checkForReadyColumns(CellInfoMatrix* cellInfoMatrix) {
    CellInfoMatrix cellInfoMatrix1_2 = *cellInfoMatrix;
    int columnCount = cellInfoMatrix1_2[0].size();

    for (int c = 0; c < columnCount; c++) {
        bool allProcessed = true;

        for (int r = 0; r < cellInfoMatrix1_2.size(); r++) {
            if (cellInfoMatrix1_2[r][c] == UNREADY) {
                break;
            } else if (cellInfoMatrix1_2[r][c] == READY) {
                allProcessed = false;
            }
        }

        if (!allProcessed) return true;
    }

    return false;
}

bool checkForReadyRows(CellInfoMatrix* cellInfoMatrix) {
    CellInfoMatrix cellInfoMatrix3_4 = *cellInfoMatrix;

    for (int r = 0; r < cellInfoMatrix3_4.size(); r++) {
        bool allProcessed = true;

        for (int c = 0; c < cellInfoMatrix3_4[r].size(); c++) {
            if (cellInfoMatrix3_4[r][c] == UNREADY) {
                break;
            } else if (cellInfoMatrix3_4[r][c] == READY) {
                allProcessed = false;
            }
        }

        if (!allProcessed) return true;
    }

    return false;
}

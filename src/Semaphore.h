#ifndef HW2_SEMAPHORE_H
#define HW2_SEMAPHORE_H

#include "Matrix.h"

#include <sys/semaphore.h>

#define ADDER_SEM_1_2 "adder_semaphore_1_2"
#define ADDER_SEM_3_4 "adder_semaphore_3_4"
#define MULTIPLIER_SEM "multiplier_semaphore"

enum CellInfo {
    /**
     * @brief the cell is not ready to be processed.
     * The adder threads have not written to it yet.
     */
    UNREADY,

    /**
     * @brief the cell is ready to be processed.
     * The adder threads have written to it.
     */
    READY,

    /**
     * @brief the cell has been processed.
     * The multiplier threads have written to it.
     */
    PROCESSED,
};

/**
 * @par adder1_2semaphore
 * is the semaphore to be used by the adder threads for matrices 1 and 2.
 * there should be only one adder thread for matrices 1 and 2 writing to the output matrix.
 * ACTS AS A MUTEX.
 * @par adder3_4semaphore
 * is the semaphore to be used by the adder threads for matrices 3 and 4.
 * there should be only one adder thread for matrices 3 and 4 writing to the output matrix.
 * ACTS AS A MUTEX.
 * @par rowColumnSemaphore
 * is the semaphore to be used by the multiplier threads to wait for the adder threads to finish writing to
 * the row and column they should work on. sent by the adder threads.
 * @par multiplierSemaphore
 * is the semaphore to be used by the multiplier threads for sum matrices.
 * there should be only one multiplier thread writing to the output matrix.
 * ACTS AS A MUTEX.
 */
extern sem_t *adder1_2semaphore, *adder3_4semaphore, *rowColumnSemaphore, *multiplierSemaphore;

/**
 * a matrix of ints to check which cells have been cellInfoMatrices to.
 */
typedef std::vector<std::vector<CellInfo>> CellInfoMatrix;

/**
 * the arguments to be passed to the adder threads.
 */
typedef struct {
    /**
     * the row numbers of the matrices to be added
     * or the row and column numbers of the matrices to be multiplied
     */
    int rowNumber;

    /**
     * the matrices to be added or multiplied
     */
    std::pair<Matrix*, Matrix*> inputMatrices;

    /**
     * the matrix to be cellInfoMatrices to
     */
    Matrix* outputMatrix;

    /**
     * the matrices in which the cellInfoMatrices cells are marked
     */
    std::pair<CellInfoMatrix*, CellInfoMatrix*> cellInfoMatrices;
} AdderArgs;

/**
 * the arguments to be passed to the multiplier threads.
 * this struct is similar to AdderArgs, with the main difference being the row and column numbers.
 * in AdderArgs, there is a single row number, but in MultiplierArgs, there are no row numbers.
 * the row and column numbers are calculated by the multiplier threads, by looking at the
 * ready CellInfoMatrix cells in the cellInfoMatrices.
 */
typedef struct {/**
     * the matrices to be added or multiplied
     */
    std::pair<Matrix*, Matrix*> inputMatrices;

    /**
     * the matrix to be cellInfoMatrices to
     */
    Matrix* outputMatrix;

    /**
     * the matrix in which the cellInfoMatrices cells are marked by the sum threads
     */
    std::pair<CellInfoMatrix*, CellInfoMatrix*> cellInfoMatrices;
} MultiplierArgs;

// adder
void* addRows1_2(void* args);
void* addRows3_4(void* args);

// multiplier
void* multiplyRowWithColumn(void* args);

// utils

/**
 * checks whether there are processable columns on a matrix.
 *  PARTICULARLY, SUM3_4
 * @param cellInfoMatrix the matrix to check for ready columns
 * @return false if all elements are PROCESSED or there are UNREADY elements, true otherwise
 */
bool checkForReadyColumns(CellInfoMatrix* cellInfoMatrix);

/**
 * checks whether there are processable rows on a matrix
 *  PARTICULARLY, SUM1_2
 * @param cellInfoMatrix the matrix to check for ready rows
 * @return false if all elements are PROCESSED or there are UNREADY elements, true otherwise
 */
bool checkForReadyRows(CellInfoMatrix* cellInfoMatrix);

#endif //HW2_SEMAPHORE_H

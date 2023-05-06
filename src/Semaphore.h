#ifndef HW2_SEMAPHORE_H
#define HW2_SEMAPHORE_H

#include "Matrix.h"

#include <sys/semaphore.h>

#define ADDER_SEM_1_2 "adder_semaphore_1_2"
#define ADDER_SEM_3_4 "adder_semaphore_3_4"
#define MULTIPLIER_SEM "multiplier_semaphore"

/**
 * a matrix of booleans to check which cells have been writtenInfoMatrix to.
 */
typedef std::vector<std::vector<bool>> WrittenInfoMatrix;

/**
 * the arguments to be passed to the adder threads.
 */
typedef struct {
    /**
     * the pointer to sem_t
     */
    sem_t* semaphore;

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
     * the matrix to be writtenInfoMatrix to
     */
    Matrix* outputMatrix;

    /**
     * the matrix in which the writtenInfoMatrix cells are marked
     */
    WrittenInfoMatrix* writtenInfoMatrix;
} AdderArgs;

/**
 * the arguments to be passed to the multiplier threads.
 * this struct is similar to AdderArgs, the main difference is the writtenSumsInfoMatrices.
 * It is similar to the writtenInfoMatrix field in AdderArgs, but it is a pair of WrittenInfoMatrix
 * instead of a single WrittenInfoMatrix. We use that field to check which cells are written
 * by the sum threads. So, we read from it, instead of writing to it.
 */
typedef struct {
    /**
     * the pointer to rowColumnSemaphore sem_t
     */
    sem_t* rowColumnSemaphore;

    /**
     * the pointer to Multiplier sem_t
     */
    sem_t* multiplierSemaphore;

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
     * the matrix to be writtenInfoMatrix to
     */
    Matrix* outputMatrix;

    /**
     * the matrix in which the writtenInfoMatrix cells are marked by the sum threads
     */
    std::pair<WrittenInfoMatrix*, WrittenInfoMatrix*> writtenSumsInfoMatrices;
} MultiplierArgs;

// adder
void* addRows(void* args);

// multiplier
void* multiplyRowWithColumn(void* args);

#endif //HW2_SEMAPHORE_H

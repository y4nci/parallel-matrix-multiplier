#ifndef HW2_SEMAPHORE_H
#define HW2_SEMAPHORE_H

#include "Matrix.h"

#include <semaphore.h>
#include <pthread.h>
#include <sys/fcntl.h>

enum SemType {
    ADDER1_2, ADDER3_4, MULTIPLIER
};

/**
 * the argument struct to be passed to the adder and multiplier threads.
 */
typedef struct {
    /**
     * the semaphore type of the thread.
     */
    SemType semType;

    /**
     * the row and column numbers of the matrices to be multiplied.
     */
    unsigned rowNumber;

    /**
     * the matrices to be added or multiplied
     */
    std::pair<Matrix*, Matrix*> inputMatrices;

    /**
     * the matrix to be cellInfoMatrices to
     */
    Matrix* outputMatrix;
} SemArgs;

typedef std::vector<std::vector<sem_t *>> SemaphoreArray;

extern SemaphoreArray S1_2, S3_4;

// adder
void* addRows(void* args);

// multiplier
void* multiplyRowWithColumn(void* args);

// utils
void initialiseSemaphoreArray(char *key, SemaphoreArray& semaphoreArray, unsigned R, unsigned C);
void destroySemaphoreArray(SemaphoreArray& semaphoreArray, unsigned R, unsigned C);

#endif //HW2_SEMAPHORE_H

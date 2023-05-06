#ifndef HW2_SEMAPHORE_H
#define HW2_SEMAPHORE_H

#include "Matrix.h"

#include <sys/semaphore.h>

#define ADDER_SEM_1_2 "adder_semaphore_1_2"
#define ADDER_SEM_3_4 "adder_semaphore_3_4"
#define MULTIPLIER_SEM "multiplier_semaphore"

typedef struct {
    sem_t* semaphore;
    int rowNumber;  // also the corresponding column number when multiplying
    std::pair<Matrix*, Matrix*> inputMatrices;
    Matrix* outputMatrix;
} Args;

// adder
void* addRows(void* args);

// multiplier
void* multiplyRowWithColumn(void* args);

#endif //HW2_SEMAPHORE_H

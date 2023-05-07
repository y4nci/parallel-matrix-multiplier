#include "Inputs.h"
#include "Matrix.h"
#include "Semaphore.h"

#include <pthread.h>
#include <iostream>

SemaphoreArray S1_2, S3_4;

int main() {
    std::vector<Matrix> matrices;
    Matrix sum1_2, sum3_4, prod;
    pthread_t *summers1_2, *summers3_4, *multipliers;
    SemArgs *args1_2, *args3_4, *argsProd;

    // see pdf
    unsigned N, M, K;

    char S1_2key[] = "S1_2", S3_4key[] = "S3_4";

    getInputs(matrices);

    N = matrices[0].getRowCount();
    M = matrices[0].getColumnCount(); // or matrices[2].getRowCount()
    K = matrices[2].getColumnCount();

    sum1_2 = Matrix(N, M, 0);
    sum3_4 = Matrix(M, K, 0);
    prod = Matrix(N, K, 0);

    summers1_2 = new pthread_t[N];
    summers3_4 = new pthread_t[M];
    multipliers = new pthread_t[N];

    args1_2 = new SemArgs [N];
    args3_4 = new SemArgs [M];
    argsProd = new SemArgs [N];

    initialiseSemaphoreArray(S1_2key, S1_2, N, M);
    initialiseSemaphoreArray(S3_4key, S3_4, M, K);

    for (unsigned i = 0; i < N; i++) {
        args1_2[i].semType = ADDER1_2;
        args1_2[i].rowNumber = i;
        args1_2[i].inputMatrices = std::make_pair(&matrices[0], &matrices[1]);
        args1_2[i].outputMatrix = &sum1_2;
        pthread_create(&summers1_2[i], NULL, addRows, (void *) &args1_2[i]);
    }

    for (unsigned i = 0; i < M; i++) {
        args3_4[i].semType = ADDER3_4;
        args3_4[i].rowNumber = i;
        args3_4[i].inputMatrices = std::make_pair(&matrices[2], &matrices[3]);
        args3_4[i].outputMatrix = &sum3_4;
        pthread_create(&summers3_4[i], NULL, addRows, (void *) &args3_4[i]);
    }

    for (unsigned i = 0; i < N; i++) {
        argsProd[i].semType = MULTIPLIER;
        argsProd[i].rowNumber = i;
        argsProd[i].inputMatrices = std::make_pair(&sum1_2, &sum3_4);
        argsProd[i].outputMatrix = &prod;
        pthread_create(&multipliers[i], NULL, multiplyRowWithColumn, (void *) &argsProd[i]);
    }

    for (unsigned i = 0; i < N; i++) {
        pthread_join(summers1_2[i], NULL);
    }

    for (unsigned i = 0; i < M; i++) {
        pthread_join(summers3_4[i], NULL);
    }

    for (unsigned i = 0; i < N; i++) {
        pthread_join(multipliers[i], NULL);
    }

    // output
    std::cout << prod;

    delete [] summers1_2;
    delete [] summers3_4;
    delete [] multipliers;
    delete [] args1_2;
    delete [] args3_4;
    delete [] argsProd;

    destroySemaphoreArray(S1_2, N, M);
    destroySemaphoreArray(S3_4, M, K);

    return 0;
}

#include "Inputs.h"
#include "Matrix.h"
#include "Semaphore.h"

#include<pthread.h>
#include <sys/fcntl.h>

/**
 * @par adder1_2semaphore
 * is the semaphore to be used by the adder threads for matrices 1 and 2.
 * there should be only one adder thread for matrices 1 and 2 writing to the output matrix.
 * @par adder3_4semaphore
 * is the semaphore to be used by the adder threads for matrices 3 and 4.
 * there should be only one adder thread for matrices 3 and 4 writing to the output matrix.
 * @par rowColumnSemaphore
 * is the semaphore to be used by the multiplier threads to wait for the adder threads to finish.
 * the row and column they should work on.
 * @par multiplierSemaphore
 * is the semaphore to be used by the multiplier threads for sum matrices.
 * there should be only one multiplier thread writing to the output matrix.
 */
sem_t *adder1_2semaphore, *adder3_4semaphore, *rowColumnSemaphore, *multiplierSemaphore;

void initialiseWrittenInfoMatrices(std::vector<std::vector<bool>> &matrix1_2writtenRows,
                                   std::vector<std::vector<bool>> &matrix3_4writtenRows,
                                   unsigned N, unsigned M, unsigned K) {
    matrix1_2writtenRows = std::vector<std::vector<bool>>(N, std::vector<bool>(M, false));
    matrix3_4writtenRows = std::vector<std::vector<bool>>(M, std::vector<bool>(K, false));
}

int main() {
    std::vector<Matrix> matrices;
    Matrix sum1_2, sum3_4, prod;
    pthread_t *summers1_2, *summers3_4, *multipliers;
    AdderArgs *args1_2, *args3_4;
    MultiplierArgs *argsProd;

    /**
     * @par
     *  matrix1_2writtenRows[i][j] = true if the matrices[0][i][j] + matrices[1][i][j] has been writtenInfoMatrix to sum1_2
     * @par
     *  matrix3_4writtenRows[i][j] = true if the matrices[2][i][j] + matrices[3][i][j] has been writtenInfoMatrix to sum3_4
     */
    WrittenInfoMatrix matrix1_2writtenRows, matrix3_4writtenRows;

    // see pdf
    unsigned N, M, K;

    getInputs(matrices);

    N = matrices[0].getRowCount();
    M = matrices[0].getColumnCount(); // or matrices[2].getRowCount()
    K = matrices[2].getColumnCount();

    sum1_2 = Matrix(N, M, 0);
    sum3_4 = Matrix(M, K, 0);
    prod = Matrix(N, K, 0);

    initialiseWrittenInfoMatrices(matrix1_2writtenRows, matrix3_4writtenRows, N, M, K);

    summers1_2 = new pthread_t[N];
    summers3_4 = new pthread_t[M];
    multipliers = new pthread_t[N];

    args1_2 = new AdderArgs[N];
    args3_4 = new AdderArgs[M];
    argsProd = new MultiplierArgs[N];

    sem_unlink(ADDER_SEM_1_2);
    sem_unlink(ADDER_SEM_3_4);
    sem_unlink(MULTIPLIER_SEM);

    adder1_2semaphore = sem_open(ADDER_SEM_1_2, O_CREAT, 0644, 1);
    adder3_4semaphore = sem_open(ADDER_SEM_3_4, O_CREAT, 0644, 1);
    multiplierSemaphore = sem_open(MULTIPLIER_SEM, O_CREAT, 0644, 1);

    if (adder1_2semaphore == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }
    if (adder3_4semaphore == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }
    if (multiplierSemaphore == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < N; i++) {
        args1_2[i].semaphore = adder1_2semaphore;
        args1_2[i].rowNumber = i;
        args1_2[i].inputMatrices = std::make_pair(&matrices[0], &matrices[1]);
        args1_2[i].outputMatrix = &sum1_2;
        args1_2[i].writtenInfoMatrix = &matrix1_2writtenRows;
        pthread_create(&summers1_2[i], NULL, addRows, (void *) &args1_2[i]);
    }

    for (int i = 0; i < M; i++) {
        args3_4[i].semaphore = adder3_4semaphore;
        args3_4[i].rowNumber = i;
        args3_4[i].inputMatrices = std::make_pair(&matrices[2], &matrices[3]);
        args3_4[i].outputMatrix = &sum3_4;
        args3_4[i].writtenInfoMatrix = &matrix3_4writtenRows;
        pthread_create(&summers3_4[i], NULL, addRows, (void *) &args3_4[i]);
    }

    for (int i = 0; i < N; i++) {
        argsProd[i].multiplierSemaphore = multiplierSemaphore;
        argsProd[i].rowColumnSemaphore = rowColumnSemaphore;
        argsProd[i].rowNumber = i;
        argsProd[i].inputMatrices = std::make_pair(&sum1_2, &sum3_4);
        argsProd[i].outputMatrix = &prod;
        argsProd[i].writtenSumsInfoMatrices = std::make_pair(&matrix1_2writtenRows, &matrix3_4writtenRows);
        pthread_create(&multipliers[i], NULL, multiplyRowWithColumn, (void *) &argsProd[i]);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(summers1_2[i], NULL);
        pthread_join(multipliers[i], NULL);
    }

    for (int i = 0; i < M; i++) {
        pthread_join(summers3_4[i], NULL);
    }

    delete [] summers1_2;
    delete [] summers3_4;
    delete [] multipliers;
    delete [] args1_2;
    delete [] args3_4;
    delete [] argsProd;

    sem_close(adder1_2semaphore);
    sem_close(adder3_4semaphore);
    sem_close(multiplierSemaphore);
    sem_close(rowColumnSemaphore);

    return 0;
}

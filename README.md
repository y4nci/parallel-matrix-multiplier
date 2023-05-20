# parallel-matrix-multiplier

This is a program that performs three operations on four matrices using threads without losing the
integrity of the values. The program takes four matrices as inputs. First two matrices and the last
two matrices are added. The result of the first addition is multiplied by the result of the second.
These operations are completed by threads. Each thread is responsible for one row of results on all
matrices (first two additions and final result). Created for CENG334 HW2.

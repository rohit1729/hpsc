hpsc
====

hspcode.cc -- contains the parallel code for matrix multiplication which is done by parallelizing recursive(strassen) method of matrix multiplication.

Matseq.cc-- contains serial implementation of matrix multiplication using strassen method.

execution time for Size N - 1024 
Simple matrix multiplication - 27 seconds
Strassen matrix multiplication (serial) - 4 seconds
Strassen matrix multiplication (parallel) - 2.13 seconds

execution time for Size N - 512
Simple matrix multiplication - 1.14 seconds
Strassen matrix multiplication (serial) - 0.6 seconds
Strassen matrix multiplication (parallel) - 0.13 seconds

About Machine:
quad-core 2.5GHz i5 intel processor






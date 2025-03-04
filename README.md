# openMP
Various Algorithms & Problems implemented using openMP in C

# List of Problems available : 
1) Algo.txt : 
- Dining Philosophers
- Producer Consumer
- Readers-writers
- Sleeping Barber
- Banker's algorithm

- Few other algorithms implemented : Longest Common Subsequence, N-Queens Problem, Mandelbrot, Pseudo Random Number Generator, DFS
------------------------------------------------
2) Matrix .txt : 
- Matrix multiplication
- Parallel Matrix Determinant Calculation
- Parallel Convolution of Two Matrices 
- Matrix transposition
- Trace Calculation
------------------------------------------------
3) Sort.txt
- quick sort
- insertion sort
- radix sort
- bubble sort
- heap sort
- merge sort
------------------------------------------------
4) FFT.txt : Fast Fourier Transform Using OpenMP
5) Pi.txt : Calculate Value of Pi (Monte Carlo Algorithm)
6) Logic.txt : Implement Different logic gates using OpenMP
------------------------------------------------

# List of OpenMP commands (Basics):
- int id = omp_get_thread_num();
- int total_threads = omp_get_num_threads();
- int max_threads = omp_get_max_threads();
- int processors = omp_get_num_procs();
- To record time for execution :
    double start = omp_get_wtime();
    double end = omp_get_wtime();
    printf("Time: %f seconds\n", end - start);
  
- double precision = omp_get_wtick();
- export OMP_NUM_THREADS=4

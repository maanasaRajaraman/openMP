//bcast
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    int number;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) { 
        number = 100;
        printf("Processor 0 broadcasting number: %d\n", number);
 
        for (int i = 1; i < size; i++) {
            MPI_Send(&number, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } 
    else { 
        MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Processor %d received number: %d\n", rank, number);
    }

    MPI_Finalize();
    return 0;
}

//scatter
#include <mpi.h>
#include <stdio.h>

#define N 8  // Total elements (should be divisible by number of processes)

int main(int argc, char** argv) {
    int rank, size;
    int data[N] = {10, 20, 30, 40, 50, 60, 70, 80};  
    int recv_data[2]; // Each processor will receive 2 elements (N/size)

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int elements_per_proc = N / size;

    if (rank == 0) { 
        for (int i = 1; i < size; i++) {
            MPI_Send(&data[i * elements_per_proc], elements_per_proc, MPI_INT, i, 0, MPI_COMM_WORLD);
        } 
        for (int i = 0; i < elements_per_proc; i++) {
            recv_data[i] = data[i];
        }
    } 
    else { 
        MPI_Recv(recv_data, elements_per_proc, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    printf("Processor %d received: %d %d\n", rank, recv_data[0], recv_data[1]);

    MPI_Finalize();
    return 0;
}

//gather
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    int value, gathered_data[10];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
 
    value = rank * 10;

    if (rank == 0) { 
        gathered_data[0] = value;  
        for (int i = 1; i < size; i++) {
            MPI_Recv(&gathered_data[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        
        printf("Processor 0 gathered data: ");
        for (int i = 0; i < size; i++) {
            printf("%d ", gathered_data[i]);
        }
        printf("\n");
    } 
    else { 
        MPI_Send(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}

//reduce
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    int value, total_sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
 
    value = rank * 5;

    if (rank == 0) {
        total_sum = value; 
        int temp;
        for (int i = 1; i < size; i++) {
            MPI_Recv(&temp, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_sum += temp;
        }
        printf("Total Sum (MPI_Reduce manually): %d\n", total_sum);
    } 
    else { 
        MPI_Send(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}

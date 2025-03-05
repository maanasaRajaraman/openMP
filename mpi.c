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

-----------------------------------------------------
// word count
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_FILE_SIZE 10000  // Maximum file size (adjust as needed)

int count_words(char* text, int length) {
    int count = 0;
    int in_word = 0;

    for (int i = 0; i < length; i++) {
        if (isspace(text[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            count++;
        }
    }
    return count;
}

int main(int argc, char** argv) {
    int rank, size;
    char text[MAX_FILE_SIZE];
    int local_count = 0, total_count = 0;
    int chunk_size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) { 
        FILE* file = fopen("text.txt", "r");
        if (!file) {
            printf("Error: Unable to open file!\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        fread(text, sizeof(char), MAX_FILE_SIZE, file);
        fclose(file);
 
        int text_length = strlen(text);
        chunk_size = text_length / size;
    }
 
    MPI_Bcast(&chunk_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
 
    char* local_text = (char*)malloc((chunk_size + 1) * sizeof(char)); 
    MPI_Scatter(text, chunk_size, MPI_CHAR, local_text, chunk_size, MPI_CHAR, 0, MPI_COMM_WORLD);
    local_text[chunk_size] = '\0';  
    local_count = count_words(local_text, chunk_size);
 
    MPI_Reduce(&local_count, &total_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Total Word Count: %d\n", total_count);
    }

    free(local_text);
    MPI_Finalize();
    return 0;
}

-----------------------------------------------------
// sort
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
 
void merge(int *array, int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        L[i] = array[left + i];
    for (j = 0; j < n2; j++)
        R[j] = array[mid + 1 + j];

    i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            array[k++] = L[i++];
        else
            array[k++] = R[j++];
    }

    while (i < n1)
        array[k++] = L[i++];
    while (j < n2)
        array[k++] = R[j++];

    free(L);
    free(R);
} 
void quicksort(int *array, int left, int right) {
    if (left < right) {
        int pivot = array[right];
        int i = left - 1;
        for (int j = left; j < right; j++) {
            if (array[j] < pivot) {
                i++;
                int temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
        int temp = array[i + 1];
        array[i + 1] = array[right];
        array[right] = temp;
        int pi = i + 1;

        quicksort(array, left, pi - 1);
        quicksort(array, pi + 1, right);
    }
}

int main(int argc, char *argv[]) {
    int rank, size;
    int n = 16;
    int data[16] = {9, 3, 2, 7, 5, 8, 10, 1, 6, 4, 15, 12, 11, 13, 16, 14};
    int local_n;
    int *local_data;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    local_n = n / size;
    local_data = (int *)malloc(local_n * sizeof(int));
 
    MPI_Scatter(data, local_n, MPI_INT, local_data, local_n, MPI_INT, 0, MPI_COMM_WORLD); 
    quicksort(local_data, 0, local_n - 1);
 
    MPI_Gather(local_data, local_n, MPI_INT, data, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) { 
        for (int i = 1; i < size; i++) {
            merge(data, 0, (i * local_n) - 1, ((i + 1) * local_n) - 1);
        }
        printf("Sorted Array: ");
        for (int i = 0; i < n; i++) {
            printf("%d ", data[i]);
        }
        printf("\n");
    }

    free(local_data);
    MPI_Finalize();
    return 0;
}

-----------------------------------------------------

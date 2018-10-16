# Introduction to Distributed-Memory Programming using MPI

## Exercise 1

The data distribution that is used in the given `mm-mpi.c` is distributed blockwise among processes.  Advantage apart from the trivialness of this method of distribution, distributing blockwise is better than doing it in cyclic manner. Because there is no good reason that we need a cyclic distribution, because the distribution is very fair where there isn't any "easy block" and "hard block" there is not even any branching in matrix multiplication such that there is no case that would harness the advantages of cyclic distribution. And the waiting part of cyclic will slow down the execution of the program.

Although the blockwise distribution is better than cyclic, doing it with checquerboard distribution will be more efficient. In the current implementation to solve for a column, master process had to send the whole matrix B to the slave. chequerboard distribution is doing this more efficiently because to solve a block you don't need to send the whole matrix B to the other process but just the column. this reducing our problem of bottleneck in networking and message passing. and this method of chequerboard is objectively superior to the row-based blockwise distribution.
## Exercise 2

To make it easier and more elegant to code I'll be doing it instead of multiplying  $A \times B$ my multiplication method will solve $A \times B^T$ The implementation is on my [github](https://github.com/plty/nus/blob/master/enrolled/parallel-CS3210/labs/lab-3/mm-mpi-cq.c) and in the end of this report

## Exercise 3

My implementation work as long as the requested number of process is dividing the matrix fairly as in $\sqrt{x} \mid size$ and the original `mm-mpi.c` only can distribute among $x = 2^k$ slaves. s.t. we can only benchmark this fairly and objectively by dividing the problem into 4, 16 or 64 processes and with the size of the matrix as 2048. I don't know why in the lab I can't use more than 3 computers concurrently with MPI s.t. I can only benchmark it by sharing it among 4 and 16 processes, this data is acquired from the Xeon Workstation.

| Impl. | Slaves | Time | Comm
|:--|--:|:--|:--
| Chequerboard | 5 | 10.382 | 0.32 |
| Blockwise | 5 | 30.097 | 0.64 |
| Chequerboard | 16 | 5.205 | 1.00 |
| Blockwise | 16 | 11.494 | 2.83 |

From this we can deduce that chequerboard distribution is better than blockwise and the result is quite significant, we don't have to compare computation time as the complexity and the number of operation needed is the same accross implementation

### Appendix.
```cpp
/**
 * CS3210 - Matrix Multiplication in MPI
 **/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <mpi.h>
#include <math.h>
#include <string.h>

#define MATRIX_A   (1 << 27)
#define MATRIX_B   (1 << 26)
#define MATRIX_RES (1 << 25)

int size;
int slaves;
int myid;
int chunk_size;

long long comm_time = 0;
long long comp_time = 0;

#define MASTER_ID slaves

typedef struct {
    float **element;
} matrix;


/**
 * Get square root.
 */
int square_root(int x) {
    return (int) sqrt(1.0 * x);
}

/**
 * Get current time.
 **/
long long wall_clock_time() {
#ifdef __linux__
    struct timespec tp;
    clock_gettime(CLOCK_REALTIME, &tp);
    return (long long)(tp.tv_nsec + (long long)tp.tv_sec * 1000000000ll);
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)(tv.tv_usec * 1000 + (long long)tv.tv_sec * 1000000000ll);
#endif
}

/**
 * Allocates memory for a matrix of size SIZE
 * The memory is allocated row-major order, i.e.
 *  elements from the same row are allocated at contiguous
 *  memory addresses.
 **/
void allocate_matrix(matrix* m) {
    // allocate array for all the rows
    m->element = (float**) malloc(sizeof(float*) * size);
    if (m->element == NULL) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }

    // allocate an array for each row of the matrix
    for (int i = 0; i < size; i++) {
        m->element[i] = (float*) malloc(sizeof(float) * size);
        if (m->element[i] == NULL) {
            fprintf(stderr, "Out of memory\n");
            exit(1);
        }
    }
}

/**
 * Initializes the elements of the matrix with
 * random values between 0 and 9
 **/
void init_matrix_random(matrix m) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            m.element[i][j] = rand() % 10;
}

/**
 * Initializes the elements of the matrix with
 * element 0.
 **/
void init_matrix_zero(matrix m) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            m.element[i][j] = 0.0;
}

/**
 * Prints the matrix
 **/
void print_matrix(matrix m) {
    for (int i = 0; i < size; i++) {
        printf("row =%4d: ", i);
        for (int j = 0; j < size; j++)
            printf("%6.2f  ", m.element[i][j]);
        printf("\n");
    }
}


/**
 * Transpose the matrix
 */
void transpose_matrix(matrix m) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int temp = m.element[i][j];
            m.element[i][j] = m.element[j][i];
            m.element[j][i] = temp;
        }
    }
}

/*******************************************************************************/

/**
 * Function used by the slaves to receive data from the master
 * Each slave receives the entire B matrix
 * and a number of rows from the A matrix
 **/
void slave_receive_data(float a[][size], float b[][size]) {
    int i;
    MPI_Status status;

    long long before = wall_clock_time();

    for (i = 0; i < chunk_size; i++) {
        MPI_Recv(&a[i], size, MPI_FLOAT, MASTER_ID, MATRIX_A | i, MPI_COMM_WORLD, &status);
        MPI_Recv(&b[i], size, MPI_FLOAT, MASTER_ID, MATRIX_B | i, MPI_COMM_WORLD, &status);
    }
    fprintf(stderr," --- SLAVE %d: Received row [%d-%d] of matrix A\n", myid, myid * chunk_size,myid * chunk_size + chunk_size - 1);
    fprintf(stderr," --- SLAVE %d: Received row [%d-%d] of matrix B\n", myid, myid * chunk_size,myid * chunk_size + chunk_size - 1);
    long long after = wall_clock_time();
    comm_time += after - before;
}


/**
 * Function used by the slaves to compute the product
 * result = A x B
 **/
void slave_compute(float a[][size], float b[][size], float result[][chunk_size]) {
    long long before = wall_clock_time();
    for (int i = 0; i < chunk_size; i++)
        for (int j = 0; j < chunk_size; j++)
            result[i][j] = 0;

    for (int i = 0; i < chunk_size; i++)
        for (int j = 0; j < chunk_size; j++)
            for (int k = 0; k < size; k++)
                result[i][j] += a[i][k] * b[j][k];

    long long after = wall_clock_time();
    comp_time += after - before;

    fprintf(stderr," --- SLAVE %d: Finished the computations\n", myid);
}

/**
 * Function used by the slaves to send the product matrix
 * back to the master
 **/
void slave_send_result(float result[][chunk_size]) {
    long long before = wall_clock_time();
    for (int i = 0; i < chunk_size; i++) {
        MPI_Send(&result[i], chunk_size, MPI_FLOAT, MASTER_ID, MATRIX_RES | i, MPI_COMM_WORLD);
    }
    long long after = wall_clock_time();
    comm_time += after - before;
    fprintf(stderr," --- SLAVE %d: Sent the results back\n", myid);
}


/**
 * Main function called by slaves
 *
 **/
void slave() {
    int rows_per_slave = size / slaves ;

    float row_a[chunk_size][size];
    float row_b[chunk_size][size];

    float result[chunk_size][chunk_size];
    //float result[1024][2048];

    // Receive data
    slave_receive_data(row_a, row_b);

    // Doing the computations
    slave_compute(row_a, row_b, result);

    // Sending the results back
    slave_send_result(result);

    fprintf(stderr, " --- SLAVE %d: communication_time=%6.2f seconds; computation_time=%6.2f seconds\n", myid, comm_time / 1000000000.0, comp_time / 1000000000.0);
}


/*******************************************************************************/

/**
 * Function called by the master to distribute
 * rows from matrix A among different slaves
 * and the entire matrix B to each slave
 **/
void master_distribute(matrix a, matrix b) {
    int chunk = square_root(slaves);
    fprintf(stderr," +++ MASTER : Distributing matrix to slaves\n");
    for (int i = 0; i < chunk; i++) {
        for (int j = 0; j < chunk; j++) {
            int slave_id = i * chunk + j;
            int row_a = i * chunk_size;
            int row_b = j * chunk_size;
            for (int k = 0; k < chunk_size; k++) {
                MPI_Send(a.element[row_a + k], size, MPI_FLOAT, slave_id, MATRIX_A | k, MPI_COMM_WORLD);
                MPI_Send(b.element[row_b + k], size, MPI_FLOAT, slave_id, MATRIX_B | k, MPI_COMM_WORLD);
            }
            fprintf(stderr," +++ MASTER : Finished sending row [%d-%d] of matrix A to process %d\n",
                    row_a, row_a + chunk_size, slave_id);
            fprintf(stderr," +++ MASTER : Finished sending row [%d-%d] of matrix B to process %d\n",
                    row_b, row_b + chunk_size, slave_id);
        }
    }
}

/**
 * Receives the result matrix from the slaves
 * row by row and assembles it into the
 * object @result
 **/
void master_receive_result(matrix result) {
    MPI_Status status;
    fprintf(stderr," +++ MASTER : Receiving the results from slaves\n");

    int chunk = square_root(slaves);
    for (int i = 0; i < chunk; i++) {
        for (int j = 0; j < chunk; j++){
            int slave_id = i * chunk + j;
            float buffer[chunk_size][chunk_size];
            int row = i, col = j * chunk_size;
            for (int k = 0; k < chunk_size; k++) {
                MPI_Recv(&result.element[row][col], chunk_size, MPI_FLOAT, slave_id, MATRIX_RES | k, MPI_COMM_WORLD, &status);
            }
        }
    }
}

/**
 * Main function called by the master process
 *
 **/
void master() {
    matrix a, b, result;

    // Allocate memory for matrices
    allocate_matrix(&a);
    allocate_matrix(&b);
    allocate_matrix(&result);

    // Initialize matrix elements
    init_matrix_random(a);
    init_matrix_random(b);
    transpose_matrix(b);

    // Distribute data to slaves
    master_distribute(a, b);

    // Gather results from slaves
    master_receive_result(result);

    // Print the result matrix
    // print_matrix(result);
}



/*******************************************************************************/

/**
 * Matrix multiplication using master-slave paradigm
 * The master initializes and sends the data to the
 * slaves. The slaves do all the computation and send
 * the product back to the master
 *
 * Matrix size must by a power of 2
 * Number of slaves must be a power of 2 and less than matrix size
 *
 * Total number of processes is 1 + number of slaves
 *
 **/
int main(int argc, char ** argv) {
    int nprocs;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    size = 2048;

    // One master and nprocs-1 slaves
    slaves = nprocs - 1;
    chunk_size = size / square_root(slaves);

    if (myid == MASTER_ID) {
        fprintf(stderr, " +++ Process %d is master\n", myid);
        master();
    } else {
        fprintf(stderr, " --- Process %d is slave\n", myid);
        slave();
    }  

    MPI_Finalize();
    return 0;
}
```

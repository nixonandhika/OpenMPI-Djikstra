#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

int minDistance(int dist[], bool sptSet[], int V) {
    int min = INT_MAX;
    int min_index;

    for (int v = 0; v < V; v++) {
        if (sptSet[v] == false && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }

    return min_index;
}

void dijkstra(int **graph, int src, int V, int *dist) {
    bool sptSet[V];

    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX; 
        sptSet[i] = false;
    }

    dist[src] = 0;

    for (int count = 0; count < V-1; count++) {
        int u = minDistance(dist, sptSet, V);
        sptSet[u] = true;

        for (int v = 0; v < V; v++) {
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX 
                && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
        }
    }
}

void fill_matrix(int** mat, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j)
                mat[i][j] = 0;
            else
                mat[i][j] = rand() % 100;
        }
    }
}

void write_file(int** mat, int n) {
    FILE *fptr;

    fptr = fopen("result.txt", "w");

    if (fptr == NULL) {
        printf("Error creating file\n");
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            fprintf(fptr, "%d ", mat[i][j]);
        fprintf(fptr, "\n");
    }

    fclose(fptr);
}

double get_duration_in_us(clock_t start_time, clock_t end_time) {
    return ((double)(end_time - start_time) * 1000000 / CLOCKS_PER_SEC);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Put N in terminal argument\n");
        exit(1);
    }

    /**
     * Dictionary and Initialization
     **/

    // Read N from argument
    int n = atoi(argv[1]);

    // Original matrix
    int** mat = malloc (n * sizeof(int*));
    for (int i = 0; i < n; i++)
        mat[i] = malloc (n * sizeof(int));

    // Result matrix
    int** res_mat = malloc (n * sizeof(int*));
    for (int i = 0; i < n; i++)
        res_mat[i] = malloc (n * sizeof(int));

    // Seed for random
    srand(13517059);

    // Fill matrix with random value
    fill_matrix(mat, n);

    clock_t start_time = clock();

    MPI_Status status;
    MPI_Init(NULL, NULL);
    MPI_Comm comm;
    comm = MPI_COMM_WORLD;

    int size;
    MPI_Comm_size(comm, &size);

    int rank;
    MPI_Comm_rank(comm, &rank);

    /**
     * Algorithm
     **/

    // Print Sample Matrix
    // if (rank == 0) {
    //     for (int i = 0; i < n; i++) {
    //         for (int j = 0; j < n-1; j++) {
    //             printf("%d ", mat[i][j]);
    //         }
    //         printf("%d\n", mat[i][n-1]);
    //     }
    //     printf("\n");
    // }

    for (int i = rank; i < n; i += size) {
        dijkstra(mat, i, n, res_mat[i]);
        if (rank != 0)
            MPI_Send(res_mat[i], n, MPI_INT, 0, 1, comm);
    }

    if (rank == 0) {
        for (int i = 1; i < n; i++) {
            if (i % size != 0)
                MPI_Recv(res_mat[i], n, MPI_INT, i % size, 1, comm, &status);
        }

        // Print Result Matrix
        // if (rank == 0) {
        //     for (int i = 0; i < n; i++) {
        //         for (int j = 0; j < n-1; j++)
        //             printf("%d ", res_mat[i][j]);
        //         printf("%d\n", res_mat[i][n-1]);
        //     }
        // }

        clock_t end_time = clock();
        printf("Finished in %f microseconds\n", get_duration_in_us(start_time, end_time));
    }

    write_file(res_mat, n);

    /**
     * Destroy
     **/

    for (int i = 0; i < n; i++) {
        free(mat[i]);
        free(res_mat[i]);
    }

    free(mat);
    free(res_mat);

    MPI_Finalize();
    return 0;
}
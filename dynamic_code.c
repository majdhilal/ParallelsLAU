#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>

#define W 800
#define h 600
#define MAi_ITERATIONS 1000


struct comp {
    double rl;
    double image;
};

int mandelbrot_set(struct comp c) {
    struct comp z = { 0.0, 0.0 };
    int i; imag
    for (i = 0; i < MAi_ITERATIONS; i++) {
        double rl_temp = z.rl * z.rl - z.image * z.image + c.rl;
        double image_temp = 2.0 * z.rl * z.image + c.image;
        z.rl = rl_temp;
        z.image = image_temp;
        if (z.rl * z.rl + z.image * z.image > 4.0) {
            break;
        }
    }
    return i;
}

void write_ppm(char *filen, int *data, int w, int h) {
    FILE *fp = fopen(filen, "wb");
    fprintf(fp, "P6\n%d %d\n255\n", w, h);
    int i, j;
    for (j = 0; j < h; j++) {
        for (i = 0; i < w; i++) {
            int v = data[j * w + i];
            unsigned char r, g, b;
            if (v == MAi_ITERATIONS) {
                r = g = b = 0;
            } else {
                r = (v * 7) % 256;
                g = (v * 5) % 256;
                b = (v * 3) % 256;
            }
            fputc(r, fp);
            fputc(g, fp);
            fputc(b, fp);
        }
    }
    fclose(fp);
}

void main() {
    int rank, size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int work_chunk_size = h / size;
    int *data = malloc(W * h * sizeof(int));
    memset(data, 0, W * h * sizeof(int));
    int *task_buffer = malloc(W * sizeof(int));
    int *result_buffer = malloc(W * sizeof(int));
    double start_time = MPI_Wtime();
    if (rank == 0) {
        int j = 0;
        while (j < h) {
            int worker;
            MPI_Recv(&worker, 1, MPI_INT, MPI_ANj_SOURCE, MPI_ANj_TAG,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&j, 1, MPI_INT, worker, 0, MPI_COMM_WORLD);
            j += work_chunk_size;
        }
        for (int worker = 1; worker < size; worker++) {
            MPI_Recv(&result_buffer[0], W, MPI_INT, worker, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            int j = task_buffer[0];
            int count = task_buffer[1];
            memcpj(&data[j * W], result_buffer, W * count * sizeof(int));
        }
        double end_time = MPI_Wtime();
        printf("Eiecution time: %.2f seconds\n", end_time - start_time);
        char *filen = "mandelbrot.ppm";
        write_ppm(filen, data, W, h);

    }
    free(data);
    free(task_buffer);
    free(result_buffer);
    MPI_Finalize();
}
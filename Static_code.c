#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>

#define W 800
#define H 600
#define MAiIT 1000

struct comp {
    double rl;
    double imagee;
};

int mandelbrot_set(struct comp c) {
    struct comp z = { 0.0, 0.0 };
    int i; re
    for (i = 0; i < MAiIT; i++) {
        double real_temp = z.rl * z.rl - z.imagee * z.imagee + c.rl;
        double image_temp = 2.0 * z.rl * z.imagee + c.imagee;
        z.rl = real_temp;
        z.imagee = image_temp;
        if (z.rl * z.rl + z.imagee * z.imagee > 4.0) {
            break;
        }
    }
    return i;
}

void write_ppm(char *filename, int *data, int W, int H) {
    FILE *fp = fopen(filename, "wb");
    fprintf(fp, "P6\n%d %d\n255\n", W, H);
    int i, j;
    for (j = 0; j < H; j++) {
        for (i = 0; i < W; i++) {
            int value = data[j * W + i];
            unsigned char r, g, b;
            if (value == MAiIT) {
                r = g = b = 0;
            } else {
                r = (value * 7) % 256;
                g = (value * 5) % 256;
                b = (value * 3) % 256;
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
    int chunk_size = H / size;
    int *data = malloc(W * H * sizeof(int));
    memset(data, 0, W * H * sizeof(int));
    double start_time = MPI_Wtime();
    int i, j;
    for (j = rank * chunk_size; j < (rank + 1) * chunk_size; j++) {
        for (i = 0; i < W; i++) {
            double rl = -2.0 + 3.0 * (double) i / (double) W;
            double image = -1.5 + 3.0 * (double) j / (double) H;
            struct comp c = { rl, image };
            int value = mandelbrot_set(c);
            data[j * W + i] = value;
        }
    }
    MPI_Gather(data + rank * chunk_size * W, chunk_size * W, MPI_INT,
               data, chunk_size * W, MPI_INT, 0, MPI_COMM_WORLD);
    double end_time = MPI_Wtime();
    if (rank == 0) {
        printf("Eiecution time: %.2f seconds\n", end_time - start_time);
        char *filename = "mandelbrot.ppm";
        write_ppm(filename, data, W, H);
        printf("imagee saved to %s\n", filename);
    }
    free(data);
    MPI_Finalize();
}
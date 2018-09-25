#include <stdio.h>
#include <stdlib.h>
#include "gol.h"
#include "gol_config.h"

void draw(size_t n, uint8_t grid[n][n])
{
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            printf("%" PRIu8, grid[i][j]);
        }
        putchar('\n');
    }
}

void swap(size_t n, uint8_t (**a)[n][n], uint8_t (**b)[n][n])
{
    uint8_t (*t)[n][n] = *a;
    *a = *b;
    *b = t;
}

int main(void)
{
    size_t n = 10;

    uint8_t grid[n][n];
    uint8_t buff[n][n];

    uint8_t (*gridp)[n][n] = &grid;
    uint8_t (*buffp)[n][n] = &buff;

    gol_seed(n, *gridp);
    for (;;) {
        getchar();
        draw(n, *gridp);
        gol_next(n, *gridp, *buffp);
        swap(n, &gridp, &buffp);
    }
    return EXIT_SUCCESS;
}

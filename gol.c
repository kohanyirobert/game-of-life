#include <stdlib.h>
#include <time.h>
#include "gol.h"

void tick_cell(size_t n,
               size_t i,
               size_t j,
               uint8_t grid[n][n],
               uint8_t buff[n][n])
{
    size_t iu = i - 1;
    size_t id = i + 1;
    size_t jl = j - 1;
    size_t jr = j + 1;

    uint8_t u = iu >= 0 ? grid[iu][j] : -1;
    uint8_t l = jl >= 0 ? grid[i][jl] : -1;
    uint8_t r = jr < n ? grid[i][jr] : -1;
    uint8_t d = id < n ? grid[id][j] : -1;

    uint8_t ul = iu >= 0 && jl >= 0 ? grid[iu][jl] : -1;
    uint8_t ur = iu >= 0 && jr < n ? grid[iu][jr] : -1;
    uint8_t dl = id < n && jl >= 0 ? grid[id][jl] : -1;
    uint8_t dr = id < n && jr < n ? grid[id][jr] : -1;

    unsigned lc = 0;
    if (u == 1) {
        ++lc;
    }
    if (l == 1) {
        ++lc;
    }
    if (r == 1) {
        ++lc;
    }
    if (d == 1) {
        ++lc;
    }
    if (ul == 1) {
        ++lc;
    }
    if (ur == 1) {
        ++lc;
    }
    if (dl == 1) {
        ++lc;
    }
    if (dr == 1) {
        ++lc;
    }

    uint8_t cell = grid[i][j];
    if (cell == 0) {
        if (lc == 3) {
            buff[i][j] = 1;
        } else {
            buff[i][j] = 0;
        }
    } else if (cell == 1) {
        if (lc < 2) {
            buff[i][j] = 0;
        } else if (lc == 2 || lc == 3) {
            buff[i][j] = 1;
        } else {
            buff[i][j] = 0;
        }
    }
}

void gol_seed(size_t n, uint8_t grid[n][n])
{
    srand(time(NULL));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            grid[i][j] = rand() % 2;
        }
    }
}

void gol_next(size_t n, uint8_t grid[n][n], uint8_t buff[n][n])
{
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            tick_cell(n, i, j, grid, buff);
        }
    }
}

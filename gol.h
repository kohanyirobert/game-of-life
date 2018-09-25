#include <inttypes.h>

#ifndef GOL_H
#define GOL_H

void gol_seed(size_t n, uint8_t grid[n][n]);
void gol_next(size_t n, uint8_t grid[n][n], uint8_t buff[n][n]);

#endif

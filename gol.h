#ifndef GOL__H
#define GOL__H

#include <stdbool.h>

typedef struct grid grid;

#define CELLSIZE 10
#define CURSORSIZE CELLSIZE / 3

#define CURSORCOLOR BLUE

// Initialize an empty grid of size n with the given margin
// Return a pointer or the created grid
// Return NULL in failure
extern grid *gol_init(size_t n, size_t margin);

// Free a grid structure
// Set the pointer to the grid to NULL
extern void gol_free(grid **g);


void gol_draw(grid *g, bool quadrillage);

void gol_put(grid *g, size_t i, size_t j);

void gol_reverse(grid *g, size_t cursorx, size_t cursory);

void gol_draw_cursor(grid *g, size_t cursorx, size_t cursory);

void gol_gen(grid *g);

void gol_clean(grid *g);

#endif
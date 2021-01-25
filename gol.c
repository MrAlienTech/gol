#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "gol.h"
#include "sg.h"

#define FUN_SUCESS 0
#define FUN_FAILURE -1

#define M(g) g->margin
#define N(g) g->n

typedef struct cell {
    bool life[2];
} cell;

struct grid {
    size_t n;
    size_t real_n;
    size_t margin;
    size_t i;
    cell **arr;
};

static bool __isValidEntry(grid *g, size_t n);

grid *gol_init(size_t n, size_t margin) {
    grid *g = malloc(sizeof(*g));
    if (g == NULL) {
        perror("malloc");
        return NULL;
    }

    g->real_n = margin + n + margin;
    g->n = n;
    g->margin = margin;
    g->i = 0;

    g->arr = malloc(g->real_n * sizeof(cell *));

    for (size_t i = 0; i < g->real_n; ++i) {
        g->arr[i] = malloc(g->real_n * sizeof(cell));
        if (g->arr == NULL) {
            perror("malloc");
            free(g);
            return NULL;
        }
    }

    for (size_t i = 0; i < g->real_n; ++i) {
        for (size_t j = 0; j < g->real_n; ++j) {
            cell *c = &(g->arr[i][j]);
            c->life[0] = false;
            c->life[1] = false;
        }
    }    

    return g;
}

void gol_free(grid **g) {
    free((*g)->arr);
    free(*g);
    *g = NULL;
}

void gol_draw(grid *g, bool quadrillage) {
    // clear window
    sg_clear();

    if (quadrillage) {
        sg_set_fgcolor(GREY);
        int len = (int)N(g) * (CELLSIZE + 1) + 1; 
        for (size_t i = 0; i <= N(g); ++i) {
            int c = (int) (i * (CELLSIZE + 1));
            // draw vertical broders
            sg_draw_line(c, 0, c, (int) len - 1);
            // draw horizontal broders
            sg_draw_line(0, c, (int) len - 1, c);
        }
    }

    // set color to WHITE
    sg_set_fgcolor(WHITE);

    for (size_t i = M(g); i < M(g) + N(g); ++i) {
        for (size_t j = M(g); j < M(g) + N(g); ++j) {
            cell *c = &(g->arr[i][j]);
            if (c->life[g->i] == false) {
                continue;
            }
            int x = (int) ((CELLSIZE + 1) * (i - M(g)) + 1);
            int y = (int) ((CELLSIZE + 1) * (j - M(g)) + 1);
            sg_fill_rectangle((int)x, (int)y, 
                    (unsigned int)CELLSIZE, (unsigned int)CELLSIZE);
        }
    }
}

void gol_reverse(grid *g, size_t i, size_t j) {
    if(!__isValidEntry(g, i) || !__isValidEntry(g, j)) {
        return;
    }
    g->arr[i][j].life[g->i] = !(g->arr[i][j].life[g->i]);
}

void gol_put(grid *g, size_t i, size_t j) {
    if(!__isValidEntry(g, i) || !__isValidEntry(g, j)) {
        return;
    }
    g->arr[i][j].life[g->i] = true;
}

void gol_kill(grid *g, size_t i, size_t j) {
    if(!__isValidEntry(g, i) || !__isValidEntry(g, j)) {
        return;
    }
    g->arr[i][j].life[g->i] = false;
}

void gol_draw_cursor(grid *g, size_t cursorx, size_t cursory) {
    if (!__isValidEntry(g, cursorx) || !__isValidEntry(g, cursory)) {
        return;
    }

    sg_set_fgcolor(CURSORCOLOR);

    int center_offset = (CELLSIZE-CURSORSIZE)/2;
    int x = (int) ((CELLSIZE + 1) * (cursorx - M(g)) + 1) + center_offset;
    int y = (int) ((CELLSIZE + 1) * (cursory - M(g)) + 1) + center_offset;
    sg_fill_rectangle(x, y, CURSORSIZE, CURSORSIZE);

    sg_set_fgcolor(WHITE);
}

static size_t __neightbors(grid *g, size_t i, size_t j, size_t gen) {
    size_t n = 0;
    n += (g->arr[i - 1][j - 1].life[gen] == true);
    n += (g->arr[i - 1][j].life[gen] == true);
    n += (g->arr[i][j - 1].life[gen] == true);
    n += (g->arr[i - 1][j + 1].life[gen] == true);
    n += (g->arr[i + 1][j - 1].life[gen] == true);
    n += (g->arr[i + 1][j].life[gen] == true);
    n += (g->arr[i][j + 1].life[gen] == true);
    n += (g->arr[i + 1][j + 1].life[gen] == true);
    return n;
}

void gol_gen(grid *g) {
    size_t oldGen = 0;
    size_t nexGen = 0;
    if (g->i == 0) {
        g->i = 1;
        oldGen = 0;
        nexGen = 1;
    } else {
        g->i = 0;
        oldGen = 1;
        nexGen = 0;
    }
    for (size_t i = 1; i < g->real_n - 1; ++i) {
        for (size_t j = 1; j < g->real_n - 1; ++j) {
            cell *c = &(g->arr[i][j]);
            size_t n = __neightbors(g, i, j, oldGen);
            if (c->life[oldGen]) {
                c->life[nexGen] = (n != 2 && n != 3 ? false : true);
            } else if (n == 3) {
                c->life[nexGen] = true;
            }
        }
    }
    for (size_t i = 0; i < g->real_n; ++i) {
        for (size_t j = 0; j < g->real_n; ++j) {
            g->arr[i][j].life[oldGen] = false;
        }
    }
}

void gol_clean(grid *g) {
    if (g == NULL) {
        return;
    }
    for (size_t i = 0; i < g->real_n; ++i) {
        for (size_t j = 0; j < g->real_n; ++j) {
            g->arr[i][j].life[0] = false;
            g->arr[i][j].life[1] = false;
        }
    }
}

static bool __isValidEntry(grid *g, size_t e) {
    if (e < M(g) || e >= M(g) + N(g)) {
        return false;
    }
    return true;
}
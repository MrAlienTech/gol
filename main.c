#include <stdlib.h>
#include <stdio.h>

#include "gol.h"
#include "sg.h"

int main(void) {

    size_t n = 80;
    size_t margin = 3; 

    grid *g = gol_init(n, margin);
    if (g == NULL) {
        fprintf(stderr, "Error initializing grid\n");
        return EXIT_FAILURE;
    }

    // Opening window
    sg_open((unsigned int)(n * (CELLSIZE + 1) + 1), 
            (unsigned int)(n * (CELLSIZE + 1) + 1), 
                BLACK, WHITE, "game of life");

    gol_put(g, 4, 4);

    // Draw the grid
    gol_draw(g, false);
    int c;
    while((c = sg_get_key()) != 'q') {
        switch (c) {
            case 'm':
                gol_draw(g, true);
                size_t cursorx = margin;
                size_t cursory = margin;
                gol_draw_cursor(g, cursorx, cursory);
                while ((c = sg_get_key()) != ENTER) {
                    switch (c) {
                        case ARROW_UP:
                            if (cursory > margin) {--cursory;}
                            break;
                        case ARROW_LEFT:
                            if (cursorx > margin) {--cursorx;}
                            break;
                        case ARROW_DOWN:
                            if (cursory < n + margin - 1) {++cursory;}
                            break;
                        case ARROW_RIGHT:
                            if (cursorx < n + margin - 1) {++cursorx;}
                            break;
                        case SPACE:
                            gol_reverse(g, cursorx, cursory);
                            break;
                        default:
                            break;
                    }
                    gol_draw(g, true);
                    gol_draw_cursor(g, cursorx, cursory);
                }
                break;
            case 'c':
                gol_clean(g);
                break;
            case ENTER:
                gol_gen(g);
                break;
            default:
                break;
        }

        gol_draw(g, false);
    }

    sg_close();
    gol_free(&g);

    return EXIT_SUCCESS;
}
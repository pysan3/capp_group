#pragma once

#include "../header.h"

void draw_doraemon(void);
void draw_LOTSO(void);
typedef enum {
    DORAEMON,
    LOTSO,
} Character;

void draw_character(Character c) {
    switch (c) {
        case DORAEMON:
            draw_doraemon();
            break;
        case LOTSO:
            draw_LOTSO();
            break;
        default:
            break;
    }
}


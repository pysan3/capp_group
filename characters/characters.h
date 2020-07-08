#pragma once

#include "../header.h"

void draw_doraemon(void);

typedef enum {
    DORAEMON,
} Character;

void draw_character(Character c) {
    switch (c) {
        case DORAEMON:
            draw_doraemon();
            break;
        default:
            break;
    }
}

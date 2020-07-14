#pragma once

typedef enum {
    DORAEMON,
    LOTSO,
    // ここ
} Character;

void draw_doraemon(void);
void draw_LOTSO(void);

// ここに追加

void draw_character(Character c);

#pragma once

typedef enum {
    DORAEMON,
    LOTSO,
    RILAKKUMA
    // ここ
} Character;

void draw_doraemon(void);
void draw_LOTSO(void);
void draw_Rilakkuma(void);
// ここに追加

void draw_character(Character c);

#pragma once

typedef enum {
    DORAEMON,
    LOTSO,
    KATANA,
    // ここ
} Character;

void draw_doraemon(void);
void draw_LOTSO(void);
void draw_KATANA(void);
// ここに追加

void draw_character(Character c);

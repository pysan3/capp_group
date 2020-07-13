#pragma once

typedef enum {
    DORAEMON,
    LOTSO,
    MIFFY,
    // ここ
} Character;

void draw_doraemon(void);
void draw_LOTSO(void);
void draw_miffy(void);
// ここに追加

void draw_character(Character c);

#pragma once

#include "../header.h"

//地面を描画する
void draw_ground(void);

// 雪だるまを描画する
// 引数:locationが雪だるまの座標
// y=0が地面とする。
void draw_snowman(Coordinate location);

// 玉を描画する
// 引数:locationが玉の中心座標
void draw_bullet(Coordinate location);

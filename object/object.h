#pragma once

#include "../header.h"

//地面を描画する
void draw_ground(void);

//エリア外の地面を描画する
void draw_outside(void);

// 雪だるまを描画する
// 引数:locationが雪だるまの座標
// y=0が地面とする。
void draw_snowman(Coordinate *location);

// 玉を描画する
// 引数:locationが玉の中心座標
void draw_bullet(Coordinate *location);

//壁を描画する
//引数locationが壁の座標および向き
void draw_wall(Coordinate *location);

//キャラを描画する
//引数cがキャラクターの種類
//引数locationがキャラクターの座標
void put_character(Character c,Coordinate *location);

void draw_string(const char string[], Coordinate *location);

void draw_string2(const char string[], Coordinate *location);

void draw_hp(Player *p);

void draw_introdutcion(void);

void put_snowmen(void);
#pragma once

#include "../header.h"

//長方形の上下にいる場合
int ishit_ud(double a, double b, double h, double w);

//長方形の左右にいる場合
int ishit_rl(double a, double b, double h, double w);

//長方形の斜めの位置にいる場合
int ishit_dig(double a, double b, double h, double w);

//新しい玉がないか確認、最後に描画と位置の更新
void update_bullets(void); 

//引数として物体の4隅の位置を受け取り受けたダメージの合計を返す、当たった玉は消去
//左上、右上、右下、左下の順に並べてポインターを渡して
//長方形でお願いします
double bullet_hit(Coordinate* corner[4]);

//次のフレームでの玉の位置を計算
void bullet_next(void);

//Bullet型を受け取り、球を生成、サーバーにも伝える
void bullet_throw(Bullet* start);

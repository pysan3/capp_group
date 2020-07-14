#pragma once

#include "../header.h"

//IDと名前を受け取ってplayerを生成する
int update_player(int id,char *name);

//指定された数分の敵を生成
void update_enemy(int index,int id);

//砲台の生成
void update_cannon();

//playerを矢印キーで動かす
void player_move();

//盾をキー操作で生み出す
//sleep分連打できないようにする。
void player_make_shield();

//ホイールで向きを決めて球を発射する
void player_make_bullet();

//playerのhpの更新
//引数分減らす
void player_hp(double hp);

//敵の動きを更新
//ランダムで動かす
void player_enemy_move();

//固定砲台
//一定の確率て球をランダムな敵に発射
void player_battery();


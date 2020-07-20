#pragma once

#include "../header.h"
#include "../Wall/wall.h"
#include "../Bullet/bullet.h"
#include "../websocket/websocket.h"

//IDと名前を受け取ってplayerを生成する
void update_player();

//指定された数分の敵を生成
void update_enemy();

//砲台の生成
void update_cannon();

//playerを矢印キーで動かす
void player_move();//o

//盾をキー操作で生み出す
//sleep分連打できないようにする。
void player_make_shield();//o

//ホイールで向きを決めて球を発射する
//->ホイールはopenglだと使えないらしい、freeglっていうやつだとできるっぽい…？
void player_make_bullet(int x,int y);//o

//playerのhpの更新
//引数分減らす
void player_hp(double hp,Player player);//o

//敵の動きを更新
//ランダムで動かす
void player_enemy_move();

//固定砲台
//一定の確率て球をランダムな敵に発射
void player_cannon();

void myKeyboardFunc(unsigned char key,int x,int y);

void mySpecialUpFunc(int key,int x, int y);//o

void mySpecialFunc(int key,int x,int y);//o

void myMouseFunc(int button,int state,int x,int y);//o

void draw_player()
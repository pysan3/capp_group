#pragma once

#include "../header.h"
#include "../Wall/wall.h"
#include "../Bullet/bullet.h"
#include "../websocket/websocket.h"
#include "../object/object.h"

//fieldinfoを得る
void player_fieldget(FieldInfo *fieldinfo);

//IDと名前を受け取ってplayerを生成する
void update_player();

//playerを矢印キーで動かす
void player_move();//ok

//盾をキー操作で生み出す
//sleep分連打できないようにする。
void player_make_shield(Player *player);//ok

//ホイールで向きを決めて球を発射する
//->ホイールはopenglだと使えないらしい、freeglっていうやつだとできるっぽい…？
//クリックした場所に飛ばすよう変更
void player_make_bullet(int x,int y);//ok

//playerのhpの更新
//引数分減らす
void player_hp(double hp,Player *player);//ok

//敵の動きを更新
//ランダムで動かす
void player_enemy_move();

//固定砲台
//一定の確率て球をランダムな敵に発射
void player_cannon();//o

void myKeyboardFunc(unsigned char key,int x,int y);

void mySpecialUpFunc(int key,int x, int y);

void mySpecialFunc(unsigned char key,int x,int y);

void myMouseFunc(int button,int state,int x,int y);

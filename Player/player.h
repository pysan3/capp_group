#pragma once

#include "../header.h"

//fieldinfoを得る
void player_fieldget(FieldInfo *fieldinfo);

void player_init(Player *p, double x, double z, char name[], Character c);

//IDと名前を受け取ってplayerを生成する
int update_player(void);

void player_hitPlace(Player *p, Coordinate c[], double radius);

int update_enemy_offline(int enemy_id);

void update_snowman(Player *p);

//playerを矢印キーで動かす
void player_move();//ok

//盾をキー操作で生み出す
//sleep分連打できないようにする。
void player_make_shield(Player *player, double rotate);//ok

//ホイールで向きを決めて球を発射する
//->ホイールはopenglだと使えないらしい、freeglっていうやつだとできるっぽい…？
//クリックした場所に飛ばすよう変更
void player_make_bullet(Player *p, Coordinate *direction);//ok

//playerのhpの更新
//引数分減らす
void player_hp(double hp,Player *player);//ok

//敵の動きを更新
//ランダムで動かす
void player_random_move(Player *p);

//固定砲台
//一定の確率て球をランダムな敵に発射
void player_cannon(Player *p);//o

void myKeyboardFunc(unsigned char key,int x,int y);

void myKeyboardUpFunc(unsigned char key,int x,int y);

void myMouseFunc(int button,int state,int x,int y);

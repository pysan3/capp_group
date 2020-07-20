#include "player.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<pthread.h>

#define M_PI 3.14159265358979323846

int mySpecialValue1 = 0;
int mySpecialValue2 = 0;
int mySpecialValue3 = 0;
int mySpecialValue4 = 0;

FieldInfo *fieldinfo;


void player_move(int value)//プレイヤーの移動
{
	if (mySpecialValue1 == 1 << 0){
		((fieldinfo->me->location.z)--);
		fieldinfo->me->location.rotate=M_PI/2;
	}
	if (mySpecialValue2 == 1 << 1){
		((fieldinfo->me->location.x)--);
		fieldinfo->me->location.rotate=M_PI;
	}
	if (mySpecialValue3 == 1 << 2){
		((fieldinfo->me->location.x)++);
		fieldinfo->me->location.rotate=0;
	}
	if (mySpecialValue4 == 1 << 3)((fieldinfo->me->location.z)++);

	if (mySpecialValue1)glutTimerFunc(50, player_move, 0);
	if (mySpecialValue2)glutTimerFunc(50, player_move, 0);
	if (mySpecialValue3)glutTimerFunc(50, player_move, 0);
	if (mySpecialValue4)glutTimerFunc(50, player_move, 0);
}

void mySpcialFunc(unsigned char key,int x,int y)//特殊キーが押された時
{   
    if (!mySpecialValue1)glutTimerFunc(50, player_move, 0);
	if (!mySpecialValue2)glutTimerFunc(50, player_move, 0);
	if (!mySpecialValue3)glutTimerFunc(50, player_move, 0);
	if (!mySpecialValue4)glutTimerFunc(50, player_move, 0);
    switch (key)
	{
	case GLUT_KEY_UP://上矢印キー
		mySpecialValue1 |= 1 << 0;
		break;
	case GLUT_KEY_LEFT://左矢印キー
		mySpecialValue2 |= 1 << 1;
		break;
	case GLUT_KEY_RIGHT://右矢印キー
		mySpecialValue3 |= 1 << 2;
		break;
	case GLUT_KEY_DOWN://下矢印キー
		mySpecialValue4 |= 1 << 3;
		break;
	default:
		break;
	}
}

void mySpcialUpFunc(int key, int x, int y)//特殊キーが上げられた時
{
	switch (key)
	{
	case GLUT_KEY_UP:
		mySpecialValue1 &= ~(1 << 0);
		break;
	case GLUT_KEY_LEFT:
		mySpecialValue2 &= ~(1 << 1);
		break;
	case GLUT_KEY_RIGHT:
		mySpecialValue3 &= ~(1 << 2);
		break;
	case GLUT_KEY_DOWN:
		mySpecialValue4 &= ~(1 << 3);
		break;
    
	default:
		break;
	}

}

void myKeyboardFunc(unsigned char key,int x,int y)
{
    switch(key)
    {
    case ' ':
        if(((fieldinfo->me->sleep_wall)==0)&&((fieldinfo->me->sleep_bullet)==0)){
            player_make_shield();
            fieldinfo->me->sleep_wall=1;
        }
		break;
	default:
		break;
	}	
}

void player_make_shield(){
	Coordinate location=fieldinfo->me->location;
	Wall wall={location,10,3,fieldinfo->me->id};//新しく壁を生成
	wall_make(&wall);//壁側に投げる
    fieldinfo->me->sleep_wall=0;//再び作れるようにする
}

void myMouseFunc(int button,int state,int x,int y){//とりあえずタップ
	if((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		if(fieldinfo->me->sleep_bullet==0 && fieldinfo->me->sleep_wall==0){
			player_make_bullet(x,y);
			fieldinfo->me->sleep_bullet=1;
		}
	}
}

void player_make_bullet(int x,int y){
	Coordinate location = fieldinfo->me->location;
	Coordinate velocity = {x,10,y};
	Bullet bullet = {location,velocity,0,fieldinfo->me->id};//弾の生成
	bullet_throw(&bullet);//弾側に投げる
	fieldinfo->me->sleep_bullet=0;
}

void player_hp(double hp,Player *player){//指定された分hpを減らす
	player->hp-=hp;
}

void player_enemy_move(Player enemy){
	strand(1);//乱数の初期化
	int num = rand() % 6 + 1;//上下左右弾壁
	switch (num){
		case 1:

			break;
		case 2:

			break;
		case 3:

			break;
		case 4:

			break;
		case 5:

			break;
		case 6:

			break;
	}
}

void player_cannon(){

}

void update_cannon(){

}

void update_enemy(int index,int id){

}

void update_player(){

}


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

void player_fieldget(FieldInfo *field){
	fieldinfo=field;
	printf("get");
}

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
	if (mySpecialValue4 == 1 << 3){
		((fieldinfo->me->location.z)++);
		fieldinfo->me->location.rotate=3*M_PI/2;
	}

	if (mySpecialValue1)glutTimerFunc(50, player_move, 0);
	if (mySpecialValue2)glutTimerFunc(50, player_move, 0);
	if (mySpecialValue3)glutTimerFunc(50, player_move, 0);
	if (mySpecialValue4)glutTimerFunc(50, player_move, 0);
	printf("(%lf,%lf,%lf,%lf)\n",fieldinfo->me->location.x,fieldinfo->me->location.y,fieldinfo->me->location.z,fieldinfo->me->location.rotate);
}

void mySpecialFunc(unsigned char key,int x,int y)//特殊キーが押された時
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

void mySpecialUpFunc(int key, int x, int y)//特殊キーが上げられた時
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
			fieldinfo->me->sleep_wall=1;
            player_make_shield(fieldinfo->me);
        }
		break;
	default:
		break;
	}	
}

void player_make_shield(Player *player){
	Coordinate location=player->location;
	Wall wall={location,10,3,player->id};//新しく壁を生成
	wall_make(&wall);//壁側に投げる
	printf("shield\n");
    player->sleep_wall=0;//再び作れるようにする
}

void myMouseFunc(int button,int state,int x,int y){//とりあえずタップ
	if((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		if(fieldinfo->me->sleep_bullet==0 && fieldinfo->me->sleep_wall==0){
			fieldinfo->me->sleep_bullet=1;
			player_make_bullet(x,y);
		}
	}
}

void player_make_bullet(int x,int y){
	Coordinate location = fieldinfo->me->location;
	Coordinate velocity = {x,10,y,fieldinfo->me->location.rotate};
	Bullet bullet = {location,velocity,0,fieldinfo->me->id};//弾の生成
	printf("bullet(%d,%d)\n",x,y);
	bullet_throw(&bullet);//弾側に投げる
	fieldinfo->me->sleep_bullet=0;
}

void player_hp(double hp,Player *player){//指定された分hpを減らす
	player->hp-=hp;
}

void player_enemy_bullet(){
	Coordinate location = fieldinfo->enemies[0]->location;
	Coordinate velocity = {fieldinfo->me->location.x,fieldinfo->me->location.y,fieldinfo->me->location.z,fieldinfo->enemies[0]->location.rotate};
	Bullet bullet = {location,velocity,0,fieldinfo->enemies[0]->id};//弾の生成
	bullet_throw(&bullet);//弾側に投げる
	fieldinfo->me->sleep_bullet=0;
}

void player_enemy_move(){
	int num = rand() % 6 + 1;//上下左右弾壁
	switch (num){
		case 1://上
			((fieldinfo->enemies[0]->location.z)--);
			fieldinfo->enemies[0]->location.rotate=M_PI/2;
			break;
		case 2://下
			((fieldinfo->enemies[0]->location.x)--);
			fieldinfo->enemies[0]->location.rotate=M_PI;
			break;
		case 3://左
			((fieldinfo->enemies[0]->location.x)++);
			fieldinfo->enemies[0]->location.rotate=0;
			break;
		case 4://右
			((fieldinfo->enemies[0]->location.z)++);
			fieldinfo->enemies[0]->location.rotate=3*M_PI/2;
			break;
		case 5://壁
			fieldinfo->enemies[0]->sleep_wall=1;
			player_make_shield(fieldinfo->enemies[0]);
			break;
		case 6://弾
			fieldinfo->enemies[0]->sleep_bullet=1;
			player_enemy_bullet();
			break;
	}
	printf("%d\n",num);
	printf("(%lf,%lf,%lf,%lf)\n",fieldinfo->enemies[0]->location.x,fieldinfo->enemies[0]->location.y,fieldinfo->enemies[0]->location.z,fieldinfo->enemies[0]->location.rotate);
}

void player_cannon(){
	int num = rand() % 5 + 1;
	if(num==1){
		Coordinate location = fieldinfo->cannon[0]->location;
		Coordinate velocity = fieldinfo->enemies[0]->location;
		Bullet bullet = {location,velocity,0,fieldinfo->me->id};//弾の生成
		bullet_throw(&bullet);//弾側に投げる
		printf("makebullet1");
	}else if(num==2){
		Coordinate location = fieldinfo->cannon[1]->location;
		Coordinate velocity = fieldinfo->me->location;
		Bullet bullet = {location,velocity,0,fieldinfo->enemies[0]->id};//弾の生成
		bullet_throw(&bullet);//弾側に投げる
		printf("makebullet2");
	}
	printf("%d\n",num);
}

void update_player(){
	player_enemy_move();//敵を動かす
	put_character(fieldinfo->enemies[0]->c,&(fieldinfo->enemies[0]->location));//敵の描画
	player_hp(bullet_hit(&(fieldinfo->me->location)),fieldinfo->me);
	int i=0;
	for(i=0;i<2;i++){
		draw_snowman(&(fieldinfo->cannon[i]->location));
	}
	ws_sendPlayer(fieldinfo->me);
	put_character(fieldinfo->me->c,&(fieldinfo->me->location));
	if(fieldinfo->isOnline==0){
		if(fieldinfo->me->hp<=0){
			ws_dead(fieldinfo->me->id);
		}else if(fieldinfo->enemies[0]->hp<=0){
			ws_dead(fieldinfo->enemies[0]->id);
		}
	}else{
		ws_close(fieldinfo->me->id);
		ws_close(fieldinfo->enemies[0]->id);
	}

}


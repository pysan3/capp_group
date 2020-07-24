#include "player.h"
#include "../Wall/wall.h"
#include "../Bullet/bullet.h"
#include "../websocket/websocket.h"
#include "../object/object.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

int mySpecialValue= 0;

FieldInfo *fieldinfo;

void player_fieldget(FieldInfo *field){
	fieldinfo=field;
	printf("get");
}

double distance(double a, double b) {
	return sqrt(a * a + b * b);
}

void player_to(Player *p, int x, int z) {
	double dist = distance(x, z);
	p->location.x += x * dist;
	p->location.z += z * dist;
	if (p->location.x < 0) p->location.x = 0;
	if (p->location.z < 0) p->location.z = 0;
	if (p->location.x > FIELD_MAX_X) p->location.x = FIELD_MAX_X;
	if (p->location.z > FIELD_MAX_Z) p->location.z = FIELD_MAX_Z;
	int angle = 0;
	if (x + z < 0) angle += 4;
	angle += -abs(x) + abs(z) + 1;
	if (x + z == 0) angle += 4 + 2 * x;
	p->location.rotate = M_PI * angle / 4;
}

void player_move(int value)//プレイヤーの移動
{
	int x = 0, z = 0;
	if (mySpecialValue == 1 << 0){//右
		x = 1;
	}
	if (mySpecialValue == 1 << 1){//上
		z = -1;
	}
	if (mySpecialValue == 1 << 2){//左
		x = -1;
	}
	if (mySpecialValue == 1 << 3){//下
		z = 1;
	}
	player_to(fieldinfo->me, x, z);

	printf("(%lf,%lf,%lf,%lf)\n",fieldinfo->me->location.x,fieldinfo->me->location.y,fieldinfo->me->location.z,fieldinfo->me->location.rotate);
	if (mySpecialValue)glutTimerFunc(1000 / FPS, player_move, 0);
}

void myKeyboardUpFunc(unsigned char key, int x, int y)// キーが上げられた時
{
	switch (key)
	{
	case 'd':
		mySpecialValue &= ~(1 << 0);
		break;
	case 'w':
		mySpecialValue &= ~(1 << 1);
		break;
	case 'a':
		mySpecialValue &= ~(1 << 2);
		break;
	case 's':
		mySpecialValue &= ~(1 << 3);
		break;
	default:
		break;
	}
}

void myKeyboardFunc(unsigned char key,int x, int y)
{
	if (!mySpecialValue)glutTimerFunc(1000 / FPS, player_move, 0);
    switch (key)
	{
	case 'd'://右矢印キー
		mySpecialValue |= 1 << 0;
		break;
	case 'w'://上矢印キー
		mySpecialValue |= 1 << 1;
		break;
	case 'a'://左矢印キー
		mySpecialValue |= 1 << 2;
		break;
	case 's'://下矢印キー
		mySpecialValue |= 1 << 3;
		break;
    case ' ':
		player_make_shield(fieldinfo->me, fieldinfo->me->location.rotate);
		break;
	default:
		break;
	}
}

void copyCoordinate(Coordinate *a, Coordinate *b) {
	a->x = b->x;
	a->y = b->y;
	a->z = b->z;
	a->rotate = b->rotate;
}

void player_make_shield(Player *player, double rotate){
	if (player->sleep_wall > fieldinfo->elapsed_time) return;
	Wall *newWall = (Wall *)malloc(sizeof(Wall));
	copyCoordinate(&newWall->location, &player->location);
	// TODO: 自分から少し離す
	newWall->location.x += 5 * sin(rotate);
	newWall->location.z += 5 * cos(rotate);
	newWall->remain = 3 * FPS;
	newWall->player_id = player->id;
	wall_make(newWall);//壁側に投げる
	printf("shield\n");
    player->sleep_wall = fieldinfo->elapsed_time + 3 * FPS;//再び作れるようにする
}

void myMouseFunc(int button,int state,int x,int y){//とりあえずタップ
	if((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		Coordinate c = {x, 0, y};
		player_make_bullet(fieldinfo->me, &c);
	}
}

void player_make_bullet(Player *p, Coordinate *direction){
	if (p->sleep_bullet > fieldinfo->elapsed_time) return;
	double dist = distance(direction->x, direction->z);
	Coordinate newDirection = {
		direction->x / dist * BULLET_SPEED,
		direction->y / dist * BULLET_SPEED,
		direction->z / dist * BULLET_SPEED
	};
	Bullet *newBullet = (Bullet *)malloc(sizeof(Bullet));
	copyCoordinate(&newBullet->location, &p->location);
	copyCoordinate(&newBullet->velocity, &newDirection);
	for (int i=0; i<10; i++) {
		bullet_calcNext(newBullet);
	}
	newBullet->player_id = p->id;
	newBullet->damage = BULLET_DAMAGE;
	printf("bullet(%lf,%lf,%lf)\n",newBullet->location.x,newBullet->location.y,newBullet->location.z);
	bullet_throw(newBullet);//弾側に投げる
	p->sleep_bullet = fieldinfo->elapsed_time + 0.5 * FPS;
}

void player_hp(double hp,Player *player){//指定された分hpを減らす
	player->hp-=hp;
}

// void player_enemy_bullet(int enemy_id){
// 	Coordinate velocity = {fieldinfo->me->location.x,fieldinfo->me->location.y,fieldinfo->me->location.z,fieldinfo->enemies[0]->location.rotate};
// 	Bullet *newBullet = (Bullet *)malloc(sizeof(Bullet));
// 	copyCoordinate(&newBullet->location, &fieldinfo->enemies[enemy_id]->location);
// 	copyCoordinate(&newBullet->velocity, &velocity);
// 	bullet_throw(newBullet);//弾側に投げる
// 	fieldinfo->me->sleep_bullet=0;
// }

void player_random_move(Player *p){
	int num = rand() % 3;//上下左右弾壁
	switch (num){
		case 0://移動
			player_to(p, rand() % 3 - 1, rand() % 3 - 1);
			break;
		case 1://壁
			player_make_shield(p, p->location.rotate);
			break;
		case 2://弾
			player_make_bullet(p, &fieldinfo->me->location);
			break;
	}
	printf("%d\n",num);
	printf("(%lf,%lf,%lf,%lf)\n",p->location.x,p->location.y,p->location.z,p->location.rotate);
}

void player_cannon(Player *c){
	int num = rand() % 5 + 1;
	if(num==1){
		player_make_bullet(c, &fieldinfo->me->location);
		printf("makebullet1");
	}else if(num==2){
		player_make_bullet(c, &fieldinfo->enemies[rand() % ENEMY_NUM]->location);
		printf("makebullet2");
	}
	printf("%d\n",num);
}

void player_hitPlace(Player *p, Coordinate **c, double radius) {
	int angle = 1;
	for (int i=0; i<4; i++, angle -= 2) {
		c[i]->x = p->location.x + cos(angle / 4.0 * M_PI) * radius;
		c[i]->z = p->location.z + sin(angle / 4.0 * M_PI) * radius;
	}
}

void update_snowman(Player *p) {
	// player_random_move(p);
	player_cannon(p);
	Coordinate *c[4];
	player_hitPlace(p, c, 5);
	bullet_hit(c);
}

int update_player(){
	for (int i=0; i<100; i++) {
		update_snowman(fieldinfo->cannon[i]);
	}
	Player *me = fieldinfo->me;
	Coordinate *c[4];
	player_hitPlace(me, c, 5);
	player_hp(bullet_hit(c),me);
	ws_sendPlayer(me);
	put_character(me->c,&me->location);
	if (me->hp <= 0) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int update_enemy_offline(int enemy_id) {
	Player *enemy = fieldinfo->enemies[enemy_id];
	player_random_move(enemy);
	Coordinate *c[4];
	player_hitPlace(enemy, c, 5);
	player_hp(bullet_hit(c), enemy);
	ws_sendPlayer(enemy);
	put_character(enemy->c, &enemy->location);
	if (enemy->hp <= 0) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

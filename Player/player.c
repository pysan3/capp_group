#include "player.h"
#include "../Wall/wall.h"
#include "../Bullet/bullet.h"
#include "../websocket/websocket.h"
#include "../object/object.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

int mySpecialValue= 0;

FieldInfo *fieldinfo;

void player_fieldget(FieldInfo *field){
	fieldinfo=field;
}

void player_init(Player *p, double x, double z, char name[], Character c) {
    p->id = 0;
    p->location.x = x;
    p->location.y = 0;
    p->location.z = z;
    p->location.rotate = 0;
    p->hp = 100;
    strcpy(p->name, name);
    p->sleep_bullet = 0;
    p->sleep_wall = 0;
    p->c = c;
}

static double distance(double a, double b) {
	return sqrt(a * a + b * b);
}

void player_to(Player *p, int x, int z) {
	double dist = distance(x, z);
	if (x) p->location.x += x / dist * fieldinfo->isUpdated;
	if (z) p->location.z += z / dist * fieldinfo->isUpdated;
	if (p->location.x < 0) p->location.x = 0;
	if (p->location.z < 0) p->location.z = 0;
	if (p->location.x > FIELD_MAX_X) p->location.x = FIELD_MAX_X;
	if (p->location.z > FIELD_MAX_Z) p->location.z = FIELD_MAX_Z;
	if (!(x | z)) return;
	int angle = 0;
	if (x + z < 0) angle += 4;
	angle += -abs(x) + abs(z) + 1;
	if (x + z == 0) angle += 4 + 2 * x;
	p->location.rotate = -M_PI * angle / 4;
}

void player_move(int value)//プレイヤーの移動
{
	int x = 0, z = 0;
	if (mySpecialValue & 1 << 0){//右
		x = 1;
	}
	if (mySpecialValue & 1 << 1){//上
		z = -1;
	}
	if (mySpecialValue & 1 << 2){//左
		x = -1;
	}
	if (mySpecialValue & 1 << 3){//下
		z = 1;
	}
	player_to(fieldinfo->me, x, z);

	// printf("player move: (%lf,%lf,%lf,%lf)\n",fieldinfo->me->location.x,fieldinfo->me->location.y,fieldinfo->me->location.z,fieldinfo->me->location.rotate);
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

void enemy_make_shield(Player *player, double rotate, int iscp) {
	if (player->sleep_wall > fieldinfo->elapsed_time) return;
	Wall *newWall = (Wall *)malloc(sizeof(Wall));
	copyCoordinate(&newWall->location, &player->location);
	// TODO: 自分から少し離す
	newWall->location.x += 8 * cos(rotate);
	newWall->location.z -= 8 * sin(rotate);
	newWall->location.rotate += M_PI / 2;
	newWall->remain = 1 * FPS;
	newWall->player_id = player->id;
	wall_make(newWall);//壁側に投げる
	if (!iscp) ws_sendNewWall(player->id, newWall);
	printf("%s: shield\n", player->name);
    player->sleep_wall = fieldinfo->elapsed_time + 3 * FPS;//再び作れるようにする
}

void player_make_shield(Player *player, double rotate){
	enemy_make_shield(player, rotate, 0);
}

void myMouseFunc(int button,int state,int x,int y){//とりあえずタップ
	if((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		Coordinate c = {
			(640+(x-640.0)*((720.0-y)/574.0*0.625+1))*FIELD_MAX_X/1280.0,
			0,
			(y - 146.0) / 574.0 * FIELD_MAX_Z,
			0
		};
		printf("(%d, %d)\n", x, y);
		player_make_bullet(fieldinfo->me, &c);
	}
}

void enemy_make_bullet(Player *p, Coordinate *direction, int iscp) {
	if (p->sleep_bullet > fieldinfo->elapsed_time) return;
	double dist = distance(direction->x - p->location.x, direction->z - p->location.z);
	Coordinate newDirection = {
		(direction->x - p->location.x) / dist * BULLET_SPEED,
		(direction->y - p->location.y) / dist * BULLET_SPEED,
		(direction->z - p->location.z) / dist * BULLET_SPEED
	};
	Bullet *newBullet = (Bullet *)malloc(sizeof(Bullet));
	copyCoordinate(&newBullet->location, &p->location);
	copyCoordinate(&newBullet->velocity, &newDirection);
	for (int i=0; i<8; i++) {
		newBullet->location.x += newBullet->velocity.x;
		newBullet->location.y += newBullet->velocity.y;
		newBullet->location.z += newBullet->velocity.z;
	}
	newBullet->player_id = p->id;
	newBullet->damage = BULLET_DAMAGE;
	// printf("bullet(%lf,%lf,%lf)\n",newBullet->location.x,newBullet->location.y,newBullet->location.z);
	bullet_throw(newBullet);//弾側に投げる
	if (!iscp) ws_sendNewBullet(p->id, newBullet);
	p->sleep_bullet = fieldinfo->elapsed_time + 0.5 * FPS;
}

void player_make_bullet(Player *p, Coordinate *direction){
	enemy_make_bullet(p, direction, 0);
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
	int num = rand() % 6;//上下左右弾壁
	switch (num) {
		case 0: {
			player_make_shield(p, p->location.rotate);
			break;
		}
		case 1: {
			enemy_make_bullet(p, &fieldinfo->me->location, 1);
			break;
		}
		default: {
			player_to(p, rand() % 3 - 1, rand() % 3 - 1);
			break;
		}
	}
	// printf("random: %s:(%lf,%lf,%lf,%lf)\n",p->name,p->location.x,p->location.y,p->location.z,p->location.rotate);
}

void player_cannon(Player *c){
	int num = rand() % 5 + 1;
	if(num==1){
		player_make_bullet(c, &fieldinfo->me->location);
	}else if(num==2){
		player_make_bullet(c, &fieldinfo->enemies[rand() % ENEMY_NUM]->location);
	}
}

void player_hitPlace(Player *p, Coordinate c[], double radius) {
	int angle = 1;
	GLfloat white_[] = {1,1,1,1};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white_);
    glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
	glBegin(GL_LINE_STRIP);
	for (int i=0; i<4; i++, angle += 2) {
		c[i].x = p->location.x + cos(angle / 4.0 * M_PI) * radius;
		c[i].z = p->location.z + sin(angle / 4.0 * M_PI) * radius;
		glVertex3d(c[i].x, 5, c[i].z);
	}
	glEnd();
}

void update_snowman(Player *p) {
	// player_random_move(p);
	player_cannon(p);
	Coordinate c[4];
	player_hitPlace(p, c, 5);
	bullet_hit(c);
	draw_snowman(&p->location);
}

int update_player(){
	for (int i=0; i<100; i++) {
		if (fieldinfo->cannon[i] != NULL) {
			update_snowman(fieldinfo->cannon[i]);
		}
	}
	Player *me = fieldinfo->me;
	// player_random_move(me);
	Coordinate c[4];
	player_hitPlace(me, c, 5);
	int damage = bullet_hit(c);
	if (damage) {
		player_hp(damage,me);
		printf("you got hit, hp: %d\n", me->hp);
	}
	ws_sendPlayer(me);
	put_character(me->c,&me->location);
	draw_string(me->name, &me->location);
	if (me->hp <= 0) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int update_enemy_offline(int enemy_id) {
	Player *enemy = fieldinfo->enemies[enemy_id];
	player_random_move(enemy);
	Coordinate c[4];
	player_hitPlace(enemy, c, 5);
	double damage = bullet_hit(c);
	if (damage) {
		player_hp(damage, enemy);
		printf("%s got hit, hp: %d\n", enemy->name, enemy->hp);
	}
	pthread_t tid = ws_sendPlayer(enemy);
	pthread_join(tid, NULL);
	// printf("%s: %d, (%lf, %lf)\n", enemy->name, enemy->hp, enemy->location.x, enemy->location.z);
	if (enemy->hp <= 0) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

#include "bullet.h"
#include "../object/object.h"
#include "../websocket/websocket.h"
#include <stdio.h>
#include <math.h>

int bullet_sum = 0;
int bullet_start = 0;
Bullet **bullets = NULL;  //Bulletの配列のポインタ

void bullet_init(Bullet **b){
  bullets = b;
}

int ishit_ud(double a, double b, double r, double h, double w){
  if((0 < a && a < w) && (h-r < b && b < r))
    return 1;
  else return 0;
}

int ishit_rl(double a, double b, double r, double h, double w){
  if((-r < a && a < w+r) && (h < b && b < 0))
    return 1;
  else return 0;
}

int ishit_dig(double a, double b, double r, double h, double w){
  if(( a*a + (h-b)*(h-b) < r*r ) || ( w-a + (h-b)*(h-b) < r*r )
     || ( a*a + b*b < r*r ) || ( (w-a)*(w-a) + b*b < r*r ))
    return 1;
  else return 0;
}

double bullet_hit(Coordinate corner[4]){
  double damage = 0;

  double x1 = corner[0].x;
  double x2 = corner[1].x;
  double x3 = corner[2].x;
  double x4 = corner[3].x;
  double z1 = corner[0].z;
  double z2 = corner[1].z;
  double z3 = corner[2].z;
  double z4 = corner[3].z;

  int i;
  double a,b,r,h,w,cos_val,sin_val; //玉のx座標y座標、半径、長方形の高さと幅
  r = BULLET_RADIUS;

  for(i=bullet_start; i<bullet_sum; i++){
    if(bullets[i] == NULL) {
      if (i == bullet_start) bullet_start++;
      continue;
    }
    a = bullets[i]->location.x;
    b = bullets[i]->location.z;
    //x4,z4を原点に合わせるように移動
    x1 = x1-z4;  z1 = z1-z4;
    x2 = x2-z4;  z2 = z2-z4;
    x3 = x3-z4;  z3 = z3-z4;
    a = a-x4;    b = b-z4;
    x4 = 0;  z4 = 0;

    h = sqrt(x1*x1 + z1*z1);  //長方形の高さ
    w = sqrt(x3*x3 + z3*z3);  //長方形の幅

    cos_val = x3/w;  sin_val = z3/w;
    //(x3,z3)ベクトルと(1,0)ベクトルの角度をθとしたときのcos_val,sin_val

    a = a*cos_val - b*sin_val;
    b = a*sin_val + b*cos_val;    //原点を中心に回転移動

    if(ishit_ud( a, b, r, h, w) || ishit_rl( a, b, r, h, w) || ishit_dig( a, b, r, h, w)){
      damage += bullets[i]->damage;
      bullets[i] = NULL;
      printf("hit %d\n", i);
    }
  }
  return damage;
}

void bullet_calcNext(Bullet *b) {
    b->location.x += b->velocity.x;
    b->location.y += b->velocity.y;
    b->location.z += b->velocity.z;
}

void bullet_next(void){
  int i;
  Bullet *b;
  for( i=bullet_start; i < bullet_sum; i++){
    if((b = bullets[i])==NULL) {
      if (i == bullet_start) bullet_start++;
      continue;
    }
    // printf("bullet: %lf, %lf, %lf, %lf\n", b->location.x, b->location.y, b->location.z, b->location.rotate);
    int delete = 0;
    if (b->location.x < 0) delete = 1;
    if (b->location.y < 0) delete = 1;
    if (b->location.z < 0) delete = 1;
    if (b->location.x > FIELD_MAX_X) delete = 1;
    if (b->location.y > FIELD_MAX_Y) delete = 1;
    if (b->location.z > FIELD_MAX_Z) delete = 1;
    if (delete) {
      b = NULL;
      continue;
    }
    bullet_calcNext(b);
    draw_bullet(&b->location);
  }
}

void bullet_throw(Bullet* start){
  pthread_t tid = ws_sendNewBullet(start->player_id, start);
  pthread_join(tid, NULL);
  bullets[bullet_sum] = start;
  bullet_sum++;
}

void update_bullets(int player_id){
  Bullet* new_bullet = NULL;
  while((new_bullet = ws_getNewBullet(player_id)) != NULL){
	printf("bullet(%lf,%lf,%lf)\n",new_bullet->location.x,new_bullet->location.y,new_bullet->location.z);
    bullets[bullet_sum] = new_bullet;
    bullet_sum++;
  }

  bullet_next();
}

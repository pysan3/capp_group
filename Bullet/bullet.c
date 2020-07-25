#include "bullet.h"
#include "../object/object.h"
#include "../websocket/websocket.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

int bullet_sum = 0;
int bullet_start = 0;
Bullet **bullets = NULL;  //Bulletの配列のポインタ
int *isUpdated;

void bullet_init(Bullet **b, int *updated){
  bullets = b;
  isUpdated = updated;
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

static double distance(double a, double b) {
	return sqrt(a * a + b * b);
}

static int collision(GLfloat L, GLfloat R, GLfloat T, GLfloat B, GLfloat x, GLfloat y, double radius) {
  if(L - radius > x || R + radius < x || T - radius > y || B + radius < y){//矩形の領域判定1
    return 0;
  }
  if(L > x && T > y && !((L - x) * (L - x) + (T - y) * (T - y) < radius * radius)){//左上の当たり判定
    return 0;
  }
  if(R < x && T > y && !((R - x) * (R - x) + (T - y) * (T - y) < radius * radius)){//右上の当たり判定
    return 0;
  }
  if(L > x && B < y && !((L - x) * (L - x) + (B - y) * (B - y) < radius * radius)){//左下の当たり判定
    return 0;
  }
  if(R < x && B < y && !((R - x) * (R - x) + (B - y) * (B - y) < radius * radius)){//右下の当たり判定
    return 0;
  }
  return 1;//すべての条件が外れたときに当たっている
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

  // int i;
  // double a,b,r,h,w,cos_val,sin_val; //玉のx座標y座標、半径、長方形の高さと幅
  // r = BULLET_RADIUS;

  Bullet *tmp;
  double corner_size = distance(x3-x1, z3-z1);
  double theta = atan((x3-x4)/(z3-z4));
  double rotate_sin = sin(theta);
  double rotate_cos = cos(theta);
  double box_z = rotate_cos * (z3-z1) - rotate_sin * (x3-x1);
  double box_x = rotate_sin * (z3-z1) + rotate_cos * (x3-x1);
  for(int i=bullet_start; i<bullet_sum; i++){
    if(bullets[i] == NULL) {
      if (i == bullet_start) bullet_start++;
      continue;
    }
    tmp = bullets[i];
    double bullet_z = rotate_cos * (tmp->location.z-z1) - rotate_sin * (tmp->location.x-x1);
    double bullet_x = rotate_sin * (tmp->location.z-z1) + rotate_cos * (tmp->location.x-x1);
    if (collision(box_z, 0, 0, box_x, bullet_z, bullet_x, BULLET_RADIUS)) {
      damage += tmp->damage;
      // printf("hit %d (%lf, %lf)\n", i, tmp->location.x, tmp->location.z);
      bullets[i] = NULL;
      continue;
    }
    // a = tmp->location.x;
    // b = tmp->location.z;
    // //x4,z4を原点に合わせるように移動
    // x1 = x1-z4;  z1 = z1-z4;
    // x2 = x2-z4;  z2 = z2-z4;
    // x3 = x3-z4;  z3 = z3-z4;
    // a = a-x4;    b = b-z4;
    // x4 = 0;  z4 = 0;

    // h = sqrt(x1*x1 + z1*z1);  //長方形の高さ
    // w = sqrt(x3*x3 + z3*z3);  //長方形の幅

    // cos_val = x3/w;  sin_val = z3/w;
    // //(x3,z3)ベクトルと(1,0)ベクトルの角度をθとしたときのcos_val,sin_val

    // a = a*cos_val - b*sin_val;
    // b = a*sin_val + b*cos_val;    //原点を中心に回転移動

    // if(ishit_ud( a, b, r, h, w) || ishit_rl( a, b, r, h, w) || ishit_dig( a, b, r, h, w)){
    //   damage += tmp->damage;
    //   printf("hit %d (%lf, %lf)\n", i, tmp->location.x, tmp->location.z);
    //   tmp = NULL;
    // }
  }
  return damage;
}

void bullet_calcNext(Bullet *b) {
    b->location.x += b->velocity.x * *isUpdated;
    b->location.y += b->velocity.y * *isUpdated;
    b->location.z += b->velocity.z * *isUpdated;
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
  bullets[bullet_sum] = start;
  bullet_sum++;
}

void update_bullets(int player_id){
  Bullet* new_bullet = NULL;
  while((new_bullet = ws_getNewBullet(player_id)) != NULL){
	printf("bullet(%d): (%lf,%lf,%lf)\n",bullet_sum,new_bullet->location.x,new_bullet->location.y,new_bullet->location.z);
    bullets[bullet_sum] = new_bullet;
    bullet_sum++;
  }

  bullet_next();
}

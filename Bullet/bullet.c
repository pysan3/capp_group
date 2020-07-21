#include "bullet.h"
#include "../object/object.h"
#include "../websocket/websocket.h"
#include <stdio.h>
#include <math.h>

int bullet_sum = 0;
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

double bullet_hit(Coordinate* corner[4]){
  double damage = 0;
  
  double x1 = corner[0]->x;
  double x2 = corner[1]->x;
  double x3 = corner[2]->x;
  double x4 = corner[3]->x;
  double z1 = corner[0]->z;
  double z2 = corner[1]->z;
  double z3 = corner[2]->z;
  double z4 = corner[3]->z;

  int i;
  double a,b,r,h,w,cos,sin; //玉のx座標y座標、半径、長方形の高さと幅
  r = BULLET_RADIUS;
  
  for(i=0; i<bullet_sum; i++){
    if(bullets[i] == NULL) continue;
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

    cos = x3/w;  sin = z3/w;
    //(x3,z3)ベクトルと(1,0)ベクトルの角度をθとしたときのcos,sin

    a = a*cos - b*sin;
    b = a*sin + b*cos;    //原点を中心に回転移動

    if(ishit_ud( a, b, r, h, w) || ishit_rl( a, b, r, h, w) || ishit_dig( a, b, r, h, w)){
      damage += bullets[i]->damage;
      free(bullets[i]);
      bullets[i] = NULL;
    }
  }
  return damage;
}

void bullet_next(void){
  int i;
  for( i=0; i < bullet_sum; i++){
    if(bullets[i]==NULL) continue;
    bullets[i]->location.x += bullets[i]->velocity.x;
    bullets[i]->location.y += bullets[i]->velocity.y;
    bullets[i]->location.z += bullets[i]->velocity.z;

    draw_bullet(&bullets[i]->location);
  }
}


void bullet_throw(Bullet* start){
  ws_sendNewBullet(start->player_id, start);
  bullets[bullet_sum] = start;
  bullet_sum++;
}

void update_bullets(int player_id){
  Bullet* new_bullet = NULL;
  while((new_bullet = ws_getNewBullet(player_id)) != NULL){
    bullets[bullet_sum] = new_bullet;
    bullet_sum++;
  }

  bullet_next();
}

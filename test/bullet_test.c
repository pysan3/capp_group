#include "../header.h"

#include "../Bullet/bullet.h"
#include "../object/object.h"
#include "../websocket/websocket.h"

#include <stdio.h>
#include <string.h>

void printBullet(Bullet **b){
  int i;
  for(i=0;i<10;i++){
    if(b[i]==NULL) continue;
    printf("bullet %d : ( %lf, %lf, %lf)    velocity ( %lf, %lf, %lf)\n"
	   ,i,b[i]->location.x,b[i]->location.y,b[i]->location.z
	   ,b[i]->velocity.x,b[i]->velocity.y,b[i]->velocity.z);
  }
  printf("\n");
}

int main(void){

  FieldInfo f_info;
  memset(&f_info,0,sizeof(FieldInfo));

  Player me;
  f_info.me=&me;
  Bullet b[3] = { (Bullet){{0,2,0,0},{1,0,1,0},1,0},
  		  (Bullet){{1,2,3,0},{1,0,1,0},1,0},
  		  (Bullet){{2,2,2,0},{1,0,-1,0},1,0} };
  int i;
  for(i=0;i<3;i++){
    f_info.bullets[i]=&b[i];
  }
  bullet_init(f_info.bullets);
  
  ws_init(0,&f_info.elapsed_time);
  ws_createPlayer(f_info.me, &f_info.me->id);
  Coordinate corner[4] = { {1,4,4,0},
			   {5,4,2,0},
			   {4,4,0,0},
			   {0,4,2,0} };
  Coordinate* c[4];
  for(i=0;i<4;i++){
    c[i]=&corner[i];
  }
  printBullet(f_info.bullets);
  bullet_next();
  printBullet(f_info.bullets);
  double x = bullet_hit(c);
  printf("damage %lf\n", x);
  printBullet(f_info.bullets);
  Bullet new = (Bullet){{3,2,5,0},{0,0,-1,0},2,0};
  bullet_throw(&new);
  printBullet(f_info.bullets);
  bullet_next();
  printBullet(f_info.bullets);
  return 0;
}

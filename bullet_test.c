#include "header.h"

#include "/Bullet/bullet.h"
#include "/object/object.h"
#include "/websocket/websocket.h"

#include <stdio.h>

int main(void){

  Bullet b[3] = { {{0,0,0,0},{1,1,1,0},1,100},
		  {{1,2,3,0},{1,1,1,0},1,100},
		  {{2,2,2,0},{1,-1,1,0},1,100} }

  bullet_init(&b);
  Coordinate corner[4] = { {1,4,5,0},
			   {5,4,4,0},
			   {4,4,0,0},
			   {0,4,1,0} };
  
  printf("%lf", bullet_hit(&corner));
  return 0;
}

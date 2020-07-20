#include "wall.h"
#include "../object/object.h"
#include "../websocket/websocket.h"
#include "../Bullet/bullet.h"

#include <stdio.h>
#include <math.h>

Wall **walls = NULL;
int wall_count=0;

void wall_init(Wall** wall){
    walls=wall;
    return 0;
}

/*corner[4]={
    {(walls->location.x+1)*cos(walls->location.rotate),walls->location.y,(walls->location.z-3)*sin(walls->location.rotate),walls->location.rotate},
    {(walls->location.x-1)*cos(walls->location.rotate),walls->location.y,(walls->location.z-3)*sin(walls->location.rotate),walls->location.rotate},
    {(walls->location.x-1)*cos(walls->location.rotate),walls->location.y,(walls->location.z+3)*sin(walls->location.rotate),walls->location.rotate},
    {(walls->location.x+1)*cos(walls->location.rotate),walls->location.y,(walls->location.z+3)*sin(walls->location.rotate),walls->location.rotate}

    corner[0]->x = (walls->location.x+1)*cos(walls->location.rotate)
    corner[1]->x = (walls->location.x-1)*cos(walls->location.rotate)
    corner[2]->x = (walls->location.x-1)*cos(walls->location.rotate)
    corner[3]->x = (walls->location.x+1)*cos(walls->location.rotate)
    corner[0]->z = (walls->location.z-3)*sin(walls->location.rotate)
    corner[1]->z = (walls->location.z-3)*sin(walls->location.rotate)
    corner[2]->z = (walls->location.z+3)*sin(walls->location.rotate)
    corner[3]->z = (walls->location.z+3)*sin(walls->location.rotate)
};*/


void updata_wall(int player_id){
    Wall *new_wall = NULL;
    while((new_wall = ws_getNewWall(player_id)) != NULL){
        walls[wall_count] = new_wall;
        wall_count++;
    }

    for(int i=0;i<wall_count;i++){
        walls[i]->remain = walls[i]->remain-1;
        if(walls[i]->remain<=0) free(walls[i]);
        bullet_hit(walls[i]->corner[4]);
        draw_wall(walls[i]->location);
    }
}


void wall_make(Wall* start){
    ws_sendNewWall(start->player_id, start);
    walls[wall_count] = start;
    wall_count++;
}

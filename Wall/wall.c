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
}

void wall_corner(int i, Coordinate **corner){
    corner[0]->x = (walls[i]->location.x+1)*cos(walls[i]->location.rotate);
    corner[1]->x = (walls[i]->location.x-1)*cos(walls[i]->location.rotate);
    corner[2]->x = (walls[i]->location.x-1)*cos(walls[i]->location.rotate);
    corner[3]->x = (walls[i]->location.x+1)*cos(walls[i]->location.rotate);
    corner[0]->z = (walls[i]->location.z-3)*sin(walls[i]->location.rotate);
    corner[1]->z = (walls[i]->location.z-3)*sin(walls[i]->location.rotate);
    corner[2]->z = (walls[i]->location.z+3)*sin(walls[i]->location.rotate);
    corner[3]->z = (walls[i]->location.z+3)*sin(walls[i]->location.rotate);
}

void updata_wall(int player_id){
    Wall *new_wall = NULL;
    while((new_wall = ws_getNewWall(player_id)) != NULL){
        walls[wall_count] = new_wall;
        wall_count++;
    }
    Coordinate c[4];
    Coordinate *corner[4];
    for (int i=0; i<4; i++) {
        corner[i] = &c[i];
    }

    for(int i=0;i<wall_count;i++){
        walls[i]->remain--;
        if(walls[i]->remain<=0) {
            free(walls[i]);
            continue;
        }
        wall_corner(i, corner);
        bullet_hit(corner);
        draw_wall(&walls[i]->location);
    }
}

void wall_make(Wall* start){
    ws_sendNewWall(start->player_id, start);
    walls[wall_count] = start;
    wall_count++;
}

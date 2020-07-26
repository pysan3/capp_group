#include "wall.h"
#include "../object/object.h"
#include "../websocket/websocket.h"
#include "../Bullet/bullet.h"

#include <stdio.h>
#include <math.h>

Wall **walls = NULL;
int *isUpdated;
int wall_count=0;
int wall_start=0;

void wall_init(Wall** wall, int *updated){
    walls=wall;
    isUpdated = updated;
}

void wall_corner(int i, Coordinate corner[]){
    corner[0].x = (walls[i]->location.x+2)*cos(walls[i]->location.rotate);
    corner[1].x = (walls[i]->location.x-2)*cos(walls[i]->location.rotate);
    corner[2].x = (walls[i]->location.x-2)*cos(walls[i]->location.rotate);
    corner[3].x = (walls[i]->location.x+2)*cos(walls[i]->location.rotate);
    corner[0].z = (walls[i]->location.z-4)*sin(walls[i]->location.rotate);
    corner[1].z = (walls[i]->location.z-4)*sin(walls[i]->location.rotate);
    corner[2].z = (walls[i]->location.z+4)*sin(walls[i]->location.rotate);
    corner[3].z = (walls[i]->location.z+4)*sin(walls[i]->location.rotate);
}

void wall_hitPlace(Wall *w, Coordinate corner[]) {
    double height = 2, width = 4;
    for (int i=0; i<4; i++) {
        corner[i].z = w->location.z+cos(w->location.rotate)*width*(i<2?1:-1)+sin(w->location.rotate)*height*(i==1||i==2?1:-1);
        corner[i].x = w->location.x+cos(w->location.rotate)*height*(i==0?1:-1)+sin(w->location.rotate)*width*(i<2?1:-1);
    }
}

void updata_wall(int player_id){
    Wall *new_wall = NULL;
    while((new_wall = ws_getNewWall(player_id)) != NULL){
        walls[wall_count] = new_wall;
        wall_count++;
    }
    Coordinate c[4];

    for(int i=wall_start;i<wall_count;i++){
        if (walls[i] == NULL) {
            if (i == wall_start) wall_start++;
            continue;
        }
        walls[i]->remain -= *isUpdated;
        if(walls[i]->remain<=0) {
            walls[i] = NULL;
            continue;
        }
        // wall_corner(i, c);
        wall_hitPlace(walls[i], c);
        double damage = bullet_hit(c);
        if (damage) {
            printf("wall: damaged\n");
        }
        draw_wall(&walls[i]->location);
    }
}

void wall_make(Wall* start){
    walls[wall_count] = start;
    wall_count++;
}

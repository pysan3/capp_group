#include "types2json.h"
#include "ws_api/json-maker.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

char *json_playerID(char *buf, int player_id) {
    return json_int(buf, "player_id", player_id);
}

int data_to_json(char *buf, char const *name, const char *args, ...) {
    va_list ap;
    char *p = buf;
    va_start(ap, args);
	buf = json_objOpen(buf, NULL);
	buf = json_str(buf, "name", name);
    for (const char *arg = args; arg != NULL; arg = va_arg(ap, const char *)) {
        int data_length = strlen(arg);
        strncpy(buf, arg, data_length);
        buf += data_length;
    }
	buf = json_objClose(buf - 1);
	buf = json_end(buf);
	return buf - p;
}

char *json_Coordinate(char *buf, char const *dataname, const Coordinate *c) {
    buf = json_objOpen(buf, dataname);
    buf = json_double(buf, "x", c->x);
    buf = json_double(buf, "y", c->y);
    buf = json_double(buf, "z", c->z);
    buf = json_double(buf, "rotate", c->rotate);
    return json_objClose(buf);
}

char *json_Player(char *buf, char const *dataname, const Player *p) {
    buf = json_objOpen(buf, dataname);
    buf = json_Coordinate(buf, "location", &p->location);
    buf = json_int(buf, "hp", p->hp);
    buf = json_str(buf, "name", p->name);
    buf = json_int(buf, "sleep_bullet", p->sleep_bullet);
    buf = json_int(buf, "sleep_wall", p->sleep_wall);
    buf = json_int(buf, "Character", p->c);
    return json_objClose(buf);
}

int player2json(char *buf, char const *name, const char *dataname, const Player *p) {
    char tmp[1000];
    json_Player(tmp, dataname, p);
    return data_to_json(buf, name, tmp);
}

char *json_Bullet(char *buf, char const *dataname, const Bullet *b) {
    buf = json_objOpen(buf, dataname);
    buf = json_Coordinate(buf, "location", &b->location);
    buf = json_Coordinate(buf, "velocity", &b->velocity);
    buf = json_double(buf, "damage", b->damage);
    buf = json_int(buf, "player_id", b->player_id);
    return json_objClose(buf);
}

int bullet2json(char *buf, char const *name, const char *dataname, const Bullet *b) {
    char tmp[1000];
    json_Bullet(tmp, dataname, b);
    return data_to_json(buf, name, tmp);
}

char *json_Wall(char *buf, char const *dataname, const Wall *w) {
    buf = json_objOpen(buf, dataname);
    buf = json_Coordinate(buf, "location", &w->location);
    buf = json_int(buf, "remain", w->remain);
    buf = json_int(buf, "player_id", w->player_id);
    return json_objClose(buf);
}

int wall2json(char *buf, char const *name, const char *dataname, const Wall *w) {
    char tmp[1000];
    json_Wall(tmp, dataname, w);
    return data_to_json(buf, name, tmp);
}

Coordinate *Coordinate_json(const json_t *json, Coordinate *c) {
    c->x = json_getReal(json_getProperty(json, "x"));
    c->y = json_getReal(json_getProperty(json, "y"));
    c->z = json_getReal(json_getProperty(json, "z"));
    c->rotate = json_getReal(json_getProperty(json, "rotate"));
    return c;
}

Player *Player_json(const json_t *json, Player *p) {
    p->id = json_getInteger(json_getProperty(json, "id"));
    Coordinate_json(json_getProperty(json, "location"), &p->location);
    p->hp = json_getInteger(json_getProperty(json, "hp"));
    strcpy(p->name, json_getPropertyValue(json, "name"));
    p->sleep_bullet = json_getInteger(json_getProperty(json, "sleep_bullet"));
    p->sleep_wall = json_getInteger(json_getProperty(json, "sleep_wall"));
    p->c = json_getInteger(json_getProperty(json, "Character"));
    return p;
}

Bullet *Bullet_json(const json_t *json, Bullet *b) {
    Coordinate_json(json_getProperty(json, "location"), &b->location);
    Coordinate_json(json_getProperty(json, "velocity"), &b->velocity);
    b->damage = json_getReal(json_getProperty(json, "damage"));
    b->player_id = json_getInteger(json_getProperty(json, "player_id"));
    return b;
}

Wall *Wall_json(const json_t *json, Wall *w) {
    Coordinate_json(json_getProperty(json, "location"), &w->location);
    w->remain = json_getInteger(json_getProperty(json, "remain"));
    w->player_id = json_getInteger(json_getProperty(json, "player_id"));
    return w;
}

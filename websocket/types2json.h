#include "../typing.h"

char *json_Coordinate(char *buf, char const *dataname, const Coordinate *c);
char *json_Player(char *buf, char const *dataname, const Player *p);
char *json_Bullet(char *buf, char const *dataname, const Bullet *b);
char *json_Wall(char *buf, char const *dataname, const Wall *w);
char *json_playerID(char *buf, int player_id);

// int player2json(char *buf, char const *name, const char *dataname, const Player *p);
// int bullet2json(char *buf, char const *name, const char *dataname, const Bullet *b);
// int wall2json(char *buf, char const *name, const char *dataname, const Wall *w);

int data_to_json(char *buf, char const *name, const char *args, ...);

#include "online.h"
#include "types2json.h"
#include "ws_api/wsclient.h"
#include "ws_api/json.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>

#ifndef MYSERVER
	#define MYSERVER "ws://freeroomfinder.herokuapp.com/ws/cappgroup"
#else
	#undef MYSERVER
	#define MYSERVER "ws://127.0.0.1:5042/ws/cappgroup"
#endif

wsclient *client = NULL;
int *elapsed;

typedef struct _bullet_node{
    Bullet *b;
    int created_at;
    struct _bullet_node *next;
} BulletNode;
typedef struct {
    BulletNode *first, *last;
    pthread_mutex_t mut;
} BulletList;
void multi_bullet_append(BulletList *list, Bullet *b) {
    BulletNode *new = (BulletNode *)malloc(sizeof(BulletNode));
    new->b = b;
    new->created_at = *elapsed;
    new->next = NULL;
    pthread_mutex_lock(&list->mut);
    if (list->last != NULL) list->last->next = new;
    list->last = new;
    if (list->first == NULL) list->first = new;
    pthread_mutex_unlock(&list->mut);
}
Bullet *multi_bullet_pop_front(BulletList *list) {
    if (list->first == NULL) {
        return NULL;
    }
    pthread_mutex_lock(&list->mut);
    BulletNode *new = list->first;
    list->first = new->next;
    if (new->next == NULL) list->last = NULL;
    pthread_mutex_unlock(&list->mut);
    Bullet *b = new->b;
    // b->location.x = b->velocity.x * (*elapsed - new->created_at);
    // b->location.y = b->velocity.y * (*elapsed - new->created_at);
    // b->location.z = b->velocity.z * (*elapsed - new->created_at);
    free(new);
    return b;
}

typedef struct _wall_node{
    Wall *w;
    int created_at;
    struct _wall_node *next;
} WallNode;
typedef struct {
    WallNode *first, *last;
    pthread_mutex_t mut;
} WallList;
void multi_wall_append(WallList *list, Wall *w) {
    WallNode *new = (WallNode *)malloc(sizeof(WallNode));
    new->w = w;
    new->created_at = *elapsed;
    new->next = NULL;
    pthread_mutex_lock(&list->mut);
    if (list->last != NULL) list->last->next = new;
    list->last = new;
    if (list->first == NULL) list->first = new;
    pthread_mutex_unlock(&list->mut);
}

Wall *multi_wall_pop_front(WallList *list) {
    if (list->first == NULL) {
        return NULL;
    }
    pthread_mutex_lock(&list->mut);
    WallNode *new = list->first;
    list->first = new->next;
    if (new->next == NULL) list->last = NULL;
    pthread_mutex_unlock(&list->mut);
    Wall *w = new->w;
    // w->remain -= *elapsed - new->created_at;
    free(new);
    return w;
}

typedef struct {
	int *playerID;
	int gameID;
	struct timeval *start_from;
	Player **enemies;
	BulletList bulletlist;
	WallList walllist;
} ArrivalInfo;
ArrivalInfo arrivals;

int onclose(wsclient *c) {
	fprintf(stderr, "onclose called: %d\n", c->sockfd);
	return 0;
}

int onerror(wsclient *c, wsclient_error *err) {
	fprintf(stderr, "onerror: (%d): %s\n", err->code, err->str);
	if(err->extra_code) {
		errno = err->extra_code;
		perror("recv");
		exit(EXIT_FAILURE);
	}
	return 0;
}

int onmessage(wsclient *c, wsclient_message *msg) {
	int isError = 0;
	// fprintf(stderr, "onmessage: (%llu): %s\n", msg->payload_len, msg->payload);
	json_t mem[5000];
	json_t const *json = json_create(msg->payload, mem, sizeof(mem)/sizeof(mem[0]));
	if (!json) {
		printf("newGameID: Error while creating json\n");
		return EXIT_FAILURE;
	}
	const char *name = json_getPropertyValue(json, "name");
	if (strcmp(name, "login") == 0) {
		if (json_getInteger(json_getProperty(json, "success")) == 1) {
			arrivals.playerID = (int *)malloc(sizeof(int));
			*arrivals.playerID = (int)json_getInteger(json_getProperty(json, "player_id"));
		} else isError = 1;
	} else if (strcmp(name, "newBullet") == 0) {
		Bullet *b = (Bullet *)malloc(sizeof(Bullet));
		multi_bullet_append(&arrivals.bulletlist, Bullet_json(json_getProperty(json, "data"), b));
	} else if (strcmp(name, "newWall") == 0) {
		Wall *w = (Wall *)malloc(sizeof(Wall));
		multi_wall_append(&arrivals.walllist, Wall_json(json_getProperty(json, "data"), w));
	} else if (strcmp(name, "updateEnemies") == 0) {
		if (arrivals.enemies == NULL) arrivals.enemies = (Player **)malloc(sizeof(Player *) * ENEMY_NUM);
		Player **e = &arrivals.enemies[0];
		const json_t *enemy;
		for (enemy = json_getChild(json_getProperty(json, "data")); enemy != NULL; enemy = json_getSibling(enemy)) {
			if (*e == NULL) *e = (Player *)malloc(sizeof(Player));
			Player_json(enemy, *e++);
		}
	} else if (strcmp(name, "startTime") == 0) {
		arrivals.start_from = (struct timeval *)malloc(sizeof(struct timeval));
		arrivals.start_from->tv_sec = json_getInteger(json_getProperty(json, "time"));
		arrivals.start_from->tv_usec = 0;
	} else {
		printf("WARNING: received unknown message from server\n\t%s\n", msg->payload);
	}
	if (isError) {
		printf("ERROR onmessage: %s\n", msg->payload);
		exit(EXIT_FAILURE);
	}
	return 0;
}

int onopen(wsclient *c) {
	// fprintf(stderr, "onopen called: %d\n", c->sockfd);
	return 0;
}

int receiveNewGameID(wsclient *c, wsclient_message *msg) {
	json_t mem[32];
	// fprintf(stderr, "onmessage: (%llu): %s\n", msg->payload_len, msg->payload);
	json_t const *json = json_create(msg->payload, mem, sizeof(mem)/sizeof(mem[0]));
	if (!json) {
		printf("newGameID: Error while creating json\n");
		return EXIT_FAILURE;
	}
	if (strcmp(json_getPropertyValue(json, "name"), "newGameID") == 0) {
		arrivals.gameID = (int)json_getInteger(json_getProperty(json, "id"));
	} else {
		printf("receiveNewGameID: something went wrong\n");
		exit(EXIT_FAILURE);
	}
	return 0;
}
int multi_createNewGameID(void) {
	arrivals.gameID = 0;
	// printf("%s\n", MYSERVER);
	wsclient *myclient = libwsclient_new(MYSERVER);
	if (!myclient) {
		fprintf(stderr, "ERROR: unable to connect with ther server for new gameID\n");
		exit(1);
	}
	libwsclient_onmessage(myclient, &receiveNewGameID);
	libwsclient_run(myclient);
	char resp[1000];
	data_to_json(resp, "generateGameID", NULL);
	printf("createNewGame: %s\n", resp);
	libwsclient_send(myclient, resp);
	while (arrivals.gameID == 0) sleep(1);
	libwsclient_close(myclient);
	libwsclient_finish(myclient);
	return arrivals.gameID;
}

int multi_init(int id, int *time) {
	// init data
	memset(&arrivals, 0, sizeof(ArrivalInfo));
	elapsed = time;
	arrivals.gameID = id;

	// connect with server
	client = libwsclient_new(MYSERVER);
	if (!client) {
		fprintf(stderr, "ERROR: unable to connect with the server\n");
		exit(1);
	}
	libwsclient_onopen(client, &onopen);
	libwsclient_onmessage(client, &onmessage);
	libwsclient_onerror(client, &onerror);
	libwsclient_onclose(client, &onclose);
	libwsclient_run(client);

	// send gameid
	char resp[64], data[64];
	sprintf(data, "\"gameID\":%d,", arrivals.gameID);
	data_to_json(resp, "login", data, NULL);
	// printf("init: %s\n", resp);
	libwsclient_send(client, resp);
	return (arrivals.gameID == id) - 1;
}

int multi_close(int gameID) {
	libwsclient_close(client);
	libwsclient_finish(client);
	return 0;
}

void multi_createPlayer_th(threatPlayer *tp) {
	char resp[1000], data[1000];
	json_Player(data, "player", tp->p);
	data_to_json(resp, "createPlayer", data, NULL);
	// printf("createPlayer: %s\n", resp);
	libwsclient_send(client, resp);
	while (arrivals.playerID == NULL) sleep(1);
	*tp->id = *arrivals.playerID;
	free(arrivals.playerID);
	free(tp);
}

void multi_sendPlayer_th(Player *p) {
	char resp[1000], data[1000];
	json_Player(data, "player", p);
	data_to_json(resp, "updatePlayer", data, NULL);
	// printf("sendPlayer: %s\n", resp);
	libwsclient_send(client, resp);
}

void multi_sendNewBullet_th(playersBullet *pb) {
	char resp[1000], data[1000];
	json_Bullet(data, "bullet", pb->b);
	data_to_json(resp, "newBullet", data, NULL);
	// printf("newBullet: %s\n", resp);
	libwsclient_send(client, resp);
	free(pb);
}

void multi_sendNewWall_th(playersWall *pw) {
	char resp[1000], data[1000];
	json_Wall(data, "wall", pw->w);
	data_to_json(resp, "newWall", data, NULL);
	// printf("newWall: %s\n", resp);
	libwsclient_send(client, resp);
	free(pw);
}

Player *multi_getEnemyInfo(int id) {
	Player *e;
	for (int i=0; i<ENEMY_NUM; i++) {
		if ((e = arrivals.enemies[i]) != NULL) {
			if (e->id == id) {
				return e;
			}
		}
	}
	return NULL;
}

Player **multi_getAllEnemyInfo(int player_id) {
	return arrivals.enemies;
}

Bullet *multi_getNewBullet(int player_id) {
	return multi_bullet_pop_front(&arrivals.bulletlist);
}

Wall *multi_getNewWall(int player_id) {
	return multi_wall_pop_front(&arrivals.walllist);
}

void multi_loadEnemies_th(threadLoadEnemy *le) {
	if (arrivals.enemies != NULL) {
		for (int i=0; i<ENEMY_NUM; i++) {
			le->e[i] = arrivals.enemies[i];
		}
		free(arrivals.enemies);
	}
	arrivals.enemies = le->e;
    free(le);
	while (arrivals.start_from == NULL) sleep(1);
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
	long time_wait = (arrivals.start_from->tv_sec - current_time.tv_sec) * MICRO + (arrivals.start_from->tv_usec - current_time.tv_usec);
	printf("Game starting in %lf sec\n", (double)time_wait / MICRO);
    if (time_wait < 0) {
        fprintf(stderr, "not good connection with the server\n");
        exit(EXIT_FAILURE);
    } else {
        usleep(time_wait);
    }
}

void multi_dead_th(threatPlayer *tp) {
	char resp[1000];
	data_to_json(resp, "dead", NULL);
	// printf("dead: %s\n", resp);
	libwsclient_send(client, resp);
	free(tp);
}

#include "online.h"
#include "types2json.h"
#include "ws_api/wsclient.h"
#include "ws_api/json.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>

wsclient *client = NULL;
int gameID;
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
    pthread_mutex_lock(&list->mut);
    BulletNode *new = (BulletNode *)malloc(sizeof(BulletNode));
    new->b = b;
    new->created_at = *elapsed;
    new->next = NULL;
    list->last->next = new;
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
    b->location.x = b->velocity.x * (*elapsed - new->created_at);
    b->location.y = b->velocity.y * (*elapsed - new->created_at);
    b->location.z = b->velocity.z * (*elapsed - new->created_at);
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
    pthread_mutex_lock(&list->mut);
    WallNode *new = (WallNode *)malloc(sizeof(WallNode));
    new->w = w;
    new->created_at = *elapsed;
    new->next = NULL;
    list->last->next = new;
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
    w->remain -= *elapsed - new->created_at;
    free(new);
    return w;
}

typedef struct {
	int *playerID;
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
	}
	return 0;
}

int onmessage(wsclient *c, wsclient_message *msg) {
	fprintf(stderr, "onmessage: (%llu): %s\n", msg->payload_len, msg->payload);
	libwsclient_close(c);
	return 0;
}

int onopen(wsclient *c) {
	fprintf(stderr, "onopen called: %d\n", c->sockfd);
    libwsclient_send(c, "Hello onopen");
	return 0;
}

int multi_createNewGameID(void) {
	return 0;
}

int multi_init(int id, int *time) {
	// init data
	gameID = id;
	elapsed = time;
	memset(&arrivals, 0, sizeof(ArrivalInfo));

	// connect with server
	client = libwsclient_new("ws://freeroomfinder.heroku.com/ws/cappgroup");
	if (!client) {
		fprintf(stderr, "ERROR: unable to connect with the server\n");
		exit(1);
	}
	libwsclient_onopen(client, &onopen);
	libwsclient_onmessage(client, &onmessage);
	libwsclient_onerror(client, &onerror);
	libwsclient_onclose(client, &onclose);
	// libwsclient_run(client);

	// send gameid
	char resp[64], data[64];
	sprintf(data, "\"gameID\":%d,", gameID);
	int resp_len = data_to_json(resp, "login", data, NULL);
	printf("init: %s\n", resp);
	// libwsclient_send(client, resp);
	return (gameID == id) - 1;
}

int multi_close(int gameID) {
	libwsclient_finish(client);
	return 0;
}

void multi_createPlayer_th(threatPlayer *tp) {
	char resp[1000], data[1000];
	json_Player(data, "player", tp->p);
	int resp_len = data_to_json(resp, "createPlayer", data, NULL);
	printf("createPlayer: %s\n", resp);
	// libwsclient_send(client, resp);
	while (arrivals.playerID == NULL) {
		sleep(1);
		arrivals.playerID = (int *)malloc(sizeof(int));
		*arrivals.playerID = 5;
	}
	*tp->id = *arrivals.playerID;
	free(arrivals.playerID);
	free(tp);
}

void multi_sendPlayer_th(Player *p) {
	char resp[1000], data[1000];
	json_Player(data, "player", p);
	int resp_len = data_to_json(resp, "updatePlayer", data, NULL);
	printf("sendPlayer: %s\n", resp);
	// libwsclient_send(client, resp);
}

void multi_sendNewBullet_th(playersBullet *pb) {
	char resp[1000], data[1000], id[1000];
	json_playerID(id, pb->player_id);
	json_Bullet(data, "bullet", pb->b);
	int resp_len = data_to_json(resp, "newBullet", id, data, NULL);
	printf("newBullet: %s\n", resp);
	// libwsclient_send(client, resp);
	free(pb);
}

void multi_sendNewWall_th(playersWall *pw) {
	char resp[1000], data[1000], id[1000];
	json_playerID(id, pw->player_id);
	json_Wall(data, "wall", pw->w);
	int resp_len = data_to_json(resp, "newWall", id, data, NULL);
	printf("newWall: %s\n", resp);
	// libwsclient_send(client, resp);
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
	multi_bullet_pop_front(&arrivals.bulletlist);
}

Wall *multi_getNewWall(int player_id) {
	multi_wall_pop_front(&arrivals.walllist);
}

void multi_loadEnemies_th(threadLoadEnemy *le) {
	arrivals.enemies = le->e;
	while (arrivals.start_from == NULL) sleep(1);
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    current_time.tv_sec -= arrivals.start_from->tv_sec;
    current_time.tv_usec -= arrivals.start_from->tv_usec;
    if (current_time.tv_sec * MICRO + MICRO - current_time.tv_usec > 0) {
        fprintf(stderr, "not good connection with the server\n");
        exit(EXIT_FAILURE);
    } else {
        usleep(- current_time.tv_sec * MICRO - (MICRO - current_time.tv_usec));
    }
    free(le);
}

int tmp_main(int gameID) {
	//Initialize new wsclient * using specified URI
	client = libwsclient_new("ws://echo.websocket.org");
	if(!client) {
		fprintf(stderr, "Unable to initialize new WS client.\n");
		exit(1);
	}
	//set callback functions for this client
	libwsclient_onopen(client, &onopen);
	libwsclient_onmessage(client, &onmessage);
	libwsclient_onerror(client, &onerror);
	libwsclient_onclose(client, &onclose);
	//bind helper UNIX socket to "test.sock"
	//One can then use netcat (nc) to send data to the websocket server end on behalf of the client, like so:
	// $> echo -n "some data that will be echoed by echo.websocket.org" | nc -U test.sock
	// libwsclient_helper_socket(client, "test.sock");
	//starts run thread.
	libwsclient_run(client);
	//blocks until run thread for client is done.
	libwsclient_finish(client);
    return 0;
}

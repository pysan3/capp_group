#include "online.h"
#include "types2json.h"
#include "ws_api/wsclient.h"
#include "ws_api/json.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

wsclient *client = NULL;
int gameID;
int *elapsed;
typedef struct {
	int *playerID;
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

// pid_t multi_createPlayer(Player *p, int *id) {
// 	pid_t pid;
// 	if ((pid = fork()) == 0) {
// 		char resp[1000], data[1000];
// 		json_Player(data, "player", p);
// 		int resp_len = data_to_json(resp, "createPlayer", data, NULL);
// 		printf("createPlayer: %s\n", resp);
// 		// libwsclient_send(client, resp);
// 		if ((pid = fork()) == 0) {
// 			while (arrivals.playerID == NULL) {
// 				sleep(1);
// 				arrivals.playerID = (int *)malloc(sizeof(int));
// 				*arrivals.playerID = 5;
// 			}
// 			*id = *arrivals.playerID;
// 			free(arrivals.playerID);
// 			exit(0);
// 		} else {
// 			waitpid(pid, NULL, 0);
// 			exit(0);
// 		}
// 	} else {
// 		return pid;
// 	}
// }

// pid_t multi_sendPlayer(Player *p) {
// 	pid_t pid;
// 	if ((pid = fork()) == 0) {
// 		char resp[1000], data[1000];
// 		json_Player(data, "player", p);
// 		int resp_len = data_to_json(resp, "updatePlayer", data, NULL);
// 		printf("sendPlayer: %s\n", resp);
// 		// libwsclient_send(client, resp);
// 		exit(0);
// 	} else {
// 		return pid;
// 	}
// }

// pid_t multi_sendNewBullet(int player_id, Bullet *b) {
// 	pid_t pid;
// 	if ((pid = fork()) == 0) {
// 		char resp[1000], data[1000], id[1000];
// 		json_playerID(id, player_id);
// 		json_Bullet(data, "bullet", b);
// 		int resp_len = data_to_json(resp, "newBullet", id, data, NULL);
// 		printf("newBullet: %s\n", resp);
// 		// libwsclient_send(client, resp);
// 		exit(0);
// 	} else {
// 		return pid;
// 	}

pthread_t multi_createPlayer(Player *p, int *id) {
	return 1;
}

pthread_t multi_sendPlayer(Player *p) {
	return 0;
}

pthread_t multi_sendNewBullet(int player_id, Bullet *b) {
	return 0;
}

pthread_t multi_sendNewWall(int player_id, Wall *w) {
	return 0;
}

Player *multi_getEnemyInfo(int id) {
	return NULL;
}

Player **multi_getAllEnemyInfo(int player_id) {
	return NULL;
}

Bullet *multi_getNewBullet(int player_id) {
	return NULL;
}

Wall *multi_getNewWall(int player_id) {
	return NULL;
}

pthread_t multi_loadEnemies(int player_id, Player *e[]) {
	return 0;
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

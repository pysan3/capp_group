#include "online.h"
#include "ws_api/wsclient.h"
#include "ws_api/json.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

wsclient *client = NULL;
int gameID;

int multi_createNewGameID(void) {
	return 0;
}

int multi_init(int id) {
	gameID = id;
	return gameID == id;
}

pid_t multi_createPlayer(Player *p, int *id) {
	return 1;
}

pid_t multi_sendPlayer(Player *p) {
	return 0;
}

pid_t multi_sendNewBullet(Bullet *b) {
	return 0;
}

pid_t multi_sendNewWall(Wall *w) {
	return 0;
}

Player *multi_getEnemyInfo(int id) {
	return NULL;
}

Bullet *multi_getNewBullet(void) {
	return NULL;
}

Wall *multi_getNewWall(void) {
	return NULL;
}

pid_t multi_loadEnemies(Player *e) {
	return 0;
}


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
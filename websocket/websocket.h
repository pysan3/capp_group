#pragma once

#include "typing4threads.h"
#include <pthread.h>

// ゲームIDを新規作成する場合に呼び出す
// 戻り値:新規作成したゲームのID
int ws_createNewGameID(void);

// wsコネクション確立
// 必ず最初に1回だけ呼び出すこと
// 引数:gameID:接続するゲームのID
// 戻り値が-1の場合fail、エラーを吐いて終了すること
int ws_init(int gameID, int *time, int *updated);

// wsコネクション切断
// 必ず最後に1回だけ呼び出すこと
// 引数:gameID:接続するゲームのID
// 戻り値が-1の場合fail、エラーを吐いて終了すること
int ws_close(int gameID);

// サーバにプレイヤーを新規登録する
// 引数:p:新規登録するプレイヤーのポインタ, *id:不定
// 戻り値:サーバからidの返答を待つプロセスのプロセスid(型はpthread_tだがこれは実質int)
// ※呼び出す際のidは何でもよい。サーバ側でid登録した後、idに格納した状態で関数が終了する
// p->idには登録しないので自分ですること
pthread_t ws_createPlayer(Player *p, int *id);

// プレイヤーのデータ更新をサーバに送信
// 各パラメータを更新したあとに呼び出すこと
// 引数: p: Playerのポインタ
// 戻り値:送信プロセスのプロセスid(型はpthread_tだがこれは実質int)
pthread_t ws_sendPlayer(Player *p);

// 新規作成されたBullet型の構造体をサーバに送信する
// パラメータの更新をしてから呼び出すこと
// 引数:player_id: 自分のid, Bulletのポインタ
// 戻り値:送信プロセスのプロセスid(型はpthread_tだがこれは実質int)
pthread_t ws_sendNewBullet(int player_id, Bullet *b);

// 新規作成されたWall型の構造体をサーバに送信する
// パラメータの更新をしてから呼び出すこと
// 引数:player_id: 自分のid, Wallのポインタ
// 戻り値:送信プロセスのプロセスid(型はpthread_tだがこれは実質int)
pthread_t ws_sendNewWall(int player_id, Wall *w);

// idの敵の情報を持った構造体のポインタを返す関数
// 返ってくるポインタの情報は最新のものである
// 引数: id: Enemyのid
// 戻り値: idに対応するEnemyの構造体のポインタ
Player *ws_getEnemyInfo(int id);

// 自分player_id以外の敵の情報のリストを返却する
// 引数: player_id: 自分のid
// 戻り値: 自分以外の敵の情報の配列
Player **ws_getAllEnemyInfo(int player_id);

// 自分以外の場所で生成された玉のポインタを1つ返す
// もし1つもない場合にはnullポインタが帰る
// 引数:player_id: 自分のid
// 戻り値:新規玉がある場合はそのポインタ、ない場合はnullポインタ
// 新規玉はmallocするので消滅時にメモリの開放をお願いします
Bullet *ws_getNewBullet(int player_id);

// 自分以外の場所で生成された壁のポインタを1つ返す
// もし1つもない場合にはnullポインタが帰る
// 引数:player_id: 自分のid
// 戻り値:新規壁がある場合はそのポインタ、ない場合はnullポインタ
// 新規壁はmallocするので消滅時にメモリの開放をお願いします
Wall *ws_getNewWall(int player_id);

// オンラインの場合、敵が窓に参加した通知がサーバから届いた場合に配列に追加する
// 引数:e:敵を格納する配列
pthread_t ws_loadEnemies(int player_id, Player *e[]);

// 自分が死んだ場合に呼ぶ関数
// この関数実行後にサーバにリクエストを投げるとバグる可能性あり
// 引数: player_id
// 戻り値: サーバへリクエストを投げるスレッドid
pthread_t ws_dead(int player_id);

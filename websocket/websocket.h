#pragma once

#include "../header.h"
#include "ws_api/wsclient.h"
#include "ws_api/json.h"

// wsコネクション確立
// 必ず最初に1回だけ呼び出すこと
// 戻り値が-1の場合fail、エラーを吐いて終了すること
int ws_init(void);

// 玉が当たってHPが減少したことを他のプレイヤーに周知する
// 与えられたidのPlayerのhpが変わったことを全員に伝える関数
// 引数: id: Player.id, hp: Player.hp
// 戻り値: 失敗で-1
int ws_sendHP(Player);

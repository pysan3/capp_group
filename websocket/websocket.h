#pragma once

#include "ws_api/wsclient.h"
#include "ws_api/json.h"

// wsコネクション確立
// 必ず最初に1回だけ呼び出すこと
// 戻り値が-1の場合fail、エラーを吐いて終了すること
int ws_init(void);

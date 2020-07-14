<<<<<<< HEAD
#include "../header.h"
=======
#pragma once

#include "../header.h"

//新しい玉がないか確認、最後に描画と位置の更新
void update_bullets(void); 

//引数として物体の4隅の位置を受け取り受けたダメージの合計を返す、当たった玉は消去
double bullet_hit(Coordinate* corner[4]);

//次のフレームでの玉の位置を計算
void bullet_next(void);

//Bullet型を受け取り、球を生成、サーバーにも伝える
void bullet_throw(Bullet* start);
>>>>>>> 92a75cb56bf5a3bef501b0037be25b93d9ff3813

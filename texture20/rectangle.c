#include "../header.h"
#include "rectangle.h"
#include "matrix.h"

/*
** 矩形の描画
*/
void rectangle(double w, double h, const float l[])
{
  /* 頂点の座標値 */
  const GLdouble vertex[4][3] = {
    { 0, 0, 0 },
    { 0, 0, FIELD_MAX_Z },
    {  FIELD_MAX_X, 0, FIELD_MAX_Z },
    {  FIELD_MAX_X, 0, 0 },
  };
  // const GLdouble vertex[4][3] = {
  //   { -w, -h, 0.0 },
  //   {  w, -h, 0.0 },
  //   {  w,  h, 0.0 },
  //   { -w,  h, 0.0 }
  // };

  /* 頂点のテクスチャ座標 */
  static const GLdouble texcoord[4][2] = {
    { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 }, { 0.0, 1.0 }
  };

  /* 接空間（＝ローカル座標系）における光源位置 */
  double lt[4] = { l[0], l[1], l[2], l[3] };
  /* モデルビュー変換行列の逆行列 */
  double m[16];

  /* 現在のモデルビュー変換行列の逆行列を求める */
  glGetDoublev(GL_MODELVIEW_MATRIX, m);
  inverse(m, m);

  /* 接空間（＝ローカル座標系）における光源位置を求める */
  transform(lt, m, lt);

  /* 平行光線でなければ実座標を求めておく */
  if (lt[3] != 0.0) {
    lt[0] /= lt[3];
    lt[1] /= lt[3];
    lt[2] /= lt[3];
  }

  /* 矩形を描く */
  glBegin(GL_QUADS);

  for (int i = 0; i < 4; ++i) {
    /* 法線マップのテクスチャ座標を設定する */
    glTexCoord2dv(texcoord[i]);

    /* 接空間における光源の方向ベクトルを
       正規化マップのテクスチャ座標に設定する */
    if (lt[3] != 0.0) {
      glMultiTexCoord3d(GL_TEXTURE1,
        lt[0] - vertex[i][0],
        lt[1] - vertex[i][1],
        lt[2] - vertex[i][2]);
    }
    else {
      glMultiTexCoord3d(GL_TEXTURE1, lt[0], lt[1], lt[2]);
    }

    /* 対応する頂点座標の指定 */
    glVertex3dv(vertex[i]);
  }
  glEnd();
}

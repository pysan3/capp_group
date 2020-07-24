#include "../header.h"

#include <math.h>

#include "normalizemap.h"

/*
** �����٥��ȥ��ƥ��������ͤ��Ѵ�����
*/
static void vec2tex(float nx, float ny, float nz, GLubyte tex[])
{
  tex[0] = (GLubyte)(nx * 127.5 + 127.5);
  tex[1] = (GLubyte)(ny * 127.5 + 127.5);
  tex[2] = (GLubyte)(nz * 127.5 + 127.5);
  tex[3] = 255;
}

/*
** �������ޥåפκ���
*/
void makeNormalizeMap(GLubyte *tex[], int width, int height)
{
  int i = 0;

  for (int v = 0; v < height; ++v) {
    float y = (float)(v + v - height) / (float)height;
    float y2 = y * y;

    for (int u = 0; u < width; ++u) {
      float x = (float)(u + u - width) / (float)width;
      float x2 = x * x;

      /* �����٥��ȥ� */
      float r = 1.0f / sqrtf(x2 + y2 + 1.0f);
      float s = x * r;
      float t = y * r;

      /* ���̤Υƥ�������ˤĤ��������٥��ȥ���Ǽ���� */
      vec2tex(-r, -t,  s, tex[0] + i);  /* negative x */
      vec2tex( s, -r, -t, tex[1] + i);  /* negative y */
      vec2tex(-s, -t, -r, tex[2] + i);  /* negative z */
      vec2tex( r, -t, -s, tex[3] + i);  /* positive x */
      vec2tex( s,  r,  t, tex[4] + i);  /* positive y */
      vec2tex( s, -t,  r, tex[5] + i);  /* positive z */

      i += 4;
    }
  }
}

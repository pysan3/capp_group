#include "../header.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "normalmap.h"

/*
** �⤵�ޥåפ��Ȥ�ˡ���ޥåפ��������
*/
void makeNormalMap(GLubyte *tex, int width, int height, double nz, const char *name)
{
  FILE *fp = fopen(name, "rb");

  if (fp) {
    unsigned long size = width * height;
    unsigned char *map = (unsigned char *)malloc(size);

    if (map) {

      /* �⤵�ޥåפ��ɤ߹��� */
      fread(map, height, width, fp);
      fclose(fp);

      for (unsigned long y = 0; y < size; y += width) {
        for (int x = 0; x < width; ++x) {
          /* ���ܤ�����ǤȤ��ͤκ���ˡ���٥��ȥ����ʬ���Ѥ��� */
          double nx = map[y + x] - map[y + (x + 1) % width];
          double ny = map[y + x] - map[(y + width) % size + x];
          /* ˡ���٥��ȥ��Ĺ������Ƥ��� */
          double nl = sqrt(nx * nx + ny * ny + nz * nz);

          *(tex++) = (GLubyte)(nx * 127.5 / nl + 127.5);
          *(tex++) = (GLubyte)(ny * 127.5 / nl + 127.5);
          *(tex++) = (GLubyte)(nz * 127.5 / nl + 127.5);
          *(tex++) = 255;
        }
      }

      free(map);
    }
  } else {
    printf("file not found\n");
  }
}

/*
** ����η׻�
*/
#include <stdio.h>
#include <math.h>
#include "matrix.h"

/*
** ñ�̹���
*/
static const double ident[] = {
  1.0,  0.0,  0.0,  0.0,
  0.0,  1.0,  0.0,  0.0,
  0.0,  0.0,  1.0,  0.0,
  0.0,  0.0,  0.0,  1.0,
};

/*
** �Ѵ�����Υ��ԡ���m1��m2��
*/
void copyMatrix(const double m1[], double m2[])
{
  int i;

  for (i = 0; i < 16; ++i) m2[i] = m1[i];
}

/*
** ñ�̹���� m ������
*/
void loadIdentity(double m[])
{
  copyMatrix(ident, m);
}

/*
** �Ѵ� (v1��m��v2)
*/
void transform(const double v1[], const double m[], double v2[])
{
  int i;
  double s[4];
  
  for (i = 0; i < 4; ++i) {
    s[i] = v1[0] * m[i] + v1[1] * m[i + 4] + v1[2] * m[i + 8] + v1[3] * m[i + 12];
  }
  
  for (i = 0; i < 4; ++i) v2[i] = s[i];
}

/*
** ������� (m1��m2��m3)
*/
void multiply(const double m1[], const double m2[], double m3[])
{
  int j;
  
  for (j = 0; j < 4; ++j) {
    transform(m1, m2, m3);
    m1 += 4;
    m3 += 4;
  }
}

/*
** ʿ�԰�ư
**
**   x, y, z: ��ư��
**   m: �Ѵ�����
*/
void translate(const double x, const double y, const double z, double t[])
{
  static double m[] = {
    1.0,  0.0,  0.0,  0.0,
    0.0,  1.0,  0.0,  0.0,
    0.0,  0.0,  1.0,  0.0,
    0.0,  0.0,  0.0,  1.0,
  };
  
  m[12] = x;
  m[13] = y;
  m[14] = z;
  
  multiply(t, m, t);
}

/*
** Ǥ�ռ�����β�ž
**
**   a: ��ž��
**   x, y, z: ��
**   m: �Ѵ�����
*/
void rotate(const double a, const double x, const double y, const double z, double r[])
{
  double d = x * x + y * y + z * z;
  
  if (d != 0.0) {
    double u  = sqrt(d);
    double l  = x / u, m  = y / u, n  = z / u;
    double l2 = l * l, m2 = m * m, n2 = n * n;
    double lm = l * m, mn = m * n, nl = n * l;
    double s = sin(a);
    double c = cos(a);
    double c1 = 1.0 - c;
    double t[16];
    
    t[ 0] = (1.0 - l2) * c + l2;
    t[ 1] = lm * c1 + n * s;
    t[ 2] = nl * c1 - m * s;
    
    t[ 4] = lm * c1 - n * s;
    t[ 5] = (1.0 - m2) * c + m2;
    t[ 6] = mn * c1 + l * s;
    
    t[ 8] = nl * c1 + m * s;
    t[ 9] = mn * c1 - l * s;
    t[10] = (1.0 - n2) * c + n2;
    
    t[ 3] = t[ 7] = t[11] = t[12] = t[13] = t[14] = 0.0;
    t[15] = 1.0;
    
    multiply(r, t, r);
  }
}

/*
** �չ���η׻� (inv(m1)��m2)
*/
int inverse(const double m1[], double m2[])
{
  int i, j, k;
  double lu[20], *plu[4], det;
  
  /* j �Ԥ����Ǥ��ͤ������ͤκ����ͤ� plu[j][4] �˵��� */
  for (j = 0; j < 4; ++j) {
    double max = fabs(*(plu[j] = lu + 5 * j) = *(m1++));
    for (i = 0; ++i < 4;) {
      double a = fabs(plu[j][i] = *(m1++));
      if (a > max) max = a;
    }
    if (max == 0.0) return 0;
    plu[j][4] = 1.0 / max;
  }
  
  det = 1.0;
  
  /* �ԥܥåȤ��θ���� LU ʬ�� */
  for (j = 0; j < 4; ++j) {
    double max = fabs(plu[j][j] * plu[j][4]);
    i = j;
    for (k = j; ++k < 4;) {
      double a = fabs(plu[k][j] * plu[k][4]);
      if (a > max) {
        max = a;
        i = k;
      }
    }
    
    if (i > j) {
      double *t = plu[j];
      plu[j] = plu[i];
      plu[i] = t;
      det = -det;
    }
    if (plu[j][j] == 0.0) return 0;
    det *= plu[j][j];
    
    for (k = j; ++k < 4;) {
      plu[k][j] /= plu[j][j];
      for (i = j; ++i < 4;) {
        plu[k][i] -= plu[j][i] * plu[k][j];
      }
    }
  }
  
  /* �չ������� */
  for (k = 0; k < 4; ++k) {
    /* m2 ��ñ�̹�������ꤹ�� */
    for (i = 0; i < 4; ++i) {
      m2[i * 4 + k] = (plu[i] == lu + k * 5) ? 1.0 : 0.0;
    }
    /* lu ����չ������� */
    for (i = 0; i < 4; ++i) {
      for (j = i; ++j < 4;) {
        m2[j * 4 + k] -= m2[i * 4 + k] * plu[j][i];
      }
    }
    for (i = 4; --i >= 0;){
      for (j = i; ++j < 4;) {
        m2[i * 4 + k] -= plu[i][j] * m2[j * 4 + k];
      }
      m2[i * 4 + k] /= plu[i][i];
    }
  }
  
  return -1;
}

/*
** �������Ƥ�ɽ��
*/
void printMatrix(const double m[])
{
  int i;
  
  for (i = 0; i < 16; ++i) {
    printf("%8.4f", m[i]);
    if (i % 4 == 3) printf("\n");
  }
  printf("\n");
}

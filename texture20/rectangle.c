#include "../header.h"
#include "rectangle.h"
#include "matrix.h"

/*
** ���������
*/
void rectangle(double w, double h, const float l[])
{
  /* ĺ���κ�ɸ�� */
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

  /* ĺ���Υƥ��������ɸ */
  static const GLdouble texcoord[4][2] = {
    { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 }, { 0.0, 1.0 }
  };

  /* �ܶ��֡ʡ�������ɸ�ϡˤˤ������������ */
  double lt[4] = { l[0], l[1], l[2], l[3] };
  /* ��ǥ�ӥ塼�Ѵ�����εչ��� */
  double m[16];

  /* ���ߤΥ�ǥ�ӥ塼�Ѵ�����εչ������� */
  glGetDoublev(GL_MODELVIEW_MATRIX, m);
  inverse(m, m);

  /* �ܶ��֡ʡ�������ɸ�ϡˤˤ�����������֤���� */
  transform(lt, m, lt);

  /* ʿ�Ը����Ǥʤ���мº�ɸ����Ƥ��� */
  if (lt[3] != 0.0) {
    lt[0] /= lt[3];
    lt[1] /= lt[3];
    lt[2] /= lt[3];
  }

  /* ��������� */
  glBegin(GL_QUADS);

  for (int i = 0; i < 4; ++i) {
    /* ˡ���ޥåפΥƥ��������ɸ�����ꤹ�� */
    glTexCoord2dv(texcoord[i]);

    /* �ܶ��֤ˤ���������������٥��ȥ��
       �������ޥåפΥƥ��������ɸ�����ꤹ�� */
    if (lt[3] != 0.0) {
      glMultiTexCoord3d(GL_TEXTURE1,
        lt[0] - vertex[i][0],
        lt[1] - vertex[i][1],
        lt[2] - vertex[i][2]);
    }
    else {
      glMultiTexCoord3d(GL_TEXTURE1, lt[0], lt[1], lt[2]);
    }

    /* �б�����ĺ����ɸ�λ��� */
    glVertex3dv(vertex[i]);
  }
  glEnd();
}

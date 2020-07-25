#include "texture.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TEXWIDTH  256 * 2
#define TEXHEIGHT 256 * 2

void texture_init(void) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    GLubyte texture[TEXHEIGHT * TEXWIDTH * 4];

    makeNormalMap(texture, TEXWIDTH, TEXHEIGHT, 20.0, "../texture20/new_bump_b.raw");

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXWIDTH, TEXHEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glActiveTexture(GL_TEXTURE1);

    static GLubyte t[6][TEXHEIGHT / 2 * TEXWIDTH / 2 * 4];
    static GLubyte *normalize[] = { t[0], t[1], t[2], t[3], t[4], t[5] };

    makeNormalizeMap(normalize, TEXHEIGHT / 2, TEXWIDTH / 2);

    for (int i = 0; i < 6; ++i) {
        static const int target[] = {
            GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
            GL_TEXTURE_CUBE_MAP_POSITIVE_X,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        };
        glTexImage2D(target[i], 0, GL_RGBA, TEXHEIGHT / 2, TEXWIDTH / 2, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, normalize[i]);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
    glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_DOT3_RGB);
    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_PREVIOUS);
    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_TEXTURE);

    glActiveTexture(GL_TEXTURE0);
}

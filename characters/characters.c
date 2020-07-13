#include "characters.h"

void draw_character(Character c) {
    switch (c) {
        case DORAEMON:
            draw_doraemon();
            break;
        case LOTSO:
            draw_LOTSO();
            break;
        case KATANA:
            draw_KATANA();
            break;
        // case ここ:
        //     draw_ここ();
        //     break;
        default:
            break;
    }
}

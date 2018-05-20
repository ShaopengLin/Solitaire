#include "CardStruct.h"

void initializeAllegro()
{

    al_init();
    al_init_image_addon();
    al_init_native_dialog_addon();
    al_install_mouse();

}

void printb(ALLEGRO_BITMAP *background)
{

    al_draw_bitmap(background,0,0,NULL);

}

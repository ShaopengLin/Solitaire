#include "CardStruct.h"

void initializeAllegro()
{

    al_init();
    al_init_image_addon();
    al_init_native_dialog_addon();
    al_install_mouse();
    al_init_font_addon();
    al_init_ttf_addon();

}

void printb(ALLEGRO_BITMAP *background)
{

    al_draw_bitmap(background,0,0,NULL);

}

void drawTime(ALLEGRO_FONT *font,ALLEGRO_TIMER *timer, int seconds, int minutes)
{
    char cseconds[4], cminutes[4];
    seconds += al_get_timer_count(timer) - 60*(al_get_timer_count(timer)/60);
    minutes += al_get_timer_count(timer)/60;
    sprintf(cseconds,"%d",seconds);
    sprintf(cminutes,"%d",minutes);
    al_draw_text(font, al_map_rgb(255,255,255), 300, 10,NULL, "Time:");
    al_draw_text(font,al_map_rgb(255,255,255), 300, 30,NULL, cminutes);
    al_draw_text(font,al_map_rgb(255,255,255), 320+(al_get_timer_count(timer)/600*10), 30,NULL, ":");
    al_draw_text(font, al_map_rgb(255,255,255), 335+(al_get_timer_count(timer)/600*10), 30,NULL, cseconds);

}

void drawMovescount(ALLEGRO_FONT *font, int movesCounter)
{
    char cMovesCounter[10];
    sprintf(cMovesCounter,"%d",movesCounter);
    al_draw_text(font, al_map_rgb(255,255,255), 200, 10,NULL, "Moves:");
    al_draw_text(font,al_map_rgb(255,255,255), 200, 30,NULL, cMovesCounter);


}

void drawScore(ALLEGRO_FONT *font, int score)
{
    char cScore[10];
    sprintf(cScore,"%d",score);
    al_draw_text(font, al_map_rgb(255,255,255), 100, 10,NULL, "Scores:");
    al_draw_text(font,al_map_rgb(255,255,255), 100, 30,NULL, cScore);


}

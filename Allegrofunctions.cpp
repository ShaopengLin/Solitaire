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

void drawSeconds(ALLEGRO_FONT *font, ALLEGRO_TIMER *timer, int seconds)
{

    char cseconds[4];
    sprintf(cseconds,"%d",seconds + al_get_timer_count(timer) - 60*(al_get_timer_count(timer)/60));
    al_draw_text(font, al_map_rgb(255,255,255), 335+((seconds + al_get_timer_count(timer))/600*10), 30,NULL, cseconds);

}

void drawMinutes(ALLEGRO_FONT *font, ALLEGRO_TIMER *timer, int seconds)
{
    char cminutes[4];
    sprintf(cminutes,"%d",(seconds + al_get_timer_count(timer))/60);
    al_draw_text(font, al_map_rgb(255,255,255), 300, 10,NULL, "Time:");
    al_draw_text(font,al_map_rgb(255,255,255), 300, 30,NULL, cminutes);
    al_draw_text(font,al_map_rgb(255,255,255), 320+(seconds + al_get_timer_count(timer))/600*10, 30,NULL, ":");

}

void drawMovescount(ALLEGRO_FONT *font, int movesCounter)
{
    char cMovesCounter[10];
    sprintf(cMovesCounter,"%d",movesCounter);
    al_draw_text(font, al_map_rgb(255,255,255), 175, 10,NULL, "Moves:");
    al_draw_text(font,al_map_rgb(255,255,255), 175, 30,NULL, cMovesCounter);

}

void drawScore(ALLEGRO_FONT *font, int &score)
{
    char cScore[10];
    fixScore(score);
    sprintf(cScore,"%d",score);
    al_draw_text(font, al_map_rgb(255,255,255), 50, 10,NULL, "Scores:");
    al_draw_text(font,al_map_rgb(255,255,255), 50, 30,NULL, cScore);

}

void drawPausescreen(Decks c[], ALLEGRO_BITMAP *pauseButton, ALLEGRO_BITMAP *pauseScreen)
{

    al_draw_bitmap(pauseButton,400,20,NULL);

    al_draw_bitmap(pauseScreen, 10, 100, NULL);

}


void resumeGame(Decks c[], ALLEGRO_BITMAP *pauseButton, ALLEGRO_BITMAP *pauseScreen, ALLEGRO_TIMER *timer,ALLEGRO_TIMER * pauseTimer, ALLEGRO_BITMAP *resume)
{

    drawPausescreen(c, pauseButton, pauseScreen);
    al_draw_bitmap(resume, RESUMEx, RESUMEy, NULL);
    al_flip_display();
    al_clear_to_color(al_map_rgb(0,0,0));
    al_rest(0.1);
    al_start_timer(timer);
    al_stop_timer(pauseTimer);

}

void startNewgame(Decks c[], ALLEGRO_BITMAP *pauseButton, ALLEGRO_BITMAP *pauseScreen, ALLEGRO_TIMER *timer,ALLEGRO_TIMER * pauseTimer, ALLEGRO_BITMAP *newGame)
{
    drawPausescreen(c, pauseButton, pauseScreen);
    al_draw_bitmap(newGame, NEWGAMEx, NEWGAMEy, NULL);
    al_flip_display();
    al_clear_to_color(al_map_rgb(0,0,0));
    al_rest(0.1);
    al_set_timer_count(timer, 0);
    al_stop_timer(pauseTimer);
    al_start_timer(timer);

}

void fixScore(int &score)
{

    if (score < 0) {
        score = 0;
    }

}

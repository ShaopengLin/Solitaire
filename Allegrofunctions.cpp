#include "CardStruct.h"

//called in main: initialize allegro
void initializeAllegro()
{
    al_init();
    al_init_image_addon();
    al_init_native_dialog_addon();
    al_install_mouse();
    al_init_font_addon();
    al_init_ttf_addon();

}

//called in main: draw the seconds on the screen
void drawSeconds(ALLEGRO_FONT *font, ALLEGRO_TIMER *timer, int seconds)
{

    char cseconds[4];

    //add up the timer and the seconds in save file, and set restrictions
    seconds = fixSeconds(timer, seconds);

    //put the value of seconds into a string
    sprintf(cseconds,"%d",seconds - 60*(seconds/60));

    al_draw_text(font, al_map_rgb(255,255,255), 335+(calculateExtraspace(seconds, timer)*10), 30,NULL, cseconds);

}

//called in main: draw the minutes on the screen
void drawMinutes(ALLEGRO_FONT *font, ALLEGRO_TIMER *timer, int seconds)
{
    char cminutes[4];

    //add up the timer and the seconds in save file, and set restrictions
    seconds = fixSeconds(timer, seconds);

    //put value of seconds into a string
    sprintf(cminutes,"%d",seconds/60);

    al_draw_text(font, al_map_rgb(255,255,255), 300, 10,NULL, "Time:");
    al_draw_text(font,al_map_rgb(255,255,255), 300, 30,NULL, cminutes);
    al_draw_text(font,al_map_rgb(255,255,255), 320+(calculateExtraspace(seconds, timer)*10), 30,NULL, ":");

}

//called in main: draw move made on screen
void drawMovescount(ALLEGRO_FONT *font, int movesCounter)
{
    char cMovesCounter[10];

    //put value of moves into a string
    sprintf(cMovesCounter,"%d",movesCounter);
    al_draw_text(font, al_map_rgb(255,255,255), 175, 10,NULL, "Moves:");
    al_draw_text(font,al_map_rgb(255,255,255), 175, 30,NULL, cMovesCounter);

}

//called in main: draw score on screen
void drawScore(ALLEGRO_FONT *font, int &score)
{
    char cScore[10];

    //if score is lower than 0, score = 0;
    score = fixScore(score);

    //put value of score into a string
    sprintf(cScore,"%d",score);
    al_draw_text(font, al_map_rgb(255,255,255), 50, 10,NULL, "Scores:");
    al_draw_text(font,al_map_rgb(255,255,255), 50, 30,NULL, cScore);

}

//called in main and some functions in the pause part: draw the pause screen
void drawPausescreen(Decks c[], ALLEGRO_BITMAP *pauseButton, ALLEGRO_BITMAP *pauseScreen)
{

    al_draw_bitmap(pauseButton,400,20,NULL);

    al_draw_bitmap(pauseScreen, 10, 100, NULL);

}

//called in main: resume the game
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

//called in main: animation for pressing new game
void drawNewgamePressed(Decks c[], ALLEGRO_BITMAP *pauseButton, ALLEGRO_BITMAP *pauseScreen, ALLEGRO_BITMAP *newGame)
{

    drawPausescreen(c, pauseButton, pauseScreen);

    al_draw_bitmap(newGame, NEWGAMEx, NEWGAMEy, NULL);
    al_flip_display();
    al_clear_to_color(al_map_rgb(0,0,0));
    al_rest(0.1);

}

//called in drawScore: score cannot be lower than 0
int fixScore(int score)
{

    if (score < 0) {

        return 0;

    }

}

//called in drawSeconds and drawMinutes: set maximum time that can be shown
int fixSeconds(ALLEGRO_TIMER *timer, int seconds)
{

    //if time used is over 999 minutes
    if (seconds + al_get_timer_count(timer) > 59940) {

        return 59940;

    } else {

        return seconds + al_get_timer_count(timer);

    }
}
//called in main: draw the win screen
void winningScreen(Decks c[], ALLEGRO_BITMAP *winScreen)
{

    al_draw_bitmap(winScreen, 0, 300, NULL);

}

//called in main: determine if the user has beaten the highscore and draws a medal
void determineBeathighscores(ALLEGRO_BITMAP *medal, ALLEGRO_TIMER *timer, int score, int &highScore, int seconds, int &quickest, int movesCounter, int &leastMove)
{

    if ((al_get_timer_count(timer)+seconds) < quickest || quickest == 0) {

        quickest = (al_get_timer_count(timer)+seconds);
        al_draw_bitmap(medal, 260, 10,NULL);
    }

    if (highScore <= score) {

        highScore = score;
        al_draw_bitmap(medal, 10, 10,NULL );

    }
    if (movesCounter < leastMove || leastMove == 0) {

        leastMove = movesCounter;
        al_draw_bitmap(medal, 135,10,NULL);

    }
}

//called in main: calculate the score relative to the time used
int calculateUnltimateScore(int score, int movesCounter, int seconds, ALLEGRO_TIMER *timer)
{

    return score + (240- (seconds+al_get_timer_count(timer)))*100 - movesCounter;

}

//called in main: reset the nessecery states and restart the game
void startNewgame(Decks c[], int &movesCounter, int &seconds, int &score, ALLEGRO_TIMER *timer, ALLEGRO_TIMER *pauseTimer, ALLEGRO_BITMAP *card, ALLEGRO_BITMAP *background, ALLEGRO_EVENT_QUEUE *event_queue)
{

    //reset scores
    movesCounter = 0;
    seconds = 0;
    score = 0;

    //restart timer and game
    al_set_timer_count(timer, 0);
    al_stop_timer(pauseTimer);
    al_start_timer(timer);
    cardInfodistribution(c);
    dealCardsIn(c, card, background, event_queue);

}

//called in drawSeconds and drawMinutes: calculate the extra space if minutes are to increase,
int calculateExtraspace(int seconds, ALLEGRO_TIMER *timer)
{
        if (seconds + al_get_timer_count(timer) >= 6*pow(10, 3)){

            return 2;

        }
        else if (seconds + al_get_timer_count(timer) >= 6*pow(10, 2)) {

            return 1;

        }
        else if (seconds + al_get_timer_count(timer) > 0){

            return 0;

        }
}





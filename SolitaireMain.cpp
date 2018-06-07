
#include "CardStruct.h"

int main(int argc, char *argv[])
{

    initializeAllegro();

    //create display;
    ALLEGRO_DISPLAY *display;
    if(!display) {
        al_show_native_message_box(NULL,"Error", NULL, "Could not create display", NULL,NULL);
    }

    //initialize display
    display = al_create_display(ScreenWidth, ScreenHeight);

    FILE *readInsave = fopen("savefile.txt", "r");
    FILE *writeInsave = fopen("replacesave.txt", "w");
    FILE *readInscore = fopen("scorefile.txt", "r");
    FILE *writeInscore = fopen("replacescore.txt", "w");

    ALLEGRO_FONT *font = al_load_font("Roboto-Medium.ttf",20,NULL);
    ALLEGRO_FONT *startingFont = al_load_font("Hug Me Tight - TTF.ttf", 40, NULL);
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    ALLEGRO_EVENT_QUEUE *pause_queue = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1.0/FPS);
    ALLEGRO_TIMER *pauseTimer = al_create_timer(1.0/FPS);

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(pause_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    //initialize background, card, back of card spritesheet
    ALLEGRO_BITMAP *background = al_load_bitmap("background.png");
    ALLEGRO_BITMAP *card = al_load_bitmap("cards.png");
    ALLEGRO_BITMAP *pauseButton = al_load_bitmap("pause.png");
    ALLEGRO_BITMAP *pauseScreen = al_load_bitmap("pausescreen.png");
    ALLEGRO_BITMAP *newGame = al_load_bitmap("newgame.png");
    ALLEGRO_BITMAP *resume = al_load_bitmap("resume.png");
    ALLEGRO_BITMAP *winScreen = al_load_bitmap("winscreen.png");
    ALLEGRO_BITMAP *medal = al_load_bitmap("medal.png");

    //create mouse state
    ALLEGRO_MOUSE_STATE state;

    //set window position
    al_set_window_position(display,700, 20);

    //initialize randomizer
    srand(time(NULL));



    Decks cards[52];
    bool done = false, cardMoving = false, mouseOnbackup = false, asked = false;
    char saved[10];
    int largestLayer = 0;
    int seconds = 0;
    int movesCounter = 0;
    int score = 0;
    int highScore = 0, quickest = 0, leastMove = 0;

    rewind(readInsave);
    rewind(readInscore);
    fscanf(readInscore, "%d", &quickest);
    fscanf(readInscore, "%d", &highScore);
    fscanf(readInscore, "%d", &leastMove);
    fscanf(readInscore, "%s", saved);

    if (readFromfile(cards, display,readInsave, readInscore,timer,seconds, score, movesCounter, saved) != 0) {

        al_draw_text(startingFont, al_map_rgb(255,255,255), 150, ScreenHeight/2, NULL, "Tap to Start");
        al_flip_display();
        al_clear_to_color(al_map_rgb(0,0,0));

        while (1) {

            ALLEGRO_EVENT events;
            al_wait_for_event(event_queue, &events);


            if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {

            return 0;


            }
            al_get_mouse_state(&state);
            if (al_mouse_button_down(&state, state.buttons & 1)){
                al_rest(0.2);
                //distribute information to the 52 cards
                cardInfodistribution(cards);
                dealCardsIn(cards, card, background, event_queue,done);
                break;
            }
        }
    }

    //Actual Game

    determineLargestlayer(cards,largestLayer);
    al_start_timer(timer);

    while(!done) {
        ALLEGRO_EVENT events;
        al_wait_for_event(event_queue, &events);

        if (!asked) {
            if (autoComplete(cards,card,background,timer,font,largestLayer,score,movesCounter,seconds,display) == 2) {

                asked = true;
            }
        }

        if(determineWon(cards)){

        al_stop_timer(timer);

        al_start_timer(pauseTimer);

        while (1){
        al_wait_for_event(event_queue, &events);
        if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {

            done = true;
            break;

        }

        printb(background);

        drawMinutes(font,timer,seconds);

        drawSeconds(font, timer, seconds);

        drawMovescount(font,movesCounter);

        drawScore(font, score);

        createCards(cards, card, largestLayer);

        determineBeathighscores(medal, timer,score, highScore,seconds, quickest, movesCounter, leastMove);

        winningScreen(cards, winScreen);
        }
        al_stop_timer(timer);

        al_start_timer(pauseTimer);

        }

        done = determineWon(cards);


        //if clicked X button top right
        if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {

            done = true;

        }

        al_get_mouse_state(&state);

        if (al_mouse_button_down(&state, state.buttons & 1)) {

            if (pauseHitbox(events)) {

                al_stop_timer(timer);

                al_start_timer(pauseTimer);

                while (1) {

                    al_wait_for_event(event_queue, &events);

                    if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {

                        al_start_timer(timer);
                        al_stop_timer(pauseTimer);

                        done = true;
                        break;

                    }

                    printb(background);

                    drawMinutes(font,timer,seconds);

                    drawSeconds(font, timer, seconds);

                    drawMovescount(font,movesCounter);

                    drawScore(font, score);

                    createCards(cards, card, largestLayer);

                    al_get_mouse_state(&state);
                    if(al_mouse_button_down(&state, state.buttons & 1)) {

                        if(resumeHitbox(events) == true) {

                            resumeGame(cards, pauseButton, pauseScreen, timer, pauseTimer,resume);
                            break;

                        } else if(startHitbox(events)) {

                            startNewgame(cards, pauseButton, pauseScreen, timer, pauseTimer,newGame);

                            movesCounter = 0;
                            seconds = 0;
                            score = 0;

                            cardInfodistribution(cards);
                            dealCardsIn(cards, card, background, event_queue,done);
                            break;
                        }
                    }

                    drawPausescreen(cards, pauseButton, pauseScreen);

                    al_flip_display();
                    al_clear_to_color(al_map_rgb(0,0,0));
                }
            }
        }

        //Control cards movement
        cardMovements(cards,event_queue,state, events, cardMoving, largestLayer, mouseOnbackup, movesCounter, score);

        determineLargestlayer(cards,largestLayer);

        //print back ground
        printb(background);

        revealCard(cards);

        //draws the 52 cards
        createCards(cards, card, largestLayer);

        drawMinutes(font,timer,seconds);

        drawSeconds(font, timer, seconds);

        drawMovescount(font,movesCounter);

        drawScore(font, score);

        // draw to screen
        al_flip_display();

        //clear screen to prevent overlapping
        al_clear_to_color(al_map_rgb(0,0,0));
    }


    saveTofile(cards, writeInsave, writeInscore, timer, seconds,quickest,score,highScore,movesCounter,leastMove);

    rearrageFiles(writeInsave, writeInscore, readInsave, readInscore);

    //destroy pointers
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_bitmap(background);
    al_destroy_bitmap(card);
    al_destroy_timer(timer);
    return 0;
}

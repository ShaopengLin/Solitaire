//Chris Lin    ICS3U    Lindsay Cullum    June 12, 2018
//This program is a space themed version of the classical game solitaire.
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

    //create file pointers for reading and writings
    FILE *readInsave = fopen("savefile.txt", "r");
    FILE *writeInsave = fopen("replacesave.txt", "w");
    FILE *readInscore = fopen("scorefile.txt", "r");
    FILE *writeInscore = fopen("replacescore.txt", "w");

    //create fonts
    ALLEGRO_FONT *font = al_load_font("Roboto-Medium.ttf",20,NULL);
    ALLEGRO_FONT *startingFont = al_load_font("Hug Me Tight - TTF.ttf", 40, NULL);

    //create event queue
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();

    //create timers
    ALLEGRO_TIMER *timer = al_create_timer(1.0/FPS);
    ALLEGRO_TIMER *pauseTimer = al_create_timer(1.0/FPS);

    //register event sources
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    //initialize background, card, back of card spritesheet
    ALLEGRO_BITMAP *background = al_load_bitmap("background.png");
    ALLEGRO_BITMAP *cardBitmap = al_load_bitmap("cards.png");
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
    bool done = false, cardMoving = false, mouseOnbackup = false, askedForautocomplete = false;
    char saved[10];
    int largestLayer = 0;
    int seconds = 0;
    int movesCounter = 0;
    int score = 0;
    int highScore = 0, quickest = 0, leastMove = 0;

    //rewind files
    rewind(readInsave);
    rewind(readInscore);

    //scan for high scores
    fscanf(readInscore, "%d", &quickest);
    fscanf(readInscore, "%d", &highScore);
    fscanf(readInscore, "%d", &leastMove);
    fscanf(readInscore, "%s", saved);

    //read save file, if no file, show start screen
    if (readFromfile(cards, display,readInsave, readInscore,timer,seconds, score, movesCounter, saved) != 0) {

        //draw start screen
        al_draw_text(startingFont, al_map_rgb(255,255,255), 150, ScreenHeight/2, NULL, "Click to Start");
        al_flip_display();
        al_clear_to_color(al_map_rgb(0,0,0));

        while (1) {

            ALLEGRO_EVENT events;
            al_wait_for_event(event_queue, &events);

            // close the program
            if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {

                return 0;

            }
            al_get_mouse_state(&state);

            //when clicked on screen, start the game
            if (al_mouse_button_down(&state, state.buttons & 1)) {

                al_rest(0.2);
                //distribute information to the 52 cards
                cardInfodistribution(cards);
                dealCardsIn(cards, cardBitmap, background, event_queue);

                break;
            }
        }
    }

    //Actual Game

    //determine the total amount of layers
    determineLargestlayer(cards,largestLayer);
    al_start_timer(timer);

    while(!done) {
        ALLEGRO_EVENT events;
        al_wait_for_event(event_queue, &events);

        //ask the user once if they want to auto complete
        if (!askedForautocomplete) {

            //auto complete the game
            if (autoComplete(cards,cardBitmap,background,timer,font,largestLayer,score,movesCounter,seconds,display) == 2) {

                askedForautocomplete = true;
            }
        }


        if(determineWon(cards)) {

            //stop the main game timer and start the pause screen timer to not affect in game time count
            al_stop_timer(timer);

            al_start_timer(pauseTimer);

            //calculate the score based on how much time the user used
            score = calculateUnltimateScore(score,movesCounter, seconds, timer);

            //draw the needed game screens
            al_draw_bitmap(background,0,0,NULL);

            drawMinutes(font,timer,seconds);

            drawSeconds(font, timer, seconds);

            drawMovescount(font,movesCounter);

            drawScore(font, score);

            createCards(cards, cardBitmap, largestLayer);

            //determine if the user has beaten his history high scores
            determineBeathighscores(medal, timer,score, highScore,seconds, quickest, movesCounter, leastMove);

            //draw the winning screen
            winningScreen(cards, winScreen);

            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));

            //ask the user if want to play again
            if (al_show_native_message_box(display,"Message", "Do you want to play again?", "Yes to replay, No to quit", NULL, ALLEGRO_MESSAGEBOX_YES_NO)== 1) {

                //restart the game
                startNewgame(cards, movesCounter, seconds, score, timer, pauseTimer, cardBitmap, background, event_queue);
            } else {
            }
        }

        //exit the game
        done = determineWon(cards);


        //if clicked X button top right
        if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {

            //exit the game
            done = true;

        }

        al_get_mouse_state(&state);


        if (al_mouse_button_down(&state, state.buttons & 1)) {

            //if user clicked the pause button
            if (pauseHitbox(events)) {

                //stop the main game timer and start the pause screen timer to not affect in game time count
                al_stop_timer(timer);

                al_start_timer(pauseTimer);

                while (1) {

                    al_wait_for_event(event_queue, &events);

                    //exit program if pressed close
                    if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {

                        done = true;
                        break;

                    }

                    //draw game screens
                    al_draw_bitmap(background,0,0,NULL);

                    drawMinutes(font,timer,seconds);

                    drawSeconds(font, timer, seconds);

                    drawMovescount(font,movesCounter);

                    drawScore(font, score);

                    createCards(cards, cardBitmap, largestLayer);

                    al_get_mouse_state(&state);


                    if(al_mouse_button_down(&state, state.buttons & 1)) {

                        //if the user hit resume button
                        if(resumeHitbox(events) == true) {

                            //resume the game
                            resumeGame(cards, pauseButton, pauseScreen, timer, pauseTimer,resume);
                            break;

                            //if the user hit restart button
                        } else if(restartHitbox(events)) {

                            //animation of newGame button
                            drawNewgamePressed(cards, pauseButton, pauseScreen,newGame);

                            //restart the game
                            startNewgame(cards, movesCounter, seconds, score, timer, pauseTimer, cardBitmap, background, event_queue);
                            break;
                        }
                    }

                    //draws the pause screen
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
        al_draw_bitmap(background,0,0,NULL);

        //reveal the cards in play area after user moved the card on top of it and stacked it
        revealCard(cards);

        //draws the 52 cards
        createCards(cards, cardBitmap, largestLayer);

        //draw the high scores, time counts, and move counts on top
        drawMinutes(font,timer,seconds);

        drawSeconds(font, timer, seconds);

        drawMovescount(font,movesCounter);

        drawScore(font, score);

        // draw to screen
        al_flip_display();

        //clear screen to prevent overlapping
        al_clear_to_color(al_map_rgb(0,0,0));
    }

    //save scores and game data in to the save and score file
    saveTofile(cards, writeInsave, writeInscore, timer, seconds,quickest,score,highScore,movesCounter,leastMove);

    //rename and delete files
    rearrageFiles(writeInsave, writeInscore, readInsave, readInscore);

    //destroy pointers
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_bitmap(background);
    al_destroy_bitmap(cardBitmap);
    al_destroy_timer(timer);

    return 0;
}

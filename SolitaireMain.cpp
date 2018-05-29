
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

    //create and register event queues
    ALLEGRO_FONT *font = al_load_font("Roboto-Medium.ttf",20,NULL);
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1.0/FPS);
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());

    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    //al_hide_mouse_cursor(display);

    //initialize background, card, back of card spritesheet
    ALLEGRO_BITMAP *background = al_load_bitmap("background.png");
    ALLEGRO_BITMAP *card = al_load_bitmap("cards.png");
    ALLEGRO_BITMAP *pause = al_load_bitmap("pause.png");
   /*ALLEGRO_BITMAP *newBackground = al_create_bitmap(al_get_bitmap_width(background), al_get_bitmap_height(background));

    al_set_target_bitmap(newBackground);

    al_draw_bitmap(background,0,0,NULL);
    al_draw_bitmap_region(card,910, 200, 70, 100,backUpx,backUpy, NULL);
    al_draw_bitmap_region(card,910,100,70,100, Ax, Ay,NULL);
    al_draw_bitmap_region(card,910,100,70,100, Ax+75, Ay,NULL);
    al_draw_bitmap_region(card,910,100,70,100, Ax+150, Ay,NULL);
    al_draw_bitmap_region(card,910,100,70,100, Ax+225, Ay,NULL);
    al_draw_bitmap_region(card,910,0,70,100,Ax, 210,NULL);
    al_draw_bitmap_region(card,910,0,70,100,Ax + 75, 210,NULL);
    al_draw_bitmap_region(card,910,0,70,100,Ax + 150, 210,NULL);
    al_draw_bitmap_region(card,910,0,70,100,Ax + 225, 210,NULL);
    al_draw_bitmap_region(card,910,0,70,100,Ax + 300, 210,NULL);
    al_draw_bitmap_region(card,910,0,70,100,Ax + 375, 210,NULL);
    al_draw_bitmap_region(card,910,0,70,100,Ax + 450, 210,NULL);
    al_draw_bitmap(pause,400,20,NULL);


    al_save_bitmap("newbackground.png", newBackground);

    al_set_target_bitmap(al_get_backbuffer(display));
*/
    //create mouse state
    ALLEGRO_MOUSE_STATE state;

    //set window position
    al_set_window_position(display,700, 20);

    //initialize randomizer
    srand(time(NULL));

    Decks cards[52];
    bool done = false, cardMoving = false, mouseOnbackup = false;
    int largestLayer = 0;
    int seconds = 0, minutes = 0;
    int movesCounter = 0;
    int score = 0;

    //distribute information to the 52 cards
    cardInfodistribution(cards);


    dealCardsIn(cards, card, background, event_queue,done);
    determineLargestlayer(cards,largestLayer);
    //Actual Game

    al_start_timer(timer);
    while(!done) {
        ALLEGRO_EVENT events;
        al_wait_for_event(event_queue, &events);

        //if clicked X button top right
        if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            done = true;
        }



        //Control cards movement
        cardMovements(cards,event_queue,state, events, cardMoving, largestLayer, mouseOnbackup, movesCounter, score);

        determineLargestlayer(cards,largestLayer);

        //print back ground
        printb(background);

        animationFlip(cards);

        //draws the 52 cards
        createCards(cards, card, largestLayer);

        drawTime(font, timer, seconds, minutes);

        drawMovescount(font,movesCounter);

        drawScore(font, score);


        // draw to screen
        al_flip_display();

        //clear screen to prevent overlapping
        al_clear_to_color(al_map_rgb(0,0,0));

        winningFunction(cards,done);



    }

    //destroy pointers
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_bitmap(background);
    al_destroy_bitmap(card);
    //al_destroy_timer(timer);
    return 0;
}

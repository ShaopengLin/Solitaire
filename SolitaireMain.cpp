
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
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());

    //al_hide_mouse_cursor(display);

    //initialize background, card spritesheet
    ALLEGRO_BITMAP *background = al_load_bitmap("background.jpg");
    ALLEGRO_BITMAP *card = al_load_bitmap("cards.png");
    ALLEGRO_BITMAP *backcard = al_load_bitmap("backcard.png");

    //create mouse state
    ALLEGRO_MOUSE_STATE state;

    //set window position
    al_set_window_position(display,700, 20);

    //initialize randomizer
    srand(time(NULL));

    Decks cards[52];
    bool done = false, cardMoving = false;
    int largestLayer = 0;

    //distribute information to the 52 cards
    cardInfodistribution(cards);

   dealCardsIn(cards, card, background, event_queue, backcard,done);
   determineLargestlayer(cards,largestLayer);
    //Actual Game

    while(!done) {
        ALLEGRO_EVENT events;
        al_wait_for_event(event_queue, &events);

        //if clicked X button top right
        if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            done = true;
        }



        //Control cards movement
        cardMovements(cards,event_queue,state, events, cardMoving, largestLayer);

        determineLargestlayer(cards,largestLayer);

        //print back ground
        printb(background);

        animationFlip(cards);

        //draws the 52 cards
        createCards(cards, card, backcard, largestLayer);

        // draw to screen
        al_flip_display();

        //clear screen to prevent overlapping
        al_clear_to_color(al_map_rgb(0,0,0));

    }

    //destroy pointers
    al_destroy_display(display);
    al_destroy_bitmap(background);
    al_destroy_bitmap(card);

    return 0;
}

#include "CardStruct.h"

//deal cards
void cardInfodistribution (Decks c[])
{

    int numbersDealt = 0;
    int suitsDealt = 0;
    int thirteenNumbers[13] = {1,2,3,4,5,6,7,8,9,10,11,12,13};
    int fourSuits[4] = {1, 2, 3, 4};

    // gives a card a number and a suit
    for (int i = 0; i < 52; i++) {

        //number
        c[i].number = thirteenNumbers[rand()%(13 - numbersDealt)];
        c[0].counterNumber[c[i].number]++;

        //calls to check if all 13 of a suit is used up or all 4 numbers are used up
        cardNumberremove(c,numbersDealt, i, thirteenNumbers);
    }
    //suit
    cardSuitremoveAnddistribute(c,fourSuits,suitsDealt);
}

//determine the largest Layer
void determineLargestlayer(Decks c[], int &largestLayer)
{
    int i = 0;
    largestLayer = c[0].layer.numbers;
    for (i = 0; i < 52; i++) {
        if (c[i].layer.numbers > largestLayer) {
            largestLayer = c[i].layer.numbers;
        }
    }
}

//draws the 52 cards constantly depending on their layering
void createCards(Decks c[], ALLEGRO_BITMAP *card, ALLEGRO_BITMAP *backcard, int largestLayer)
{
    int i = 0, j = 1;

    //print the images depending on if a card should be revealed or not
    for (j = 1; j <= largestLayer; j++) {

        for (i = 0; i < 52; i++) {

            if (c[i].layer.numbers == j) {
                if (c[i].revealed) {

                    al_draw_bitmap_region(card, (c[i].number - 1) * 70, (c[i].suit -1) * 100, 70, 100,c[i].x, c[i].y, NULL);
                } else {
                    al_draw_bitmap_region(backcard,0, 0, 70, 100,c[i].x, c[i].y, NULL);
                }
            }
        }
    }

}

//controls the cards' movement
void cardMovements(Decks c[],ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_MOUSE_STATE &state, ALLEGRO_EVENT events, bool &cardMoving, int &largestLayer)
{

    int i = 0, j = 0;

    al_get_mouse_state(&state);
    for (i = 0; i < 52; i++) {

        // if left mouse button is pressed down on a card's hit box
        if (al_mouse_button_down(&state, state.buttons & 1)) {

            //if the card is revealed to the user
            if(c[i].revealed == true) {

                if (events.mouse.x >= c[i].x && events.mouse.x <= c[i].x + 70 && events.mouse.y >= c[i].y && events.mouse.y <= c[i].y+100) {

                    //if a card is already moving
                    if (cardMoving == true) {

                    } else {

                        //determine if there are more cards under the card that the user is dragging and back up information
                        for (j = 0; j < 52; j++) {

                            if (c[j].column.numbers == c[i].column.numbers) {

                                if (c[j].layer.numbers > c[i].layer.numbers) {

                                    c[j].backupx = c[j].x;
                                    c[j].backupy = c[j].y;
                                    c[j].follow = true;
                                    c[j].layer.origin = c[j].layer.numbers;
                                    c[j].layer.numbers += largestLayer;
                                }
                            }
                        }

                        // back information for the card user is dragging
                        c[i].backupx = c[i].x;
                        c[i].backupy = c[i].y;
                        c[i].moving = true;
                        cardMoving = true;
                        c[i].layer.origin = c[i].layer.numbers;
                        c[i].layer.numbers += largestLayer;

                    }
                }
            }
        }

        //if the mouse button is lifted
         else {

            if(c[i].moving == true) {

                //determine if the card user is dragging can be stack on another card
                firstCardfixposition(c, i);

                //determine where the cards following the dragging card should go based on the dragging card information
                followCardfixposition(c, i);
            }


            c[i].moving = false;
            cardMoving = false;
        }

        //move with the mouse's displacement
        if(c[i].moving == true) {

            if (c[i].x > ScreenWidth) {
                c[i].x = ScreenWidth;
            } else if(c[i].x < 0) {
                c[i].x = 0;
            } else if (c[i].y < 0) {
                c[i].y = 0;
            } else if(c[i].y > ScreenHeight) {
                c[i].y = ScreenWidth;
            } else {
                c[i].x += events.mouse.dx;
                c[i].y += events.mouse.dy;
            }

            //the following cards moves with the dragging card
            for (j = 0; j< 52; j++) {
                if (c[j].follow) {
                    c[j].x = c[i].x;
                    c[j].y = c[i].y + (c[j].layer.origin - c[i].layer.origin)*30;
                }
            }
        }
    }
}


//print the cards on screen at the start of the game
void dealCardsIn(Decks c[], ALLEGRO_BITMAP *card, ALLEGRO_BITMAP *background, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_BITMAP *backcard, bool &done)
{


    int j, i, k;
    int endOfrow = 0, multiplier = 0, rowlimit = 7;
    float movespeedx, movespeedy;

    //gives identity for the 28 backup deck up top
    for (i = 28; i < 52; i++) {
        c[i].x = 463;
        c[i].y = 74;
        c[i].layer.numbers = i;
        c[i].revealed = false;
    }

    //refreshes each cards' identity after it has been dealt
    for (i = 0; i < 28; i++) {

        //every row, there will be one card less
        if (endOfrow == rowlimit) {
            multiplier++;
            rowlimit--;
            endOfrow = 0;
        }

        //move speed based on the card's position to where they should go
        movespeedx = (c[i].x - (8+(75*endOfrow)+(75*multiplier))) / 30;
        movespeedy = (c[i].y - (195 + (30*multiplier))) / 30;

        // first card of a column will be revealed
        if (endOfrow == 0) {
            c[i].revealed = true;

        } else {
            c[i].revealed = false;

        }

        //move the card to a position on screen and fixing the card's position
        for (j = 0; j < 30; j++) {

            c[i].x -= movespeedx;
            c[i].y -= movespeedy;
            printb(background);
            if(j == 29) {
                if(c[i].x != (8+(75*endOfrow)+(75*multiplier))) {
                    (8+(75*endOfrow)+(80*multiplier));
                }
                if((c[i].y != (195 + (30*multiplier)))) {
                    c[i].y = (195 + (30*multiplier));
                }
            }

            // draw the card
            for (k = 0; k < 52; k++) {

                if (c[k].revealed) {

                    al_draw_bitmap_region(card, (c[k].number - 1) * 70, (c[k].suit -1) * 100, 70, 100,c[k].x, c[k].y, NULL);
                } else {
                    al_draw_bitmap_region(backcard,0, 0, 70, 100,c[k].x, c[k].y, NULL);
                }
            }

            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));
        }
        endOfrow++;

        //distributing layer information to cards
        c[i].layer.numbers = multiplier + 1;
        c[i].column.numbers = endOfrow+multiplier;
        c[endOfrow+multiplier].layer.totalLayer++;
    }
}











//all the functions here are called in the main function

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
void createCards(Decks c[], ALLEGRO_BITMAP *card, int largestLayer)
{
    int i = 0, j = 1;

    //print the images depending on if a card should be revealed or not
    for (j = 1; j <= largestLayer; j++) {

        for (i = 0; i < 52; i++) {

            if (c[i].layer.numbers == j) {
                if (c[i].revealed) {

                    al_draw_bitmap_region(card, (c[i].number - 1) * 70, (c[i].suit -1) * 100, 70, 100,c[i].x, c[i].y, NULL);
                } else {
                    al_draw_bitmap_region(card,910, 300, 70, 100,c[i].x,c[i].y, NULL);
                }
            }
        }
    }
}



//controls the cards' movement
void cardMovements(Decks c[],ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_MOUSE_STATE &state, ALLEGRO_EVENT events, bool &cardMoving, int &largestLayer, bool &mouseOnbackup, int &movesCounter,  int &score)
{

    int i = 0, j = 0, firstLayer = true;

    al_get_mouse_state(&state);
    for (i = 0; i < 52; i++) {

        // if left mouse button is pressed down on a card's hit box
        if (al_mouse_button_down(&state, state.buttons & 1)) {

            if (events.mouse.x >= c[i].x && events.mouse.x <= c[i].x + 70 && events.mouse.y >= c[i].y && events.mouse.y <= c[i].y+100) {

                //if the card is revealed to the user
                if (c[i].revealed == true) {

                    for (j = 0; j < 52; j++) {
                        if (j != i) {
                            if (c[j].revealed){
                            if (events.mouse.x >= c[j].x && events.mouse.x <= c[j].x + 70 && events.mouse.y >= c[j].y && events.mouse.y <= c[j].y+100) {
                                if(c[j].layer.numbers > c[i].layer.numbers) {
                                    firstLayer = false;

                                }
                            }
                            }
                        }
                    }
                    if(firstLayer == true) {
                        //determine if there are more cards under the card that the user is dragging and back up information for both while changing their state
                        changeCardsstate(c, cardMoving, largestLayer, i);
                    }
                    else{
                        firstLayer = true;
                    }
                }
            }

            //if the mouse is in the area of the backup Deck
            else if (events.mouse.x >= backUpx && events.mouse.x <= backUpx + 70 && events.mouse.y >= backUpy && events.mouse.y <= backUpy+100) {
                if (!cardMoving) {
                    mouseOnbackup = true;
                }
            }
        }

        //if the mouse button is lifted
        else {

            if(c[i].moving == true) {

                //determine if the card is in the A column
                columnAfixposition(c,i, movesCounter, score);

                //determine if the card user is dragging is in the K column
                columnKfixposition(c, i, movesCounter);

                //determine if the card user is dragging can be stack on another card
                firstCardfixposition(c, i, movesCounter);



                //determine where the cards following the dragging card should go based on the dragging card information
                followCardfixposition(c, i);

            }

            if(mouseOnbackup) {

                if (events.mouse.x >= backUpx && events.mouse.x <= backUpx + 70 && events.mouse.y >= backUpy && events.mouse.y <= backUpy+100) {
                    //controls the distribution and animation of the backup deck
                    manageBackupcard(c, largestLayer, movesCounter, score);
                    mouseOnbackup = false;
                }
            }

            c[i].moving = false;
            cardMoving = false;

        }

        //move with the mouse's displacement and the following cards moves with the dragging card
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
void dealCardsIn(Decks c[], ALLEGRO_BITMAP *card, ALLEGRO_BITMAP *background, ALLEGRO_EVENT_QUEUE *event_queue, bool &done)
{


    int j, i, k, layerCount = 24;
    int endOfrow = 0, multiplier = 0, rowlimit = 7;
    float movespeedx, movespeedy;

    //gives identity for the 24 backup deck up top
    for (i = 28; i < 52; i++) {
        c[i].x = backUpx;
        c[i].y = backUpy;
        c[i].column.numbers = 0;
        c[i].layer.numbers = layerCount;
        c[0].backupDeck.totalLayer = i;
        c[i].revealed = false;
        layerCount--;
    }
    layerCount = 24;


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
        movespeedy = (c[i].y - (210 + (30*multiplier))) / 30;

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
                if((c[i].y != (210 + (30*multiplier)))) {
                    c[i].y = (210 + (30*multiplier));
                }
            }

            // draw the card
            for (k = 0; k < 52; k++) {

                if (c[k].revealed) {

                    al_draw_bitmap_region(card, (c[k].number - 1) * 70, (c[k].suit -1) * 100, 70, 100,c[k].x, c[k].y, NULL);
                } else {
                    al_draw_bitmap_region(card, 910, 300, 70, 100,c[k].x,c[k].y, NULL);
                }
            }

            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));
        }
        endOfrow++;

        //distributing layer information to cards
        c[i].layer.numbers = multiplier + 1;
        c[i].column.numbers = endOfrow+multiplier;
        c[c[i].column.numbers].column.dimensionx = 8+(75*(endOfrow-1)+(75*multiplier));
        c[c[i].column.numbers].column.dimensiony = 210;
        c[c[i].column.numbers].layer.totalLayer++;
    }
}

void animationFlip(Decks c[])
{

    for (int i = 0; i < 28; i++) {
        if (c[c[i].column.numbers].layer.totalLayer == c[i].layer.numbers) {
            if(c[i].revealed == false) {
                c[i].revealed = true;


            }
        }
    }
}









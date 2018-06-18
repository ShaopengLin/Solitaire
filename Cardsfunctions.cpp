
#include "CardStruct.h"

//called in main: giving cards random suits and numbers
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

//called in main and autoComplete: determine the largest Layer in the game currently
void determineLargestlayer(Decks c[], int &largestLayer)
{
    int i = 0;
    largestLayer = c[0].layer.numbers;

    //the card with the largest layer number becomes the largest layer
    for (i = 0; i < 52; i++) {

        if (c[i].layer.numbers > largestLayer) {

            largestLayer = c[i].layer.numbers;

        }
    }
}

//called almost everywhere that needs to draw the screen: draws the 52 cards constantly depending on their layering and states(revealed, not revealed)
void createCards(Decks c[], ALLEGRO_BITMAP *cardBitmap, int largestLayer)
{
    int i = 0, j = 1;

    //print the images depending on the state of cards
    for (j = 1; j <= largestLayer; j++) {

        for (i = 0; i < 52; i++) {

            if (c[i].layer.numbers == j) {

                if (c[i].revealed) { //draws the bitmap representing front of the card

                    al_draw_bitmap_region(cardBitmap, (c[i].number - 1) * 70, (c[i].suit -1) * 100, 70, 100,c[i].x, c[i].y, NULL);

                } else { // draws the bitmap representing back of the card

                    al_draw_bitmap_region(cardBitmap,910, 300, 70, 100,c[i].x,c[i].y, NULL);

                }
            }
        }
    }
}

//called in main: controls the cards' movement when dragging, containing lots of other functions that implement the rules when dragging
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

                    //because hit box in cards overlap with each other, this piece of code determines exactly which card the user want to click
                    for (j = 0; j < 52; j++) {

                        if (j != i) {

                            if (c[j].revealed) {

                                if (events.mouse.x >= c[j].x && events.mouse.x <= c[j].x + 70 && events.mouse.y >= c[j].y && events.mouse.y <= c[j].y+100) {

                                    if(c[j].layer.numbers > c[i].layer.numbers) { //if there are still cards on top of card i, then, card i is not the one user is trying to drag therefore skipped.

                                        //the card has cards on top of it
                                        firstLayer = false;

                                    }
                                }
                            }
                        }
                    }


                    if(firstLayer == true) {

                        //determine if there are more cards on top of the card that the user is dragging and back up information for both while changing their state
                        changeCardsstate(c, cardMoving, largestLayer, i);

                    } else {

                        //reset value for loop
                        firstLayer = true;

                    }
                }
            }

            //if the mouse is in the area of the backup Deck
            else if (events.mouse.x >= BACKUPx && events.mouse.x <= BACKUPx + 70 && events.mouse.y >= BACKUPy && events.mouse.y <= BACKUPy+100) {

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

            //if when the mouse button is lifted and the mouse is still on the back up deck
            if(mouseOnbackup) {

                if (events.mouse.x >= BACKUPx && events.mouse.x <= BACKUPx + 70 && events.mouse.y >= BACKUPy && events.mouse.y <= BACKUPy+100) {

                    //controls the distribution and animation of the backup deck
                    manageBackupcard(c, largestLayer, movesCounter, score);
                    mouseOnbackup = false;

                } else {

                    mouseOnbackup = false;

                }
            }

            c[i].moving = false;
            cardMoving = false;

        }

        //restrictions to prevent user dragging the card out side the screen
        if(c[i].moving == true) {

            if (c[i].x > ScreenWidth) {

                c[i].x = ScreenWidth;

            } else if(c[i].x < 0) {

                c[i].x = 0;

            } else if (c[i].y < 0) {

                c[i].y = 0;

            } else if(c[i].y > ScreenHeight) {

                c[i].y = ScreenHeight;

            } else {

                //move with the mouse's displacement and the following cards moves with the dragging card
                c[i].x += events.mouse.dx;
                c[i].y += events.mouse.dy;

            }

            //movement for the cards that are following the dragged card
            for (j = 0; j< 52; j++) {

                if (c[j].follow) {

                    c[j].x = c[i].x;
                    c[j].y = c[i].y + (c[j].layer.origin - c[i].layer.origin)*30;

                }
            }
        }
    }
}


//called in main and other functions involving restarting the game: print the cards on screen at the start of the game
void dealCardsIn(Decks c[], ALLEGRO_BITMAP *cardBitmap, ALLEGRO_BITMAP *background, ALLEGRO_EVENT_QUEUE *event_queue)
{


    int j, i, k;
    int endOfrow = 0, multiplier = 0, rowlimit = 7;
    float moveSpeedx, moveSpeedy;

    //reset information for cards in play area
    resetPlayareacardsInfo(c);

    //gives identity for the 24 backup deck up top
    int layerCount = 24;

    for (i = 28; i < 52; i++) {

        c[i].x = BACKUPx;
        c[i].y = BACKUPy;
        c[i].column.numbers = 0;
        c[i].layer.numbers = layerCount;
        c[0].backupDeck.totalLayer = i;
        c[i].revealed = false;
        layerCount--;
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
        moveSpeedx = (c[i].x - (8+(75*endOfrow)+(75*multiplier))) / 15;
        moveSpeedy = (c[i].y - (210 + (30*multiplier))) / 15;

        // first card of a column will be revealed
        if (endOfrow == 0) {

            c[i].revealed = true;

        } else {

            c[i].revealed = false;

        }

        //move the card to a position on screen
        for (j = 0; j < 15; j++) {

            c[i].x -= moveSpeedx;
            c[i].y -= moveSpeedy;

            //because x and y is float, fixing the card's position might be nessecery
            if(j == 14) {

                if(c[i].x != (8+(75*endOfrow)+(75*multiplier))) {

                    (8+(75*endOfrow)+(80*multiplier));

                }
                if((c[i].y != (210 + (30*multiplier)))) {

                    c[i].y = (210 + (30*multiplier));

                }
            }

            //draw the cards
            al_draw_bitmap(background,0,0,NULL);

            for (k = 0; k < 52; k++) {

                if (c[k].revealed) {

                    al_draw_bitmap_region(cardBitmap, (c[k].number - 1) * 70, (c[k].suit -1) * 100, 70, 100,c[k].x, c[k].y, NULL);

                } else {

                    al_draw_bitmap_region(cardBitmap, 910, 300, 70, 100,c[k].x,c[k].y, NULL);

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

//called in main: reveal the cards in play area after user moved the card on top of it and stacked it
void revealCard(Decks c[])
{


    for (int i = 0; i < 28; i++) {

        if (c[c[i].column.numbers].layer.totalLayer == c[i].layer.numbers) {

            if(c[i].revealed == false) {

                c[i].revealed = true;

            }
        }
    }
}

//called in main: auto complete the game
int autoComplete(Decks c[], ALLEGRO_BITMAP *cardBitmap, ALLEGRO_BITMAP *background, ALLEGRO_TIMER *timer, ALLEGRO_FONT *font, int &largestLayer, int &score, int &movesCounter, int &seconds, ALLEGRO_DISPLAY *display)
{

    //would not let user auto complete if there are non revealed cards or if there are still cards left in the back up deck
    for (int i = 0; i < 52; i++) {

        if (!c[i].revealed || c[i].column.numbers == 0) {

            return 0;

        }
    }

    //ask the user if they want to auto complete the game
    if (al_show_native_message_box(display,"Message", "Congratulations","Do you want to Auto Complete?", NULL,ALLEGRO_MESSAGEBOX_YES_NO) == 1) {

        while (!determineWon(c)) {

            for (int i = 0; i < 52; i++) {

                for (int j = 0; j < 4; j++) {

                    if (c[i].number == c[COLUMNA+j].layer.totalLayer+1) {

                        if (c[i].suit == c[COLUMNA+j].layer.suit) {

                            if (c[i].column.numbers < COLUMNA) {

                                if (c[COLUMNA+j].layer.totalLayer < 1) {

                                    //fix the layer of the card auto completing
                                    c[i].layer.numbers = c[i].number;
                                    determineLargestlayer(c, largestLayer);

                                    //animation for the card
                                    animationAutocomplete(c, cardBitmap,background,timer,font,i,j,largestLayer,score,movesCounter,seconds);

                                    //stack the A card to the A position
                                    stackAcardonA(c, i, j);

                                    score += 10;

                                } else {

                                    //fix the layer of the card auto completing
                                    c[i].layer.numbers = c[i].number;
                                    determineLargestlayer(c, largestLayer);

                                    //animation for the card
                                    animationAutocomplete(c, cardBitmap,background,timer,font,i,j,largestLayer,score,movesCounter,seconds);

                                    //stack the cards that are not A to the A position
                                    stackNonAcardonA(c, i, j);

                                    score += 10;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {

        return 2;

    }
}









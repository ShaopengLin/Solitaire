#include "CardStruct.h"

// called in cardsMovement : determine if there are more cards under the card that the user is dragging and back up information for both while changing their state
void changeCardsstate(Decks c[], bool &cardMoving, int largestLayer, int i)
{

    int j = 0;

    //if a card is already moving
    if (cardMoving == true) {

    } if (cardMoving == false) {

        for (j = 0; j < 52; j++) {

            if (c[j].revealed == true) {

                if (c[j].column.numbers == c[i].column.numbers) {

                    if (c[j].layer.numbers > c[i].layer.numbers) {

                        //back up information for the cards that are over the dragged card for following
                        makeCardfollow(c,j, largestLayer);

                    }
                }
            }
        }

        // back up information for the card user is dragging
        cardMoving = allowCardtomove(c, i, largestLayer);

    }
}

//called in cardMovement: controls the distribution and animation of the backup deck
int manageBackupcard(Decks c[], int &largestLayer, int &movesCounter, int &score)
{

    ALLEGRO_BITMAP *background = al_load_bitmap("background.png");
    ALLEGRO_BITMAP *card = al_load_bitmap("cards.png");

    int cardsLeft = 1;
    bool skipCard = false;

    //if no cards are left in the back up deck
    if (c[0].backupDeck.totalLayer > 27) {

        // check if back up card is in play area, if so, skip it.
        while (c[c[0].backupDeck.totalLayer].column.numbers != 0) {

            c[0].backupDeck.totalLayer--;
            cardsLeft++;

            if(cardsLeft == 24) {

                //return the unmoved cards back to their original position
                resetBackupcardsInfo(c);

                return 0;

            }
        }

        //reveal the cards to the user
        c[c[0].backupDeck.totalLayer].revealed = true;
        c[c[0].backupDeck.totalLayer].x -= (float)75;

        al_draw_bitmap(background,0,0,NULL);

        createCards(c,card,largestLayer);
        movesCounter++;

        c[0].backupDeck.totalLayer--;

    } else {

        //return the unmoved cards back to their original position
        resetBackupcardsInfo(c);

        score -= 5;

    }

    //destroy the local bitmaps
    al_destroy_bitmap(background);
    al_destroy_bitmap(card);

}

//called in cardMovement: stack the K cards when moved to the K position
void columnKfixposition(Decks c[], int i, int &movesCounter)
{
    int j = 1;
    for (j = 1; j < 8; j++) {

        if(kHitbox(c,i,j)) {

            if (c[i].number == 13) {

                if (c[j].layer.totalLayer == 0) {

                    //renew the information of the K card and stack it
                    c[i].x = c[j].column.dimensionx;
                    c[i].y = c[j].column.dimensiony;
                    c[i].layer.numbers = 1;
                    c[c[i].column.numbers].layer.totalLayer--;
                    c[j].layer.totalLayer++;
                    c[i].column.numbers = j;
                    movesCounter++;
                    c[i].returnOrigin = false;
                }
            }
        }
    }
}

//called in cardMovement: stack the cards when moved to the A position
int columnAfixposition(Decks c[], int i, int &movesCounter, int &score)
{
    int j = 0;

    //loop between the four A positions
    for (j = 0; j < 4; j++) {

        if(aHitbox(c,i,j)) {

            if (c[i].number == c[COLUMNA+j].layer.totalLayer+1) {

                if (c[COLUMNA+j].layer.totalLayer >= 1) {

                    if (c[i].suit == c[COLUMNA+j].layer.suit) {

                        for (int k = 0; k < 52; k++) {

                            //would not stack a card if one or more card is following
                            if (c[k].follow == true) {

                                return 0;

                            }
                        }

                        //stack cards that are not A to the A position
                        stackNonAcardonA(c, i, j);
                        movesCounter++;

                        //if the card has already scored, it would not score again
                        if(!c[i].scored) {
                            score += 10;
                            c[i].scored = true;
                        }
                    }

                } else {

                    //stack the A card to the A position
                    stackAcardonA(c, i, j);
                    movesCounter++;

                    //if the card has already scored, it would not score again
                    if(!c[i].scored) {

                        score += 10;
                        c[i].scored = true;

                    }
                }
            }
        }
    }
}

//called in firstCardfixposition: renew the information of the card and stack it
void firstCardstack(Decks c[], int i, int j)
{

    c[i].returnOrigin = false;
    c[i].x = c[j].x;
    c[i].y = c[j].y + 30;
    c[i].layer.numbers = c[j].layer.numbers + 1;
    c[c[i].column.numbers].layer.totalLayer--;
    c[c[j].column.numbers].layer.totalLayer++;
    c[i].column.numbers = c[j].column.numbers;
    c[j].stackable = false;

}

//called in firstCardfixposition: return the card to its original position
void firstCardreturn(Decks c[], int i)
{
    if (c[i].returnOrigin == true) {
        printf("firstCardreturn\n");
        c[i].x = c[i].originx;
        c[i].y = c[i].originy;
        c[i].layer.numbers = c[i].layer.origin;
    }

    c[i].returnOrigin = true;

}

//called in followCardfixposition: return the cards that are following to their original position
void followCardreturn(Decks c[], int i, int j)
{
    c[j].x = c[j].originx;
    c[j].y = c[j].originy;
    c[j].layer.numbers = c[j].layer.origin;
    c[j].x = c[i].x;
    c[j].y = c[i].y + (c[j].layer.origin - c[i].layer.origin)*30;
}

//called in followCardfixposition: renew the followed cards information and stack them
void followCardstack(Decks c[], int i, int j)
{
    c[c[j].column.numbers].layer.totalLayer--;
    c[c[i].column.numbers].layer.totalLayer++;
    c[j].column.numbers = c[i].column.numbers;
    c[j].layer.numbers = c[i].layer.numbers + (c[j].layer.origin - c[i].layer.origin);
    c[j].x = c[i].x;
    c[j].y = c[i].y + (c[j].layer.origin - c[i].layer.origin)*30;

}

//called in changeCardsstate: back up card information to let it follow the dragged card
void makeCardfollow(Decks c[], int j, int largestLayer)
{

    c[j].originx = c[j].x;
    c[j].originy = c[j].y;
    c[j].follow = true;
    c[j].layer.origin = c[j].layer.numbers;
    c[j].layer.numbers += largestLayer;

}

//called in changeCardsstate: back up dragging card's information and let it be dragged
bool allowCardtomove(Decks c[], int i, int largestLayer)
{
    c[i].originx = c[i].x;
    c[i].originy = c[i].y;
    c[i].moving = true;
    c[i].layer.origin = c[i].layer.numbers;
    c[i].layer.numbers += largestLayer;
    return true;
}

//called in columnAfixposition: stack cards that are not A to the A position
void stackNonAcardonA(Decks c[], int i, int j)
{
    printf("A\n");
    c[c[i].column.numbers].layer.totalLayer--;
    c[i].column.numbers = COLUMNA+j;
    c[COLUMNA+j].layer.totalLayer++;
    c[i].x = Ax + (j*75);
    c[i].y = Ay;
    c[i].layer.numbers = c[COLUMNA+j].layer.totalLayer;
    c[i].returnOrigin = false;

}

//called in columnAfixposition: stack the A card to the A position
void stackAcardonA(Decks c[], int i, int j)
{

    printf("A1\n");
    c[COLUMNA+j].layer.suit = c[i].suit;
    c[c[i].column.numbers].layer.totalLayer--;
    c[i].column.numbers = COLUMNA+j;
    c[COLUMNA+j].layer.totalLayer++;
    c[i].x = Ax + (j*75);
    c[i].y = Ay;
    c[i].layer.numbers = c[COLUMNA+j].layer.totalLayer;
    c[i].returnOrigin = false;


}

//called in autoComplete: draw the animation of the card that is being auto completed
void animationAutocomplete(Decks c[], ALLEGRO_BITMAP *card, ALLEGRO_BITMAP *background, ALLEGRO_TIMER *timer, ALLEGRO_FONT *font, int i, int j, int largestLayer, int &score, int &movesCounter, int &seconds)
{

    float moveSpeedx, moveSpeedy;

    //calculate the move speed of the card being completed
    moveSpeedx = (c[i].x - (Ax+(j*75)))/10;
    moveSpeedy = (c[i].y - Ay)/10;

    for (int k = 0; k < 10; k++) {

        c[i].x -= moveSpeedx;
        c[i].y -= moveSpeedy;


        al_draw_bitmap(background,0,0,NULL);

        drawMinutes(font,timer,seconds);

        drawSeconds(font, timer, seconds);

        drawMovescount(font,movesCounter);

        drawScore(font, score);

        createCards(c, card,largestLayer);

        al_flip_display();

        al_clear_to_color(al_map_rgb(0,0,0));
    }



}

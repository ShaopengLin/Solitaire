#include "CardStruct.h"

// called in cardsMovement : determine if there are more cards under the card that the user is dragging and back up information for both while changing their state
void changeCardsstate(Decks c[], bool &cardMoving, int largestLayer, int i)
{

    int j = 0;

    //if a card is already moving
    if (cardMoving == true) {
        ;
    } else {

        for (j = 0; j < 52; j++) {

            if (c[j].revealed == true) {

                if (c[j].column.numbers == c[i].column.numbers) {

                    if (c[j].layer.numbers > c[i].layer.numbers) {

                        makeCardfollow(c,j, largestLayer);

                    }
                }
            }
        }

        // back information for the card user is dragging
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

    printf("%d", c[c[0].backupDeck.totalLayer].column.numbers);
    //reveal the cards to the user

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

        c[c[0].backupDeck.totalLayer].revealed = true;
        c[c[0].backupDeck.totalLayer].x -= (float)75;

        printb(background);

        createCards(c,card,largestLayer);
        movesCounter++;

        c[0].backupDeck.totalLayer--;

    } else {

        //return the unmoved cards back to their original position
        resetBackupcardsInfo(c);

        score -= 5;

    }

    al_destroy_bitmap(background);
    al_destroy_bitmap(card);

}

void columnKfixposition(Decks c[], int i, int &movesCounter)
{
    int j = 1;
    for (j = 1; j < 8; j++) {

        if(kHitbox(c,i,j)) {

            if (c[i].number == 13) {

                if (c[j].layer.totalLayer == 0) {

                    printf("K\n");
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

int columnAfixposition(Decks c[], int i, int &movesCounter, int &score)
{
    int j = 0;
    for (j = 0; j < 4; j++) {

        if(aHitbox(c,i,j)) {

            if (c[i].number == c[COLUMNA+j].layer.totalLayer+1) {

                if (c[COLUMNA+j].layer.totalLayer >= 1) {

                    if (c[i].suit == c[COLUMNA+j].layer.suit) {

                        for (int k = 0; k < 52; k++) {

                            if (c[k].follow == true) {

                                return 0;

                            }
                        }

                        stackNonAcardonA(c, i, j);
                        movesCounter++;
                        if(!c[i].scored) {
                            score += 10;
                            c[i].scored = true;
                        }
                    }
                } else {

                    stackAcardonA(c, i, j);

                    movesCounter++;
                    if(!c[i].scored) {
                        score += 10;
                        c[i].scored = true;
                    }

                }
            }
        }
    }
}
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

void followCardreturn(Decks c[], int i, int j)
{
    c[j].x = c[j].originx;
    c[j].y = c[j].originy;
    c[j].layer.numbers = c[j].layer.origin;
    c[j].x = c[i].x;
    c[j].y = c[i].y + (c[j].layer.origin - c[i].layer.origin)*30;
}

void followCardstack(Decks c[], int i, int j)
{
    c[c[j].column.numbers].layer.totalLayer--;
    c[c[i].column.numbers].layer.totalLayer++;
    c[j].column.numbers = c[i].column.numbers;
    c[j].layer.numbers = c[i].layer.numbers + (c[j].layer.origin - c[i].layer.origin);
    c[j].x = c[i].x;
    c[j].y = c[i].y + (c[j].layer.origin - c[i].layer.origin)*30;

}

void makeCardfollow(Decks c[], int j, int largestLayer)
{

    c[j].originx = c[j].x;
    c[j].originy = c[j].y;
    c[j].follow = true;
    c[j].layer.origin = c[j].layer.numbers;
    c[j].layer.numbers += largestLayer;

}

bool allowCardtomove(Decks c[], int i, int largestLayer)
{
    c[i].originx = c[i].x;
    c[i].originy = c[i].y;
    c[i].moving = true;
    c[i].layer.origin = c[i].layer.numbers;
    c[i].layer.numbers += largestLayer;
    return true;
}
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

void animationAutocomplete(Decks c[], ALLEGRO_BITMAP *card, ALLEGRO_BITMAP *background, ALLEGRO_TIMER *timer, ALLEGRO_FONT *font, int i, int j, int largestLayer, int &score, int &movesCounter, int &seconds)
{

    float moveSpeedx, moveSpeedy;


    moveSpeedx = (c[i].x - (Ax+(j*75)))/10;


    moveSpeedy = (c[i].y - Ay)/10;

    for (int k = 0; k < 10; k++) {

        c[i].x -= moveSpeedx;
        c[i].y -= moveSpeedy;


        printb(background);

        drawMinutes(font,timer,seconds);

        drawSeconds(font, timer, seconds);

        drawMovescount(font,movesCounter);

        drawScore(font, score);

        createCards(c, card,largestLayer);

        al_flip_display();

        al_clear_to_color(al_map_rgb(0,0,0));
    }



}

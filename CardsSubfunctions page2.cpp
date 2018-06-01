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

                        c[j].originx = c[j].x;
                        c[j].originy = c[j].y;
                        c[j].follow = true;
                        c[j].layer.origin = c[j].layer.numbers;
                        c[j].layer.numbers += largestLayer;
                    }
                }
            }
        }



        // back information for the card user is dragging
        c[i].originx = c[i].x;
        c[i].originy = c[i].y;
        c[i].moving = true;
        cardMoving = true;
        c[i].layer.origin = c[i].layer.numbers;
        c[i].layer.numbers += largestLayer;

    }


}

//called in cardMovement: controls the distribution and animation of the backup deck
int manageBackupcard(Decks c[], int &largestLayer, int &movesCounter, int &score)
{
    ALLEGRO_BITMAP *background = al_load_bitmap("background.png");
    ALLEGRO_BITMAP *card = al_load_bitmap("cards.png");
    int layerCount = 24, cardsLeft = 1;
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
                for (int i = 28; i < 52; i++) {
                    if(c[i].column.numbers == 0) {
                        c[i].x = backUpx;
                        c[i].y = backUpy;
                        c[0].backupDeck.totalLayer = i;
                        c[i].layer.numbers = layerCount;
                        c[i].revealed = false;
                    }
                    layerCount--;
                }
                layerCount = 24;

                return 0;

            }
        }

        c[c[0].backupDeck.totalLayer].revealed = true;

        //animation

            c[c[0].backupDeck.totalLayer].x -= (float)75;
            printb(background);
            createCards(c,card,largestLayer);
            movesCounter++;



        c[0].backupDeck.totalLayer--;



    } else {

        //return the unmoved cards back to their original position
        for (int i = 28; i < 52; i++) {
            if(c[i].column.numbers == 0) {
                c[i].x = backUpx;
                c[i].y = backUpy;
                c[0].backupDeck.totalLayer = i;
                c[i].layer.numbers = layerCount;
                c[i].revealed = false;
            }
            layerCount--;
        }
        layerCount = 24;
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
    int j = 0, k = 0;
    for (j = 0; j < 4; j++) {
        if(aHitbox(c,i,j)) {
            if (c[i].number == c[columnA+j].layer.totalLayer+1) {

                if (c[columnA+j].layer.totalLayer >= 1) {

                    if (c[i].suit == c[columnA+j].layer.suit) {
                        for (k = 0; k < 52; k++) {
                            if (c[k].follow == true) {
                                return 0;
                            }
                        }
                        printf("A\n");
                        c[c[i].column.numbers].layer.totalLayer--;
                        c[i].column.numbers = columnA+j;
                        c[columnA+j].layer.totalLayer++;
                        c[i].x = Ax + (j*75);
                        c[i].y = Ay;
                        c[i].layer.numbers = c[columnA+j].layer.totalLayer;
                        movesCounter++;
                        score += 10;
                        c[i].returnOrigin = false;
                    }
                } else {
                    printf("A1\n");
                    c[columnA+j].layer.suit = c[i].suit;
                    c[c[i].column.numbers].layer.totalLayer--;
                    c[i].column.numbers = columnA+j;
                    c[columnA+j].layer.totalLayer++;
                    c[i].x = Ax + (j*75);
                    c[i].y = Ay;
                    c[i].layer.numbers = c[columnA+j].layer.totalLayer;
                    movesCounter++;
                    score += 10;
                    c[i].returnOrigin = false;
                }
            }

        }

    }



}

bool kHitbox(Decks c[], int i, int j)
{

    if (c[i].x >= c[j].column.dimensionx && c[i].x <= c[j].column.dimensionx + 70 && c[i].y >= c[j].column.dimensiony && c[i].y <= c[j].column.dimensiony+100) {
        return true;
    } else if (c[i].x + 70 >= c[j].column.dimensionx && c[i].x + 70 <= c[j].column.dimensionx + 70 && c[i].y >= c[j].column.dimensiony && c[i].y <= c[j].column.dimensiony+100) {
        return true;
    } else if (c[i].x >= c[j].column.dimensionx && c[i].x <= c[j].column.dimensionx + 70 && c[i].y + 100 >= c[j].column.dimensiony && c[i].y + 100 <= c[j].column.dimensiony+100) {
        return true;
    } else if (c[i].x + 70 >= c[j].column.dimensionx && c[i].x + 70 <= c[j].column.dimensionx + 70 && c[i].y + 100 >= c[j].column.dimensiony && c[i].y + 100 <= c[j].column.dimensiony+100) {
        return true;
    } else {
        return false;
    }
}

bool aHitbox(Decks c[], int i, int j)
{

    if (c[i].x >= Ax+(j*75) && c[i].x <= Ax+(j*75) + 70 && c[i].y >= Ay && c[i].y <= Ay+100) {
        return true;
    } else if (c[i].x + 70 >= Ax+(j*75) && c[i].x + 70 <= Ax+(j*75) + 70 && c[i].y >= Ay && c[i].y <= Ay+100) {
        return true;
    } else if (c[i].x >= Ax+(j*75) && c[i].x <= Ax+(j*75) + 70 && c[i].y + 100 >= Ay && c[i].y + 100 <= Ay+100) {
        return true;
    } else if (c[i].x + 70 >= Ax+(j*75) && c[i].x + 70 <= Ax+(j*75) + 70 && c[i].y + 100 >= Ay && c[i].y + 100 <= Ay+100) {
        return true;
    } else {
        return false;
    }
}

bool determineWon(Decks c[])
{

    int counter = 0;
    for (int i = 0; i < 4; i++) {
        counter += c[columnA+i].layer.totalLayer;
    }
    if (counter == 52) {
        return true;
    }
    else {
        return false;
    }


}

bool pauseHitbox(ALLEGRO_EVENT events){

     if (events.mouse.x >= PAUSEx && events.mouse.x <= PAUSEx + 126 && events.mouse.y >= PAUSEy && events.mouse.y <= PAUSEy+35) {
        return true;

    } else {
        return false;
    }

}

bool resumeHitbox(ALLEGRO_EVENT events){


    if (events.mouse.x >= RESUMEx && events.mouse.x <= RESUMEx + 200 && events.mouse.y >= RESUMEy && events.mouse.y <= RESUMEy+40) {
        return true;

    } else {
        return false;
    }

}



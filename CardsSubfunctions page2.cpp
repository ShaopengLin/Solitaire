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
int manageBackupcard(Decks c[], int &largestLayer)
{
    ALLEGRO_BITMAP *background = al_load_bitmap("background.jpg");
    ALLEGRO_BITMAP *card = al_load_bitmap("cards.png");
    ALLEGRO_BITMAP *backcard = al_load_bitmap("backcard.png");
    int layerCount = 24, cardsLeft = 1;
    bool skipCard = false;

    //reveal the cards to the user
    if (c[0].backupDeck.totalLayer > 27) {

        // check if back up card is in play area, if so, skip it.
        while (c[c[0].backupDeck.totalLayer].column.numbers != 0)
        {
            if(cardsLeft == 24){
                return 0;
            }
            c[0].backupDeck.totalLayer--;
            cardsLeft++;
        }

        c[c[0].backupDeck.totalLayer].revealed = true;

        //animation
        for (int j = 0; j < 20; j++) {
            c[c[0].backupDeck.totalLayer].x -= (float)75/20;
            printb(background);
            createCards(c,card,backcard,largestLayer);
            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));

        }
        c[0].backupDeck.totalLayer--;



    } else {

        //return the unmoved cards back to their original position
        for (int i = 28; i < 52; i++) {
            if(c[i].column.numbers == 0){
            c[i].x = backUpx;
            c[i].y = backUpy;
            c[0].backupDeck.totalLayer = i;
            c[i].layer.numbers = layerCount;
            c[i].revealed = false;
            }
            layerCount--;
        }
        layerCount = 24;

    }


    al_destroy_bitmap(background);
    al_destroy_bitmap(backcard);
    al_destroy_bitmap(card);

}

void Kcolumnfixposition(Decks c[], int i){
    int j = 1;
        for (j = 1; j < 8; j++){
        if (c[i].number == 13){
            if (c[j].layer.totalLayer == 0){
                c[i].x = c[j].column.dimensionx;
                c[i].y = c[j].column.dimensiony;
                c[i].layer.numbers = 1;
                c[c[i].column.numbers].layer.totalLayer--;
                c[c[j].column.numbers].layer.totalLayer++;
                c[i].column.numbers = c[j].column.numbers;
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


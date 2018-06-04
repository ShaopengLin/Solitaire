//all the functions here are called in another function

#include "CardStruct.h"

//called in cardinfodistribution: make the number that has dealt out 4 times not going be dealt again
void cardNumberremove(Decks c[], int &numbersDealt, int i, int thirteenNumbers[])
{
    int temp;

    //number
    if (c[0].counterNumber[c[i].number] == 4) {

        for (int j = 0; j < 13; j++) {

            //moves the number out of the randomizing range
            if (thirteenNumbers[j] == c[i].number) {

                temp = thirteenNumbers[j];
                thirteenNumbers[j] = thirteenNumbers[12-numbersDealt];
                thirteenNumbers[12 - numbersDealt] = temp;

                if (numbersDealt == 12) {

                } else {
                    numbersDealt++;
                }

                c[0].counterNumber[c[i].number] = 0;
                break;

            }
        }
    }
}

//called in cardinfodistribution: deal suits randomly relative to each card's number
void cardSuitremoveAnddistribute(Decks c[], int fourSuits[], int suitsDealt)
{

    int temp;
    for (int i = 1; i <= 13; i++) {

        for (int j = 0; j < 52; j++) {

            if (c[j].number == i) {

                c[j].suit = fourSuits[rand()%(4 - suitsDealt)];

                for (int k = 0; k < 4; k++) {

                    //moves the suit out of the randomizing range for this number
                    if (fourSuits[k] == c[j].suit) {

                        temp = fourSuits[k];
                        fourSuits[k] = fourSuits[3-suitsDealt];
                        fourSuits[3 - suitsDealt] = temp;

                    }
                }

                suitsDealt++;
            }
        }

        suitsDealt = 0;

    }
}

//called in oneCardfixposition: determine if the card in the hit box can be stacked relative to the game rule
void determinestackable(Decks c[], int i, int j)
{

    if (c[j].revealed) {

        if ((c[j].suit % 2) != (c[i].suit % 2)) {

            if (c[j].number == c[i].number + 1) {

                c[j].stackable = true;
            }
        }
    }
}

//called in cardMovement: determine if a card stacks on another card or return to the original position
void firstCardfixposition(Decks c[], int i, int &movesCounter)
{
    int j = 0;
    for(j = 0; j < 52; j++) {

        if (j != i) {

            if (cardHitbox(c, i, j)) {
                if (c[j].layer.numbers == c[c[j].column.numbers].layer.totalLayer) {

                    //determine if the card in the hit box can be stacked relative to the game rule
                    determinestackable(c, i, j);

                    if (c[j].stackable) {

                        firstCardstack(c,i,j);

                        movesCounter++;

                    } else {
                        c[i].returnOrigin = true;

                    }
                }
            }
        }
    }

    //return to original spot
    firstCardreturn(c, i);

}

//called in cardMovement: based on the first card's information, determine if the cards under it should stack with the first card or return to their original position with the first card
void followCardfixposition(Decks c[], int i)
{

    int j = 0;
    for (j = 0; j < 52; j++) {

        if (c[j].revealed == true) {

            if(c[j].follow == true) {

                // determine if the dragging card has stacked
                if(c[i].column.numbers == c[j].column.numbers) {

                    printf("returned\n");

                    followCardreturn(c, i, j);

                // return to original position
                } else {

                    printf("followed\n");
                    followCardstack(c, i, j);

                }
            }

            if (c[j].follow == true) {

                printf("%d\n", c[j].layer.numbers);

            }

            c[j].follow = false;

        }
    }
}

void resetPlayareacardsInfo(Decks c[])
{

    for (int i = 0; i < 28; i++) {

        c[i].x = 0;
        c[i].y = 920;
        c[i].layer.numbers = 0;
        c[c[i].column.numbers].layer.totalLayer = 0;

    }
}

void resetBackupcardsInfo(Decks c[])
{

    int layerCount = 24;

    for (int i = 28; i < 52; i++) {

        if(c[i].column.numbers == 0) {

            c[i].x = BACKUPx;
            c[i].y = BACKUPy;
            c[i].column.numbers = 0;
            c[i].layer.numbers = layerCount;
            c[0].backupDeck.totalLayer = i;
            c[i].revealed = false;

        }

        layerCount--;

    }
}









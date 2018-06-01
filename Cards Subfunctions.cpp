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
                    ;
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
                        printf("firstCardstack\n");
                        c[i].returnOrigin = false;
                        c[i].x = c[j].x;
                        c[i].y = c[j].y + 30;
                        c[i].layer.numbers = c[j].layer.numbers + 1;
                        c[c[i].column.numbers].layer.totalLayer--;
                        c[c[j].column.numbers].layer.totalLayer++;
                        c[i].column.numbers = c[j].column.numbers;
                        movesCounter++;
                        c[j].stackable = false;
                    } else {
                        c[i].returnOrigin = true;

                    }
                }
            }
        }
    }

    //return to original spot
    if (c[i].returnOrigin == true) {
        printf("firstCardreturn\n");
        c[i].x = c[i].originx;
        c[i].y = c[i].originy;
        c[i].layer.numbers = c[i].layer.origin;
    }
    c[i].returnOrigin = true;
}

//called in cardMovement: based on the first card's information, determine if the cards under it should stack with the first card or return to their original position with the first card
void followCardfixposition(Decks c[], int i)
{

    int j = 0;
    for (j = 0; j < 52; j++) {



    if (c[j].revealed == true){
        if(c[j].follow == true) {

            // determine if the dragging card has stacked
            if(c[i].column.numbers == c[j].column.numbers) {
                printf("returned\n");

                c[j].x = c[j].originx;
                c[j].y = c[j].originy;
                c[j].layer.numbers = c[j].layer.origin;
                c[j].x = c[i].x;
                c[j].y = c[i].y + (c[j].layer.origin - c[i].layer.origin)*30;

            // return to original position
            } else {
                printf("followed\n");
                c[c[j].column.numbers].layer.totalLayer--;
                c[c[i].column.numbers].layer.totalLayer++;
                c[j].column.numbers = c[i].column.numbers;
                c[j].layer.numbers = c[i].layer.numbers + (c[j].layer.origin - c[i].layer.origin);
                c[j].x = c[i].x;
                c[j].y = c[i].y + (c[j].layer.origin - c[i].layer.origin)*30;
            }
        }
        if (c[j].follow == true){
             printf("%d\n", c[j].layer.numbers);
        }

        c[j].follow = false;
        }
    }
}


//called in firstCardfixposition: determine if the card user is dragging is in another card's hit box
bool cardHitbox(Decks c[], int i, int j)
{

    if (c[i].x >= c[j].x && c[i].x <= c[j].x + 70 && c[i].y >= c[j].y && c[i].y <= c[j].y+100) {
        return true;
    } else if (c[i].x + 70 >= c[j].x && c[i].x + 70 <= c[j].x + 70 && c[i].y >= c[j].y && c[i].y <= c[j].y+100) {
        return true;
    } else if (c[i].x >= c[j].x && c[i].x <= c[j].x + 70 && c[i].y + 100 >= c[j].y && c[i].y + 100 <= c[j].y+100) {
        return true;
    } else if (c[i].x + 70 >= c[j].x && c[i].x + 70 <= c[j].x + 70 && c[i].y + 100 >= c[j].y && c[i].y + 100 <= c[j].y+100) {
        return true;
    } else {
        return false;
    }
}






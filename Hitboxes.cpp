#include "CardStruct.h"

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

        counter += c[COLUMNA+i].layer.totalLayer;
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

bool startHitbox(ALLEGRO_EVENT events){

    if (events.mouse.x >= NEWGAMEx && events.mouse.x <= NEWGAMEx + 200 && events.mouse.y >= NEWGAMEy && events.mouse.y <= NEWGAMEy+40){

        return true;

    } else {

        return false;

    }
}

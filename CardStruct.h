#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro.h>
#include <allegro_native_dialog.h>
#include <allegro_image.h>

#define ScreenWidth 540
#define ScreenHeight 920
#define backUpx 458
#define backUpy 90
#define Ax 8
#define Ay 90
#define columnA 8
#define FPS 10
struct Position {

    int numbers = 0;
    int dimensionx;
    int dimensiony;
    int totalLayer = 0;
    int origin = 0;
    int suit;


};

struct Decks {

    int number;
    //1 = spades 2 = hearts, 3 = clubs, 4 = diamonds
    int suit;
    int counterNumber[14] = {0};
    int counterSuit[5] = {0};
    float x = 0;
    float originx = 0;
    float y = 920;
    float originy = 0;
    bool moving = false;
    bool revealed = false;
    bool active = true;
    bool stackable = false;
    bool follow = false;
    bool returnOrigin = true;

    Position column, layer, backupDeck;

};




void initializeAllegro();
void printb(ALLEGRO_BITMAP *background);
void createCards(Decks c[], ALLEGRO_BITMAP *card, int largestLayer);
void cardInfodistribution (Decks c[]);
void cardSuitremoveAnddistribute(Decks c[], int fourSuits[], int suitsDealt);
void cardNumberremove(Decks c[], int &numbersDealt, int i, int thirteenNumbers[]);
void dealCardsIn(Decks c[], ALLEGRO_BITMAP *card, ALLEGRO_BITMAP *background, ALLEGRO_EVENT_QUEUE *event_queue,  bool &done);
void cardMovements(Decks c[],ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_MOUSE_STATE &state, ALLEGRO_EVENT events, bool &cardMoving, int &largestLayer, bool &mouseOnbackup);
void determineLargestlayer(Decks c[], int &largestLayer);
int columnAfixposition(Decks c[], int i);
void columnKfixposition(Decks c[], int i);
void firstCardfixposition(Decks c[], int i);
void followCardfixposition(Decks c[], int i);
void determinestackable(Decks c[], int i, int j);
void animationFlip(Decks c[]);
void changeCardsstate(Decks c[], bool &cardMoving, int largestLayer, int i);
bool cardHitbox(Decks c[], int i, int j);
bool kHitbox(Decks c[], int i, int j);
bool aHitbox(Decks c[], int i, int j);
bool winningFunction(Decks c[], bool &done);
int manageBackupcard(Decks c[], int &largestLayer);






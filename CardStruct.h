#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <allegro.h>
#include <allegro_native_dialog.h>
#include <allegro_font.h>
#include <allegro_ttf.h>

#include <allegro_image.h>

#define ScreenWidth 540
#define ScreenHeight 920
#define BACKUPx 458
#define BACKUPy 90
#define Ax 8
#define Ay 90
#define COLUMNA 8
#define FPS 1
#define PAUSEx 400
#define PAUSEy 20
#define NEWGAMEx 300
#define NEWGAMEy 340
#define RESUMEx 40
#define RESUMEy 340

struct Position {

    int numbers = {0};
    int dimensionx = {0};
    int dimensiony = {0};
    int totalLayer = {0};
    int origin = {0};
    int suit = {0};


};

struct Decks {

    int number = 0;

    //1 = spades 2 = hearts, 3 = clubs, 4 = diamonds
    int suit = {0};
    int counterNumber[14] = {0};
    int counterSuit[5] = {0};
    float x = {0};
    float originx = {0};
    float y = {920};
    float originy = {0};
    bool moving = false;
    bool revealed = false;
    bool active = true;
    bool stackable = false;
    bool follow = false;
    bool returnOrigin = true;
    bool scored = false;

    Position column, layer, backupDeck;

};

void initializeAllegro();
void createCards(Decks c[], ALLEGRO_BITMAP *cardBitmap, int largestLayer);
void cardInfodistribution (Decks c[]);
void cardSuitremoveAnddistribute(Decks c[], int fourSuits[], int suitsDealt);
void cardNumberremove(Decks c[], int &numbersDealt, int i, int thirteenNumbers[]);
void dealCardsIn(Decks c[], ALLEGRO_BITMAP *cardBitmap, ALLEGRO_BITMAP *background, ALLEGRO_EVENT_QUEUE *event_queue);
void cardMovements(Decks c[],ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_MOUSE_STATE &state, ALLEGRO_EVENT events, bool &cardMoving, int &largestLayer, bool &mouseOnbackup, int &movesCounter, int &score);
void determineLargestlayer(Decks c[], int &largestLayer);
int columnAfixposition(Decks c[], int i, int &movesCounter, int &score);
void columnKfixposition(Decks c[], int i, int &movesCounter);
void firstCardfixposition(Decks c[], int i, int &movesCounter);
void followCardfixposition(Decks c[], int i);
void determinestackable(Decks c[], int i, int j);
void revealCard(Decks c[]);
void changeCardsstate(Decks c[], bool &cardMoving, int largestLayer, int i);
bool cardHitbox(Decks c[], int i, int j);
bool kHitbox(Decks c[], int i, int j);
bool aHitbox(Decks c[], int i, int j);
bool determineWon(Decks c[]);
int manageBackupcard(Decks c[], int &largestLayer, int &movesCounter,int &score);
void drawMinutes(ALLEGRO_FONT *font, ALLEGRO_TIMER *timer, int seconds);
void drawSeconds(ALLEGRO_FONT *font,ALLEGRO_TIMER *timer, int seconds);
void drawMovescount(ALLEGRO_FONT *font, int movesCounter);
void drawScore(ALLEGRO_FONT *font, int &score);
void writeMoves(FILE *writeInscore, int movesCounter);
void writeScore(FILE *writeInscore, int score);
void writeSeconds(FILE *writeInscore, ALLEGRO_TIMER *timer, int &seconds);
void writeQuickest(FILE *writeInscore, int quickest);
void writeLeastmove(FILE *writeInscore, int leastMove);
void writeHighscore(FILE *writeInscore, int highScore);
void saveTofile(Decks c[],FILE *writeInsave, FILE *writeInscore, ALLEGRO_TIMER *timer, int seconds, int quickest, int score, int highScore, int movesCounter, int  leastMove);
bool pauseHitbox(ALLEGRO_EVENT events);
void drawPausescreen(Decks c[],ALLEGRO_BITMAP *pauseButton, ALLEGRO_BITMAP *pasueScreen);
bool resumeHitbox(ALLEGRO_EVENT events);
bool restartHitbox(ALLEGRO_EVENT events);
void resumeGame(Decks c[], ALLEGRO_BITMAP *pauseButton, ALLEGRO_BITMAP *pauseScreen, ALLEGRO_TIMER *timer,ALLEGRO_TIMER * pauseTimer, ALLEGRO_BITMAP *resume);
void drawNewgamePressed(Decks c[], ALLEGRO_BITMAP *pauseButton, ALLEGRO_BITMAP *pauseScreen, ALLEGRO_BITMAP *newGame);
int readFromfile(Decks c[],ALLEGRO_DISPLAY *display,FILE *readInsave, FILE *readInscore, ALLEGRO_TIMER *timer, int &seconds, int &score, int &movesCounter, char saved[]);
void resetPlayareacardsInfo(Decks c[]);
void resetBackupcardsInfo(Decks c[]);
void rearrageFiles(FILE *writeInsave, FILE *writeInscore, FILE *readInsave, FILE *readInscore);
int fixScore(int score);
int fixSeconds(ALLEGRO_TIMER *timer, int seconds);
void firstCardstack(Decks c[], int i, int j);
void firstCardreturn(Decks c[], int i);
void followCardstack(Decks c[], int i, int j);
void followCardreturn(Decks c[],int i, int j);
void makeCardfollow(Decks c[], int j, int largestLayer);
bool allowCardtomove(Decks c[], int i, int largestLayer);
void stackAcardonA(Decks c[], int i, int j);
void stackNonAcardonA(Decks c[], int i, int j);
int autoComplete(Decks c[], ALLEGRO_BITMAP *cardBitmap, ALLEGRO_BITMAP *background, ALLEGRO_TIMER *timer, ALLEGRO_FONT *font, int &largestLayer, int &score, int &movesCounter, int &seconds,ALLEGRO_DISPLAY *display);
void animationAutocomplete(Decks c[], ALLEGRO_BITMAP *cardBitmap, ALLEGRO_BITMAP *background, ALLEGRO_TIMER *timer, ALLEGRO_FONT *font, int i, int j, int largestLayer, int &score, int &movesCounter, int &seconds);
void winningScreen(Decks c[], ALLEGRO_BITMAP *winScreen);
void determineBeathighscores(ALLEGRO_BITMAP *medal, ALLEGRO_TIMER *timer, int score, int &highScore, int seconds, int &quickest, int movesCounter, int &leastMove);
int calculateUnltimateScore(int score, int movesCounter, int seconds, ALLEGRO_TIMER *timer);
void startNewgame(Decks c[], int &movesCounter, int &seconds, int &score, ALLEGRO_TIMER *timer, ALLEGRO_TIMER *pauseTimer,  ALLEGRO_BITMAP *cardBitmap, ALLEGRO_BITMAP *background, ALLEGRO_EVENT_QUEUE *event_queue);
int calculateExtraspace(int seconds, ALLEGRO_TIMER *timer);





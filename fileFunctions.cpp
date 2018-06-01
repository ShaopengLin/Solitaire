#include "CardStruct.h"

void writeSeconds(FILE *writeInscore, ALLEGRO_TIMER *timer, int &seconds){
    seconds += al_get_timer_count(timer);
    fprintf(writeInscore, "%d\n", seconds);

}


void writeScore(FILE *writeInscore, int score){

    fprintf(writeInscore, "%d\n", score);

}

void writeMoves(FILE *writeInscore, int movesCounter){

    fprintf(writeInscore, "%d\n", movesCounter);

}
void writeQuickest(FILE *writeInscore, int quickest){

    fprintf(writeInscore, "%d\n", quickest);
}

void writeLeastmove(FILE *writeInscore, int leastMove){
    fprintf(writeInscore, "%d\n", leastMove);
}

void writeHighscore(FILE *writeInscore, int highScore){
    fprintf(writeInscore, "%d\n", highScore);
}


void saveTofile(Decks c[],FILE *writeInsave, FILE *writeInscore, ALLEGRO_TIMER *timer, int seconds, int quickest, int score, int highScore, int movesCounter, int  leastMove){
 if (determineWon(c)){
    if (seconds < quickest || quickest == 0){
            writeSeconds(writeInscore, timer,seconds);
        }
        else{
             writeQuickest(writeInscore, quickest);
        }
        if (score > highScore){
        writeScore(writeInscore, score);
        }
        else{
            writeHighscore(writeInscore, highScore);
        }
        if (movesCounter < leastMove || leastMove == 0){
            writeMoves(writeInscore, movesCounter);
        }
        else{
            writeLeastmove(writeInscore, leastMove);
        }

    }
    else {
    writeQuickest(writeInscore, quickest);
    writeHighscore(writeInscore, highScore);
    writeLeastmove(writeInscore, leastMove);
    if (movesCounter != 0){
    fprintf(writeInscore, "%s\n", "saved");
    }
    writeSeconds(writeInscore, timer, seconds);
    writeScore(writeInscore, score);
    writeMoves(writeInscore, movesCounter);

    for (int i = 0; i < 52; i++){
    fwrite(&c[i], sizeof(c[i]),1,writeInsave);


    }
    }


}

#include "CardStruct.h"

//called in saveTofile: write the time used into the scorefile
void writeSeconds(FILE *writeInscore, ALLEGRO_TIMER *timer, int &seconds)
{
    seconds += al_get_timer_count(timer);
    fprintf(writeInscore, "%d\n", seconds);

}

//called in saveTofile: write the score into scorefile
void writeScore(FILE *writeInscore, int score)
{

    fprintf(writeInscore, "%d\n", score);

}

//called in saveTofile: write the moves into scorefile
void writeMoves(FILE *writeInscore, int movesCounter)
{

    fprintf(writeInscore, "%d\n", movesCounter);

}

//called in saveTofile: write the quickest time ever used into scorefile
void writeQuickest(FILE *writeInscore, int quickest)
{

    fprintf(writeInscore, "%d\n", quickest);
}

//called in saveTofile: write the least moves ever used into scorefile
void writeLeastmove(FILE *writeInscore, int leastMove)
{
    fprintf(writeInscore, "%d\n", leastMove);
}

//called in saveTofile: write the highest score ever used into scorefile
void writeHighscore(FILE *writeInscore, int highScore)
{
    fprintf(writeInscore, "%d\n", highScore);
}

//called in main: writes high scores and necessery save data to into two files
void saveTofile(Decks c[],FILE *writeInsave, FILE *writeInscore, ALLEGRO_TIMER *timer, int seconds, int quickest, int score, int highScore, int movesCounter, int  leastMove)
{
    if (determineWon(c)) {

        if (seconds < quickest || quickest == 0) {

            writeSeconds(writeInscore, timer,seconds);

        } else {

            writeQuickest(writeInscore, quickest);

        }
        if (score > highScore) {

            writeScore(writeInscore, score);

        } else {

            writeHighscore(writeInscore, highScore);

        }
        if (movesCounter < leastMove || leastMove == 0) {

            writeMoves(writeInscore, movesCounter);

        } else {

            writeLeastmove(writeInscore, leastMove);

        }

    } else {

        writeQuickest(writeInscore, quickest);
        writeHighscore(writeInscore, highScore);
        writeLeastmove(writeInscore, leastMove);

        if (movesCounter != 0) {

            fprintf(writeInscore, "%s\n", "saved");

        }

        writeSeconds(writeInscore, timer, seconds);
        writeScore(writeInscore, score);
        writeMoves(writeInscore, movesCounter);

        //writes the structure in to save file
        for (int i = 0; i < 52; i++) {

            fwrite(&c[i], sizeof(c[i]),1,writeInsave);

        }
    }
}

//called in main: read saved data from files
int readFromfile(Decks c[],ALLEGRO_DISPLAY *display, FILE *readInsave, FILE *readInscore, ALLEGRO_TIMER *timer, int &seconds, int &score, int &movesCounter, char saved[])
{
    //if there is saved data
    if (strcmp(saved, "saved") == 0) {

        // if the user want to continue, read in the saved information
        if (al_show_native_message_box(display,"Message", "Unfinished Game","Do you want to continue from your last game?", NULL,ALLEGRO_MESSAGEBOX_YES_NO) == 1) {

            fscanf(readInscore, "%d", &seconds);
            fscanf(readInscore, "%d", &score);
            fscanf(readInscore, "%d", &movesCounter);

            for (int i = 0; i < 52; i++) {

                fread(&c[i], sizeof(c[i]), 1, readInsave);

            }

            return 0;

        } else {

            return 1;

        }
    }
}

//called in main: rename and remove files
void rearrageFiles(FILE *writeInsave, FILE *writeInscore, FILE *readInsave, FILE *readInscore)
{

    //close the files
    fclose(writeInsave);
    fclose(readInsave);
    fclose(readInscore);
    fclose(writeInscore);

    //rename and remove the files
    remove("savefile.txt");
    rename("replacesave.txt", "savefile.txt");
    remove("scorefile.txt");
    rename("replacescore.txt", "scorefile.txt");

}


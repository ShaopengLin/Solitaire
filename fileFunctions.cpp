#include "CardStruct.h"

void writeSeconds(FILE *writeInscore, ALLEGRO_TIMER *timer, int &seconds)
{
    seconds += al_get_timer_count(timer);
    fprintf(writeInscore, "%d\n", seconds);

}

void writeScore(FILE *writeInscore, int score)
{

    fprintf(writeInscore, "%d\n", score);

}

void writeMoves(FILE *writeInscore, int movesCounter)
{

    fprintf(writeInscore, "%d\n", movesCounter);

}

void writeQuickest(FILE *writeInscore, int quickest)
{

    fprintf(writeInscore, "%d\n", quickest);
}

void writeLeastmove(FILE *writeInscore, int leastMove)
{
    fprintf(writeInscore, "%d\n", leastMove);
}

void writeHighscore(FILE *writeInscore, int highScore)
{
    fprintf(writeInscore, "%d\n", highScore);
}

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

        for (int i = 0; i < 52; i++) {

            fwrite(&c[i], sizeof(c[i]),1,writeInsave);

        }
    }
}

void readFromfile(Decks c[],ALLEGRO_DISPLAY *display, FILE *readInsave, FILE *readInscore, ALLEGRO_TIMER *timer, int &seconds, int &score, int &movesCounter, char saved[])
{

    if (strcmp(saved, "saved") == 0) {

        if (al_show_native_message_box(display,"Message", "Unfinished Game","Do you want to continue from your last game?", NULL,ALLEGRO_MESSAGEBOX_YES_NO) == 1) {

            fscanf(readInscore, "%d", &seconds);
            fscanf(readInscore, "%d", &score);
            fscanf(readInscore, "%d", &movesCounter);

            for (int i = 0; i < 52; i++) {
                fread(&c[i], sizeof(c[i]), 1, readInsave);

            }
        }
    }
}

void rearrageFiles(FILE *writeInsave, FILE *writeInscore, FILE *readInsave, FILE *readInscore)
{

    fclose(writeInsave);
    fclose(readInsave);
    fclose(readInscore);
    fclose(writeInscore);
    remove("savefile.txt");
    rename("replacesave.txt", "savefile.txt");
    remove("scorefile.txt");
    rename("replacescore.txt", "scorefile.txt");

}


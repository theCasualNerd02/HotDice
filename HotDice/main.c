/*
 * main.c
 * HotDice
 *
 * Created by Matthew Rand on 2019-07-17.
 * Copyright (c) 2019 Matthew Rand. All rights reserved.
 *
 */




#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <apple2enh.h>


#define NUM_DICE 6
#define MIN_DIE_VALUE 1
#define MAX_DIE_VALUE 6
#define NUM_PLAYERS 4


typedef struct tGame {
    int points[4];
    int riskedPoints;
    int turn;
    int numActiveDice;
    int activeDice[NUM_DICE];
    int inactiveDice[NUM_DICE];
    int diceCount[MAX_DIE_VALUE];
    bool diceSelected[NUM_DICE];
} tGame;

tGame game;


int mulitplePointValue[4][6] = {
    { 1000, 200, 300, 400, 500, 600},
    { 1000, 1000, 1000, 1000, 1000, 1000},
    { 2000, 2000, 2000, 2000, 2000, 2000},
    { 3000, 3000, 3000, 3000, 3000, 3000}
};


void initGame(void)    {
    int i;
    game.numActiveDice = NUM_DICE;
    for (i = 0; i < NUM_DICE; i++)  {
        game.diceSelected[i] = false;
        game.diceCount[i] = 0;
    }
    game.riskedPoints = 0;
    game.turn = 0;
    
}


void printInstructions(void)    {
    unsigned int seed = 0;
    // Intro and Instructions
    videomode(VIDEOMODE_80x24);
    printf("Hot Dice!!\n");
    printf("Written by: Matthew Rand\n");
    printf("------------------------\n\n");
    printf("Instructions\n");
    printf("1. Each player takes turns rolling the dice. When it's your turn, you roll all six dice\n");
    printf("2. Points are earned every time you roll a 1,5, tiriplets or a strait (1 to 6)\n");
    printf("3. If none of your dice are worth points your turn ends\n");
    printf("4. The game ends the round after someone reaches 10 000 points\n");
    printf("Scoring\n");
    printf("1 = 100 pts\n");
    printf("5 = 50 pts\n");
    printf("1,1,1 = 1000 pts\n");
    printf("2,2,2 = 200 pts\n");
    printf("3,3,3 = 300 pts\n");
    printf("4,4,4 = 400 pts\n");
    printf("5,5,5 = 500 pts\n");
    printf("6,6,6 = 600 pts\n\n");
    printf("r means reroll, e means end turn\n");
    printf("Press any key to continue: \n");
    while (! kbhit())   {
        seed++;
    }
    cgetc();
    srand(seed);
    
}


void rollADice(int i)    {
    int lowRoll =  1;
    int highRoll = 6;
    game.activeDice[i] = rand()%(highRoll - lowRoll + 1);
    game.activeDice[i]+= lowRoll;
}


void sortDice(void) {
    int i, x, swap;
    for (x = 0 ; x < game.numActiveDice - 1; x++)
    {
        for (i = 0 ; i < game.numActiveDice - x - 1; i++)
        {
            if (game.activeDice[i] > game.activeDice[i+1])  { /* For decreasing order use < */
                swap       = game.activeDice[i];
                game.activeDice[i]   = game.activeDice[i+1];
                game.activeDice[i+1] = swap;
            }
        }
    }
}


void countDice(void)    {
    int i;
    for (i = 0; i < MAX_DIE_VALUE; i++) {
        game.diceCount[i] = 0;
    }
    for (i = 0; i < game.numActiveDice; i++)  {
        game.diceCount[game.activeDice[i] - 1]++;
    }
}


void printRolls(void)   {
    int i;
    for (i = 0; i < game.numActiveDice; i++) {
        printf("%i",game.activeDice[i]);
        if (i != game.numActiveDice - 1) {
            printf(", ");
        }
    }
    for (i= 0; i < NUM_DICE - game.numActiveDice; i++)
    {
        printf(", ");
        revers(true);
        printf("%i", game.inactiveDice[i]);
        revers(false);
    }
       printf("\n1, 2, 3, 4, 5, 6 select the values you wish to keep\n");
}
bool anyDiceSelected(void)  {
    int i;
    for (i = 0; i < game.numActiveDice; i++) {
        if (game.diceSelected[i]) return true;
    }
    return false;
}

void printAll(void) {
    int i;
    printRolls();
    for (i = 0; i < 4; i++) {
        printf("player%i-%i\n",i,game.points[i]);
    }
    for (i = 0; i < game.numActiveDice; i++)  {
        if (game.diceSelected[i])   {
            printf("%i-true ",i);
        }
        else    {
            printf("%i-false ",i);
        }
    }
    printf("\n");
}
void printTable(void)   {
    int i;
    for (i = 0; i < NUM_PLAYERS; i++)   {
        if (i == game.turn) {
            revers(true);
        }
        printf("Player[%i]-%i\n",i+1,game.points[i]);
        revers(false);
    }
    printf("Risked points-%i\n",game.riskedPoints);
}

void printScreen(void)  {
    clrscr();
    printRolls();
    printTable();
}

void multipleDiceSelecting(int i) {
    int x;
    for (x = 0; x < game.numActiveDice; x++)    {
        if (game.activeDice[i] == game.activeDice[x])   {
            game.diceSelected[x] = true;
        }
    }
}

bool possibleDiceSelections(void)   {
    int i;
    for (i = 0; i < game.numActiveDice; i++)   {
        if (game.activeDice[i] == 1 || game.activeDice[i] == 5)  return true;
    }
    for (i = 0; i < 6; i++) {
        if (game.diceCount[i] >= 3) return true;
    }
    return false;
}

void diceRemoveSwap(int i)   {  // i is the position of the roll being removed
    int inactiveDiceCount = NUM_DICE - game.numActiveDice;
    game.inactiveDice[inactiveDiceCount] = game.activeDice[i];
    game.activeDice[i] = game.activeDice[game.numActiveDice - 1];
    game.numActiveDice--;
}

void findValueSelected(char input)    {
    int i, diceValue;
    if (input > 48 && input < 55)   {   //b/w 1-6
        int diceSelected = input - 49;
        if (diceSelected > game.numActiveDice)  return;
        diceValue = game.activeDice[diceSelected];
        if (game.diceCount[diceValue - 1] >= 3) {
            multipleDiceSelecting(diceSelected);
            game.riskedPoints += mulitplePointValue[game.diceCount[diceValue - 1] - 3] [diceValue - 1];
        }
        else if (diceValue == 1)    {
            game.riskedPoints += 100;
            game.diceSelected[diceSelected] = true;
        }
        else if (diceValue == 5)    {
            game.riskedPoints += 50;
            game.diceSelected[diceSelected] = true;
        }
        else return;
        
        
        for (i = game.numActiveDice - 1; i >= 0; i--)  {
            if (game.diceSelected[i])   {
                diceRemoveSwap(i);
                game.diceSelected[i] = false;
            }
        }
    }
}


void takeTurn(void) {
    int i;
    char input;
    for (i = 0; i < game.numActiveDice; i++)  {
        rollADice(i);
        game.diceSelected[i] = false;
    }
    sortDice();
    countDice();
    printScreen();
    if (!possibleDiceSelections())  {
        printf("\nSorry there or no valid dice turn over\n");
        cgetc();
        game.riskedPoints = 0;
        return;
    }
    do {
        input = cgetc();
        findValueSelected(input);
        sortDice();
        printScreen();
        if (input == 'r')    {
            for (i = 0; i < game.numActiveDice; i++)  {
                rollADice(i);
                game.diceSelected[i] = false;
            }
            sortDice();
            countDice();
            printScreen();
            if (!possibleDiceSelections())  {
                printf("\nSorry there or no valid dice turn over\n");
                cgetc();
                game.riskedPoints = 0;
                return;
            }
        }
    } while(input != 'e');
}

int main(void)  {
    // Variable declaration
    char input;
    printInstructions();
    initGame();
    // Game Starts
    do  {
        takeTurn();
        game.points[game.turn]+= game.riskedPoints; // Need to either make risked points 0 if failed or add it in
        if (game.points[game.turn] >= 10000)    {
            break;
        }
        game.turn++;
        if (game.turn > 3) game.turn = 0;
        printScreen();
        if (game.riskedPoints > 0)  {
            printf("player-%i Do you want to continue with %i with %i risked points? y/n?", game.turn + 1, game.numActiveDice, game.riskedPoints);
            input = cgetc();
            if (input != 'y')   {
               game.numActiveDice = NUM_DICE;
                game.riskedPoints = 0;
            }
        }
        
    } while(true);
    clrscr();
    printf("player-%i won with %i points", game.turn + 1, game.points[game.turn]);
    return 0;
    }

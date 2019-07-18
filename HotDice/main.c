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
    int removedDice;
    int dice[NUM_DICE];
    int diceCount[MAX_DIE_VALUE];
    bool diceSelected[NUM_DICE];
} tGame;

tGame game;


void initGame(void)    {
    int i;
    game.removedDice = 0;
    for (i = 0; i < NUM_DICE; i++)  {
        game.diceSelected[i] = false;
        game.diceCount[MAX_DIE_VALUE];
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
    printf("6,6,6 = 600 pts\n");
    printf("1,2,3,4,5,6 = 1500 pts\n\n");
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
    game.dice[i] = rand()%(highRoll - lowRoll + 1);
    game.dice[i]+= lowRoll;
}


void sortDice(void) {
    int i, x, swap;
    for (x = 0 ; x < 6 - 1; x++)
    {
        for (i = 0 ; i < NUM_DICE - x - 1; i++)
        {
            if (game.dice[i] > game.dice[i+1])  { /* For decreasing order use < */
                swap       = game.dice[i];
                game.dice[i]   = game.dice[i+1];
                game.dice[i+1] = swap;
            }
        }
    }
}


void countDice(void)    {
    int i;
    for (i = 0; i < MAX_DIE_VALUE; i++) {
        game.diceCount[i] = 0;
    }
    for (i = 0; i < NUM_DICE; i++)  {
        game.diceCount[game.dice[i]]++;
    }
}


void printRolls(void)   {
    int i;
    for (i = 0; i < NUM_DICE - game.removedDice; i++) {
        printf("%i",game.dice[i]);
        countDice();
        if (i != NUM_DICE - game.removedDice - 1) {
            printf(", ");
        }
        else printf("\n");
    }
       printf("1, 2, 3, 4, 5, 6 select the values you wish to keep\n");
}
bool anyDiceSelected(void)  {
    int i;
    for (i = 0; i < 5; i++) {
        if (game.diceSelected[i]) return true;
    }
    return false;
}


void findValueSelected(void)    {
    int i;
    for (i = 0; i < MAX_DIE_VALUE; i++) {
        if (game.diceSelected[i])   {
            if (game.dice[i] == 1 ) {
                game.points[game.turn] += 100;
                game.removedDice++;
                printf("%i\n",game.removedDice);
            }
            else if ( game.dice[i] == 5)    {
                game.points[game.turn] += 50;
                game.removedDice++;
                printf("%removed-i\n",game.removedDice);
            }
            else {
                printf("Sorry but the chosen roll is invalid\n");
                game.diceSelected[i] = false;
            }
        }
    }
}


void roll(void) {
    int i;
    char input;
    for (i = 0; i < NUM_DICE - game.removedDice; i++)  {
        rollADice(i);
    }
    sortDice();
    countDice();
    printRolls();
    do {
        input = cgetc();
        for (i = 0; i < NUM_DICE; i++)  {
            if (input == i+'1')   {
                game.diceSelected[i] = true;
            }
        }
        findValueSelected();
        if (input == 'r' && anyDiceSelected())    {
            findValueSelected();
            for (i = 0; i < NUM_DICE - game.removedDice; i++)  {
                rollADice(i);
            }
            for (i = 0; i < NUM_DICE; i++)  {
                game.diceSelected[i] = false;
            }
            sortDice();
            countDice();
            printRolls();
        }
    } while(input != 'e');
    game.turn++;
    if (game.turn > 3) game.turn = 0;
}

void printAll(void) {
    int i;
    printInstructions();
    printRolls();
    printf("Removed=%d\n", game.removedDice);   // To be removed
    for (i = 0; i < 4; i++) {
        printf("%i\n",game.points[i]);
    }
}
int main(void)  {
    // Variable declaration
    int highestScore = 1;   // Is the player with the highest score variable
    int i;
    printInstructions();
    initGame();
    // Game Starts
    do  {
        roll();
        printAll();
        game.removedDice = 0;
    } while(highestScore < 10000);
    return 0;
    }

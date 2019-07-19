/*
 * main.c
 * HotDice
 *
 * Created by Matthew Rand on 2019-07-17.
 * Copyright (c) 2019 Matthew Rand. All rights reserved.
 *
 */



#include <ctype.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <apple2enh.h>


#define NUM_DICE 6
#define MIN_DIE_VALUE 1
#define MAX_DIE_VALUE 6
#define MAX_PLAYERS 8


typedef struct tGame {
    int points[MAX_PLAYERS];
    int riskedPoints;
    int turn;
    int numActiveDice;
    int activeDice[NUM_DICE];
    int inactiveDice[NUM_DICE];
    int diceCount[MAX_DIE_VALUE];
    int numPlayers;
    bool diceSelected[NUM_DICE];
    bool taken1;
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
    char inputPlayer;
    for (i = 0; i < MAX_PLAYERS; i++)   {
        game.points[i] = 0;
    }
    game.numActiveDice = NUM_DICE;
    for (i = 0; i < NUM_DICE; i++)  {
        game.diceSelected[i] = false;
        game.diceCount[i] = 0;
    }
    game.riskedPoints = 0;
    game.turn = 0;
    game.taken1 = false;
    do  {
        printf("How many people are playing? (between 2 and 8)\n");
        inputPlayer = cgetc();
        if (inputPlayer < '2' || inputPlayer > '0' + MAX_PLAYERS) {
            printf("Sorry the inputed number is invalid\n");
        }
        else    {
            game.numPlayers = inputPlayer - '0';
            break;
        }
    } while (inputPlayer < '2' || inputPlayer > '0' + MAX_PLAYERS);
}


void printInstructions(void)    {
    unsigned int seed = 0;
    // Intro and Instructions
    videomode(VIDEOMODE_80x24);
    printf("Hot Dice!!\n");
    printf("Written by: Matthew Rand\n");
    printf("From Rand-dom Software\n");
    printf("------------------------\n\n");
    printf("Instructions\n");
    printf("1. Each player takes turns rolling the dice. When it's your turn, you roll all six dice\n");
    printf("2. Points are earned every time you roll a 1,5, or 3 or more of a kind\n");
    printf("3. If none of your dice are worth points your turn ends\n");
    printf("4. The game ends after someone reaches 10 000 points\n\n");
    printf("Scoring\n");
    printf("1 = 100 pts\n");
    printf("5 = 50 pts\n");
    printf("1,1,1 = 1000 pts\n");
    printf("3 of a kind = value x 100 pts\n");
    printf("4 of a kind = 1000 pts\n");
    printf("5 of a kind = 2000 pts\n");
    printf("6 of a kind = 3000 pts\n\n");
    printf("Type r to reroll, type e to end turn\n");
    printf("Press any key to continue: \n\n");
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
    printf("Dice - ");
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
    printf("\nType - ");
    for (i = 1; i <= game.numActiveDice; i++)   {
       printf("%i", i);
        if (i != game.numActiveDice)    {
            printf(", ");
        }
        
    }
    printf(" to select dice\n");
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
        printf("Player%i-%i\n",i,game.points[i]);
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
    for (i = 0; i < game.numPlayers; i++)   {
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
    game.taken1 = true;
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
        if (diceSelected >= game.numActiveDice)  return;
        diceValue = game.activeDice[diceSelected];
        if (game.diceCount[diceValue - 1] >= 3) {
            multipleDiceSelecting(diceSelected);
            game.riskedPoints += mulitplePointValue[game.diceCount[diceValue - 1] - 3] [diceValue - 1];
        }
        else if (diceValue == 1)    {
            game.riskedPoints += 100;
            game.diceSelected[diceSelected] = true;
            game.taken1 = true;
        }
        else if (diceValue == 5)    {
            game.riskedPoints += 50;
            game.diceSelected[diceSelected] = true;
            game.taken1 = true;
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
    game.taken1 = false;
    sortDice();
    countDice();
    printScreen();
    if (!possibleDiceSelections())  {
        printf("\nSorry there are no points to select. Turn over.\n");
        cgetc();
        game.numActiveDice = NUM_DICE;
        game.riskedPoints = 0;
        return;
    }
    do {
        input = tolower(cgetc());
        findValueSelected(input);
        sortDice();
        printScreen();
        if (game.numActiveDice == 0)    {
            printf("Player-%i got every die, roll another six!!\n", game.turn + 1);
            cgetc();
            game.numActiveDice = NUM_DICE;
            input = 'r';
        }
        if ((input == 'r') && (game.taken1 == true))    {
            for (i = 0; i < game.numActiveDice; i++)  {
                rollADice(i);
                game.diceSelected[i] = false;
            }
            game.taken1 = false;
            sortDice();
            countDice();
            printScreen();
            if (!possibleDiceSelections())  {
                printf("\nSorry there are no points to select. Turn over.\n");
                cgetc();
                game.numActiveDice = NUM_DICE;
                game.riskedPoints = 0;
                return;
            }
        }
    } while(input != 'e');
}
void playGame(void) {
    do  {
        char input;
        takeTurn();
        game.points[game.turn]+= game.riskedPoints; // Need to either make risked points 0 if failed or add it in
        if (game.points[game.turn] >= 10000)    {
            break;
        }
        game.turn++;
        if (game.turn >= game.numPlayers) game.turn = 0;
        printScreen();
        if (game.riskedPoints > 0)  {
            printf("Player-%i, do you want to continue with %i die and %i risked points? y/n?", game.turn + 1, game.numActiveDice, game.riskedPoints);
            input = tolower(cgetc());
            if (input != 'y')   {
                game.numActiveDice = NUM_DICE;
                game.riskedPoints = 0;
            }
        }
        
    } while(true);
    clrscr();
    printf("Player-%i won with %i points\n", game.turn + 1, game.points[game.turn]);
}

int main(void)  {
    // Variable declaration
    char input;
    printInstructions();
    do  {
        initGame();
        // Game Starts
        playGame();
        printf("Would you like to play again? y/n?");
        input = tolower(cgetc());
    } while (input == 'y');
    
    return 0;
    }

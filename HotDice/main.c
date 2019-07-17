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


typedef struct tGame {
    int points[4];
    int riskedPoints;
    int turn;
} tGame;


void printInstructions(void)
{
    unsigned int seed = 0;
    // Intro and Instructions
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
    printf("Press any key to continue: ");
    while (! kbhit())   {
        seed++;
    }
    cgetc();
    srand(seed);
    
}


int main(void)
{
    // Variable declaration
    tGame game;
    int highestScore = 1;   // Is the player with the highest score variable
    int lowRoll =  1;
    int highRoll = 6;
    int i;
    int dice;
    char input;
    
    printInstructions();
    // Game Starts
    do  {
        do  {
            for (i = 1; i < 7; i++)  {
                dice = rand()%(highRoll - lowRoll + 1);
                printf("%i",dice);
                if (i != 6) printf(", ");
            }
            do {
                input = cgetc();
                if (input = 'r')    {
                    // Reroll
                }
            } while(input != 'e');
        } while(game.riskedPoints != 0 || input != 'e');
    } while(highestScore < 10000);
    return 0;
}


//
//  main.h
//  2022-pizza-test
//
//  Created by Bullcaos on 2/2/22.
//

#ifndef main_h
#define main_h

#define MAX_INGREDIENTS (6)

struct Ingredients
{
    int cheese;
    unsigned char noCheese;
    int basil;
    unsigned char noBasil;
    int mushroom;
    unsigned char noMushroom;
    int pepper;
    unsigned char noPepper;
    int tomatoe;
    unsigned char noTomatoe;
    int pineapple;
    unsigned char noPineapple;
};

void determineIngredient(const char* ingredient, unsigned char likes);
int processLine(char* line, unsigned char likes);
int readFile(const char* fileName);
void worstCaseDecision(char** decision);
void decideIngredients(char* decision[MAX_INGREDIENTS], int* numElement);
int outputFile(const char* destination, int numElements, char* elements[MAX_INGREDIENTS]);

#endif /* main_h */

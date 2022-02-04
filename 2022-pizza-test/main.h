//
//  main.h
//  2022-pizza-test
//
//  Created by Bullcaos on 2/2/22.
//

#ifndef main_h
#define main_h

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

void determineIngredient(const char* ingredient, const unsigned char likes);
int processLine(char* line, const size_t lineSize, const unsigned char likes);
int readFile(const char* fileName);

#endif /* main_h */

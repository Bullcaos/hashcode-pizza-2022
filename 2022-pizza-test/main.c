//
//  main.c
//  2022-pizza-test
//
//  Created by Bullcaos on 2/2/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

#define MAX_INGREDIENTS (6)

const int BUFFER_SIZE = 2048;
struct Ingredients ing = {};


 /**
  * Checks the obtained ingredient's name in order to determine which ingredient is refering to and
  * also if the potential customer likes it or not
  *
  * @param ingredient String representing the name of the ingredient
  * @param likes Flag to set whether the client likes it or not
  */
void determineIngredient(const char* ingredient, const unsigned char likes)
{
    // In order to save performance, only the critical letters of the word
    //  will be checked (first and second)
    //  i.e.: C-heese | T-omatoe | B-asil
    switch (ingredient[0]) {
        case 'c':
            if (likes) ing.cheese++;
            else
            {
                ing.cheese--;
                ing.noCheese = 0x01;
            }
            break;
        case 'b':
            if (likes) ing.basil++;
            else
            {
                ing.basil--;
                ing.noBasil = 0x01;
            }
            break;
        case 'm':
            if (likes) ing.mushroom++;
            else
            {
                ing.mushroom--;
                ing.noMushroom = 0x01;
            }
            break;
        case 't':
            if (likes) ing.tomatoe++;
            else
            {
                ing.tomatoe--;
                ing.noTomatoe = 0x01;
            }
            break;
        default:
            if (ingredient[1] == 'e')
            {
                if (likes) ing.pepper++;
                else
                {
                    ing.pepper--;
                    ing.noPepper = 0x01;
                }
            } else {
                if (likes) ing.pineapple++;
                else
                {
                    ing.pineapple--;
                    ing.noPineapple = 0x01;
                }
            }
            break;
    }
}


/**
 * Loops through the whole like extracting the number and names of the ingredients.
 *
 * @param line Line extracted from the file
 * @param lineSize Size of the line extracted from the file
 * @param likes Flag to check whether the client likes the ingredient or not
 * @return 0 on success, 1 if wrong format
 */
int processLine(char* line, const size_t lineSize, const unsigned char likes)
{
    char* tmpData = NULL;
    tmpData = strtok(line, " ");
    if (tmpData == NULL)
    {
        return 1;
    } // if (tmpData == NULL)
    int numIngredients = atoi(tmpData);
    if (numIngredients == 0)
    {
        return 0;
    } // if (numIngredients == 0)
    for (int it = 0; it < numIngredients; it++)
    {
        tmpData = strtok(NULL, " ");
        if (tmpData == NULL)
        {
            return 1;
        } // if (tmpData == NULL)
        determineIngredient(tmpData, likes);
    } // for (int it = 0; it < numIngredients; it++)
    return 0;
}

/**
 * Iterates through the whole file for processing the number of potential clients and each of the lines
 * with the client specifications. Also determines by the line position if the client likes the line or not.
 *
 * @param fileName Name of the file to be processed
 * @return 0 on success, 1 on IO error, 2 if the file could not be opened, 3 if there are no potential clients read
 */
int readFile(const char* fileName)
{
    FILE *fileIn = NULL;
    fileIn = fopen(fileName, "r");
    if (fileIn == NULL) return 2;
    
    char* buffer = (char*) calloc(BUFFER_SIZE, sizeof(char));
    const size_t realBufferSize = BUFFER_SIZE * sizeof(char);
    size_t bufferSize = BUFFER_SIZE * sizeof(char);
    ssize_t bytesRead = getline(&buffer, &bufferSize, fileIn);
    if (bytesRead == -1)
    {
        fclose(fileIn);
        return 1;
    } // if (bytesRead == -1)
    const int potentialClients = atoi(buffer);
    if (potentialClients == 0)
    {
        fclose(fileIn);
        return 3;
    } // if (potentialClients == 0)
    for (int it = 0; it < potentialClients*2; it++)
    {
        bufferSize = bufferSize > realBufferSize ? bufferSize : realBufferSize;
        bytesRead = getline(&buffer, &bufferSize, fileIn);
        if (bytesRead == -1 || processLine(buffer, bufferSize, (it % 2 == 0)))
        {
            fclose(fileIn);
            return 3;
        } // if (bytesRead == -1)
    } // for (int it = 0; it < potentialClients; it++)
    fclose(fileIn);
    return 0;
}

void worstCaseDecision(char** decision)
{
    int maxNum = ing.cheese;
    char ingredient[BUFFER_SIZE] = "cheese";
    if (ing.basil > maxNum) {
        maxNum = ing.basil;
        strcpy(ingredient, "basil");
    }
    if (ing.mushroom > maxNum)
    {
        maxNum = ing.mushroom;
        strcpy(ingredient, "mushrooms");
    }
    if (ing.pepper > maxNum)
    {
        maxNum = ing.pepper;
        strcpy(ingredient, "pepper");
    }
    if (ing.pineapple > maxNum)
    {
        maxNum = ing.pineapple;
        strcpy(ingredient, "pineapple");
    }
    if (ing.tomatoe > maxNum)
    {
        maxNum = ing.tomatoe;
        strcpy(ingredient, "tomatoes");
    }
    strcpy(decision[0], ingredient);
}

void decideIngredients(char* decision[MAX_INGREDIENTS], int* numElement)
{
    *numElement = 0;
    if (!ing.noCheese)
    {
        strcpy(decision[*numElement], "cheese");
        *numElement = *numElement + 1;
    }
    if (!ing.noBasil)
    {
        strcpy(decision[*numElement], "basil");
        *numElement = *numElement + 1;
    }
    if (!ing.noMushroom)
    {
        strcpy(decision[*numElement], "mushrooms");
        *numElement = *numElement + 1;
    }
    if (!ing.noPepper)
    {
        strcpy(decision[*numElement], "pepper");
        *numElement = *numElement + 1;
    }
    if (!ing.noPineapple)
    {
        strcpy(decision[*numElement], "pineapple");
        *numElement = *numElement + 1;
    }
    if (!ing.noTomatoe)
    {
        strcpy(decision[*numElement], "tomatoes");
        *numElement = *numElement + 1;
    }
    if (!numElement)
    {
        worstCaseDecision(decision);
        *numElement = 1;
    }
}

int outputFile(const char* destination, const int numElements, char* elements[MAX_INGREDIENTS])
{
    FILE* outFile = NULL;
    outFile = fopen(destination, "wc");
    if (outFile == NULL) return 2;
    char* message = (char*) calloc(BUFFER_SIZE, sizeof(char));
    const size_t messageSize = BUFFER_SIZE * sizeof(char);
    for (int it = 0; it < numElements; it++)
    {
        strncat(message, elements[it], messageSize);
        strncat(message, " ", messageSize);
    }
    fprintf(outFile, "%d %s", numElements, message);
    printf("%d %s", numElements, message);
    fflush(outFile);
    fclose(outFile);
    return 0;
}

int main(int argc, const char * argv[])
{
    if (readFile(argv[1])) return 1;
    char *ingredients[MAX_INGREDIENTS] = {};
    for (int it = 0; it < MAX_INGREDIENTS; it++)
    {
        ingredients[it] = (char*) calloc(BUFFER_SIZE, sizeof(char));
    }
    int numElements = 0;
    decideIngredients(ingredients, &numElements);
    int outputed = outputFile(argv[2], numElements, ingredients);
    for (int it = 0; it < MAX_INGREDIENTS; it++)
    {
        free(ingredients[it]);
    }
    if (outputed) return 2;
    return 0;
}

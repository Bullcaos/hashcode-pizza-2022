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

#define BUFFER_SIZE (2048)

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
 * @param likes Flag to check whether the client likes the ingredient or not
 * @return 0 on success, 1 if wrong format
 */
int processLine(char* line, const unsigned char likes)
{
    char* tmpData = NULL;

    // Split the line from the space char
    tmpData = strtok(line, " ");
    if (tmpData == NULL)
    {
        return 1;
    } // if (tmpData == NULL)

    // The first number is the number of ingredients
    int numIngredients = atoi(tmpData);
    if (numIngredients == 0)
    {
        return 0;
    } // if (numIngredients == 0)

    // Then, loop though all ingredients
    for (int it = 0; it < numIngredients; it++)
    {
        tmpData = strtok(NULL, " ");
        if (tmpData == NULL)
        {
            return 1;
        } // if (tmpData == NULL)

        // And determine which element is that in the Ingredients structure
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
    // Open the file for reading
    FILE *fileIn = NULL;
    fileIn = fopen(fileName, "r");
    // If the file cannot be opened, return an error
    if (fileIn == NULL) return 2;

    // Initialize the buffer where all lines will be stored
    char* buffer = (char*) calloc(BUFFER_SIZE, sizeof(char));
    const size_t realBufferSize = BUFFER_SIZE * sizeof(char);
    size_t bufferSize = BUFFER_SIZE * sizeof(char);

    // Read the file's first line
    ssize_t bytesRead = getline(&buffer, &bufferSize, fileIn);

    // If none have been read, consider it as a read failure
    if (bytesRead == -1)
    {
        fclose(fileIn);
        return 1;
    } // if (bytesRead == -1)

    // The first number represents the number of potential clients
    const int potentialClients = atoi(buffer);
    // The must be at least one, otherwise it is a read error
    if (potentialClients == 0)
    {
        fclose(fileIn);
        return 3;
    } // if (potentialClients == 0)

    // Loop through all potential clients
    for (int it = 0; it < potentialClients*2; it++)
    {
        bufferSize = bufferSize > realBufferSize ? bufferSize : realBufferSize;
        bytesRead = getline(&buffer, &bufferSize, fileIn);
        // Only pair lines (except the first one) are considered as likes
        if (bytesRead == -1 || processLine(buffer, (it % 2 == 0)))
        {
            fclose(fileIn);
            return 3;
        } // if (bytesRead == -1)
    } // for (int it = 0; it < potentialClients; it++)

    // Close the file handler and return the success code
    fclose(fileIn);
    return 0;
}

/**
 * In the case where none of the ingredients is liked by everyone, the less hated ingredient shall be the chosen one.
 *
 * @param decision Pointer where the less hated ingredient will be stored
 */
void worstCaseDecision(char** decision)
{
    int maxNum = ing.cheese;
    char ingredient[BUFFER_SIZE] = "cheese";
    if (ing.basil > maxNum)
    {
        maxNum = ing.basil;
        strcpy(ingredient, "basil");
    } // if (ing.basil > maxNum)
    if (ing.mushroom > maxNum)
    {
        maxNum = ing.mushroom;
        strcpy(ingredient, "mushrooms");
    } // if (ing.mushroom > maxNum)
    if (ing.pepper > maxNum)
    {
        maxNum = ing.pepper;
        strcpy(ingredient, "pepper");
    } // if (ing.pepper > maxNum)
    if (ing.pineapple > maxNum)
    {
        maxNum = ing.pineapple;
        strcpy(ingredient, "pineapple");
    } // if (ing.pineapple > maxNum)
    if (ing.tomatoe > maxNum)
    {
        maxNum = ing.tomatoe;
        strcpy(ingredient, "tomatoes");
    } // if (ing.tomatoe > maxNum)
    strcpy(decision[0], ingredient);
}

/**
 * Decide which ingredients will get more clients, choosing first the ones that are not being hated, and, if all of
 * them are being hated my someone, then choose the less hated.
 *
 * @param decision The name of the chosen ingredients
 * @param numElement Number of chosen ingredients
 */
void decideIngredients(char* decision[MAX_INGREDIENTS], int* numElement)
{
    // Reset the number of elements to 0
    *numElement = 0;

    // Choose all those that are not being hated
    if (!ing.noCheese)
    {
        strcpy(decision[*numElement], "cheese");
        *numElement = *numElement + 1;
    } // if (!ing.noCheese)
    if (!ing.noBasil)
    {
        strcpy(decision[*numElement], "basil");
        *numElement = *numElement + 1;
    } // if (!ing.noBasil)
    if (!ing.noMushroom)
    {
        strcpy(decision[*numElement], "mushrooms");
        *numElement = *numElement + 1;
    } // if (!ing.noMushroom)
    if (!ing.noPepper)
    {
        strcpy(decision[*numElement], "pepper");
        *numElement = *numElement + 1;
    } // if (!ing.noPepper)
    if (!ing.noPineapple)
    {
        strcpy(decision[*numElement], "pineapple");
        *numElement = *numElement + 1;
    } // if (!ing.noPineapple)
    if (!ing.noTomatoe)
    {
        strcpy(decision[*numElement], "tomatoes");
        *numElement = *numElement + 1;
    } // if (!ing.noTomatoe)

    // If all of them have haters, then choose the less hated
    if (!*numElement)
    {
        worstCaseDecision(decision);
        *numElement = 1;
    } // if (!*numElement)
}

/**
 * Output the chosen ingredients into the output file required by Google Judge System
 *
 * @param destination Where the file shall be stored
 * @param numElements Number of ingredients that have been chosen
 * @param elements Ingredients being chosen
 * @return 0 on success, 2 on IO error
 */
int outputFile(const char* destination, const int numElements, char* elements[MAX_INGREDIENTS])
{
    // Open the file
    FILE* outFile = NULL;
    outFile = fopen(destination, "wc");
    // If the file cannot be opened, return error
    if (outFile == NULL) return 2;

    // Initialize where the output message will be stored
    char* message = (char*) calloc(BUFFER_SIZE, sizeof(char));
    const size_t messageSize = BUFFER_SIZE * sizeof(char);

    // Join the elements into a text separated by spaces
    for (int it = 0; it < numElements; it++)
    {
        strncat(message, elements[it], messageSize);
        strncat(message, " ", messageSize);
    } // for (int it = 0; it < numElements; it++)

    // Output all results into the file and console
    fprintf(outFile, "%d %s", numElements, message);
    printf(" Number of ingredients: %d\nIngredients: %s\n", numElements, message);

    // Flush file output buffers and close the file
    fflush(outFile);
    fclose(outFile);

    // Report success
    return 0;
}

/**
 * Main function for launching and coordinating the project
 *
 * @param argc Count of console arguments
 * @param argv Vector of console arguments
 * @return 0 on success, 1 if unable to read file, 2 if unable to print output file, 3 on syntax error
 */
int main(int argc, const char * argv[])
{
    // Handle the syntax error case
    if (argc != 3)
    {
        puts("Syntax error. The calling syntax shall be the following:\n");
        printf("\t%s <input file> <output file>\n", argv[0]);
        puts("Where:\n");
        puts("\t- <input file>: The name of the problem input file using Google's syntax.\n");
        puts("\t- <output file>: The name of the file where the solution shall be outputed.\n\n");
        return 3;
    }

    // Parse all required inputs
    if (readFile(argv[1])) return 1;

    // Initialize the structure where all decided ingredients name will be stored
    char *ingredients[MAX_INGREDIENTS] = {};
    for (int it = 0; it < MAX_INGREDIENTS; it++)
    {
        ingredients[it] = (char*) calloc(BUFFER_SIZE, sizeof(char));
    } // for (int it = 0; it < MAX_INGREDIENTS; it++)
    int numElements = 0;

    // Decide which ingredients will be the chosen ones
    decideIngredients(ingredients, &numElements);

    // Output the decision into a specified file
    int outputed = outputFile(argv[2], numElements, ingredients);

    // Free heap memory allocation
    for (int it = 0; it < MAX_INGREDIENTS; it++)
    {
        free(ingredients[it]);
    } // for (int it = 0; it < MAX_INGREDIENTS; it++)

    // If the file has not been written, return an error 2
    if (outputed) return 2;

    // Otherwise, return the succeed status
    return 0;
}

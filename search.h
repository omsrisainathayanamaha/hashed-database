#ifndef SEARCH_H
#define SEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 1024

/**
 * int searchInFile(const char *filename, const char *search_str)
 * 
 * Searches a file for a specified search string.
 * 
 * @return 1 if found, 0 if not found.
 */
int searchInFile(const char *filename, const char *search_str) {
    FILE *file = fopen(filename, "r");  // Open file for reading
    if (!file) {
        perror("Error opening file");
        return -1;  // Return -1 for error
    }

    char line[MAX_LINE_LENGTH];

    // Read file line by line
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        if (strstr(line, search_str)) {  // Check if search_str exists in the line
            fclose(file);
            return 1;  // String found
        }
    }

    fclose(file);
    return 0;  // String not found
}
/**
 * int searchInLine(const char* line, const char* search_str)
 * 
 * Searches for search_str in line.
 * 
 * @return 1 if search_str is in line, 0 otherwise.
 */
int searchInLine(const char* line, const char* search_str)
{
    if (strstr(line, search_str)) {  // Check if search_str exists in the line
        return 1;  // String found
    }
    return 0; // String not found
}
/**
 *  int checkFileExistence(const char* filename)
 * 
 *  @return 1 if the file exists, 0 if not.
 *  @param filename The filename to check.
 */
int checkFileExistence(const char* filename)
{
    if(!access(filename, F_OK))
        return 1;
    else
        return 0;
}

#endif
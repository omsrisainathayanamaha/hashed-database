#include <stdio.h>
#include "constants.h"
#include <string.h>
#include <xxhash.h>
#include <unistd.h>
#include "search.h"
char** chunker(const char* toChunk, int* size);
int checkFileExistence(const char* filename);
int insertContentsToFile(const char* data, const char* filename);
int appendContentsToFile(const char* data, const char* filename);
void appendHash(char* keyHash, int i, int chunks, char** hashes, char* directoried);
/** 
 * int chunkStorer(const char* key, const char* data)
 * Chunks data into n chunks of size CHUNK_BYTES each, and stores it in txt files in DIRECTORY.
 * Hashes data and links each file to the next using the last line in the file.
 * The key must be unique.
 * @return n, the amount of chunks the data was stored in. n = 0 for non-unique key failure
*/
int chunkStorer(const char* key, const char* data)
{
    FILE *appendageHash = fopen(HASH_FILE, "a+"); //the hash file
    int chunks = 0; //the amount of chunks in the array
    char** arr = chunker(data, &chunks); //the chunked array
    char* hashes[chunks]; //the hashes
    for(int i = 0; i < chunks; i++)
    {
        hashes[i] = malloc(sizeof(XXH64_hash_t));
        sprintf(hashes[i], "%llu", XXH64(arr[i], strlen(arr[i]), 0)); //prints a hash as a string to the hash array
        
    }
    char* keyHash = malloc(sizeof(XXH64_hash_t));
    sprintf(keyHash, "%llu", XXH64(key, strlen(key), 0));
    char* extension = malloc(strlen(hashes[0])+2);
    
    strcpy(extension, ":");
    strcat(extension, hashes[0]);
    char* searchStr = malloc(strlen(keyHash)+2);
    strcpy(searchStr, keyHash);
    strcat(searchStr, ":");
    
    if(!searchInFile(HASH_FILE, searchStr)){
        char* keyHashTmp = malloc(strlen(keyHash)+strlen(NEW_LINE)+1);
        strcpy(keyHashTmp, keyHash);
        strcat(keyHashTmp, extension);
        strcat(keyHashTmp, NEW_LINE);
        appendContentsToFile(keyHashTmp, HASH_FILE);
        free(keyHashTmp);
        
    }else{
        free(searchStr);
        return 0;
    }
    free(searchStr); // freed out here because of scope?
    char* directoried = malloc(strlen(DIRECTORY)+sizeof(XXH64_hash_t)+1); //the directorized current filename
    for(int i = 0; i < chunks; i++)
    {
        strcpy(directoried, DIRECTORY);
        strcat(directoried, hashes[i]);

        if(checkFileExistence(directoried)) //if the file exists
        {
            appendHash(keyHash, i, chunks, hashes, directoried);
        }else{ //if the file doesn't exist
            insertContentsToFile(arr[i], directoried); //insert the contents
            appendHash(keyHash, i, chunks, hashes, directoried);
        }
    }
    return chunks;
    
}
/**
 * Appends hash to end of file.
 * 
 * @param keyHash The hash of the key.
 * @param i The index of the key.
 * @param chunks The amount of chunks extant in the datasystem.
 * @param hashes The internal pointer variable to the array containing hashes.
 * @param directoried A directorized filename, wherein storage can occur.
 */
void appendHash(char* keyHash, int i, int chunks, char** hashes, char* directoried)
{
            char* taggedKey = malloc(strlen(TAG_STRING)+strlen(keyHash)+1); //allocate memory for the appendage
            strcpy(taggedKey, TAG_STRING);
            strcat(taggedKey, keyHash);
            appendContentsToFile(taggedKey, directoried); //does a \u count?
            free(taggedKey); //free the memory allocated for the appendage
            
            if(i+1 == chunks)
            {
                appendContentsToFile(":END", directoried);
            }else{
                char* appendage = malloc(strlen(":")+1+strlen(hashes[i+1]));
                strcpy(appendage, ":");
                strcat(appendage, hashes[i+1]);
                appendContentsToFile(appendage, directoried);
                free(appendage);
            }
}
/**
 * char** chunker(const char* toChunk, int* size)
 * Chunks data into n size chunks of size CHUNK_BYTES each.
 * Note: size is changed to the length of the array
 * @return the pointer to the first element of the array
 * @param toChunk the string to be chunked
 * @param size the size of the string chunked
 */
char** chunker(const char* toChunk, int* size)
{
    int stringLength = strlen(toChunk);
    *size = (stringLength / CHUNK_BYTES) + 1;
    char** arr = malloc(sizeof(*size * sizeof(char*))); //the array that will hold it all
    
    
    for (int i = 0; i < stringLength; i++)
    {
        if(i == 0)
        {
            arr[i] = malloc((CHUNK_BYTES+1)*sizeof(char)); //allocates memory
        }
        arr[i/CHUNK_BYTES][i % CHUNK_BYTES] = toChunk[i];
        if ((!(i % CHUNK_BYTES) && i != 0) || i==stringLength-1)
        {
            arr[i/CHUNK_BYTES][i%(CHUNK_BYTES+1)] = '\0'; //finishes up
            arr[(i/CHUNK_BYTES)+1] = malloc((CHUNK_BYTES+1)*sizeof(char)); //allocates memory
        }
    }
    return arr;
}
/**
 * int insertContentsToFile(const char* data, const char* filename)
 * 
 * Inserts data to filename.
 * filename must be in the data directory.
 * Note: destructive.
 * @return 1 if data successfully inserted, 0 if not.
 * @param data The data to be inserted.
 * @param filename The filename where the data should be inserted.
 */
int insertContentsToFile(const char* data, const char* filename)
{
    system("[ -d ./data ] || mkdir ./data"); //checks if directory exists; if not creates it.
    FILE *file = fopen(filename, "w");
    
    if(file == NULL)
    {
        perror("File did not open! Insertion failed");
        return -1;
    }
    if(fprintf(file, "%s", data))
    {
        fclose(file);
        return 1;
    }
    fclose(file);
    return 0;
}
/**
 * int appendContentsToFile(const char* data, const char* filename)
 * 
 * Appends data to filename.
 * Note: Non-destructive
 * @return 1 if successful, 0 if not.
 * @param data The data to append.
 * @param filename The filename at which to append the data.
 */
int appendContentsToFile(const char* data, const char* filename)
{
    FILE *file = fopen(filename, "a+");
    if(file == NULL)
    {
        perror("File did not open! Append failed");
        return -1;
    }
    if(fprintf(file, "%s", data))
    {
        fclose(file);
        return 1;
    }
    fclose(file);
    return 0;
}

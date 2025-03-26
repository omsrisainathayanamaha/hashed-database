#include <xxhash.h>
#include "search.h"
#include "constants.h"

/**
 * char* keyToHash(char* key)
 * 
 * Converts the key to a hash by which the database is indexable.
 * 
 * @return the converted hash as a char*
 */
char* keyToHash(char* key)
{
    char* keyHash; //malloc?
    sprintf(keyHash, "%llu", XXH64(key, strlen(key), 0));

    return keyHash;
}
/**
 * char* chunkRetriever(char* key)
 * 
 * Retrieves the chunks of key.
 * @param key the key to use to retrieve data.
 * @return The reconstructed data.
 */
char* chunkRetriever(char* key)
{
    char* hash = keyToHash(key); //The hash of the key
    if(searchInFile(HASH_FILE, hash))
    {

    }
    return ""; //fill in statement
}

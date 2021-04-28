/*
 * @file key_table.h
 * @brief Provides hash table for Key lookup.
 */

#ifndef INCLUDE_KEY_TABLE_H_
#define INCLUDE_KEY_TABLE_H_

#include <stdbool.h>

#include "CharBuf.h"

typedef struct TableEntry
{
  CharBuf key_id;
  CharBuf key;
} KeyEntry;

typedef struct Keys
{
  KeyEntry *entries;
  size_t num_entries;
  size_t mem_size;
} KeyTable;

extern KeyTable key_table;

/**
 * <pre>
 * This function initializes a key table for lookup.
 * </pre>
 *
 * @param[in] key_table The pointer for key table to be initialized
 * @param[out] key_table The initialized key table
 *
 * @return 0 on success, 1 on error
 */
int key_table_init(void);

/**
 * <pre>
 * This function destroys the key table.
 * </pre>
 *
 * @param[in] key_table The key table to be destroyed
 *
 * @return 0 on success, 1 on error
 */
int key_table_destroy(void);

/**
 * <pre>
 * This function to delete values in hash table based on location in key_id.
 * </pre>
 *
 * @param[in] key_id.chars Key identifier assumed to be null terminated
 * @param[in] key_id.len The length of the key identifier
 * @param[in] key_table The key table that the key needs to be deleted from
 * @param[out] key_table The key table without the deleted key
 *
 * @return 0 on success, 1 on error
 */
int key_table_delete(CharBuf key_id);

/**
 * <pre>
 * This function to add values in hash table based on location in key_id.
 * </pre>
 *
 * @param[in] key_id.chars Key identifier assumed to be null terminated
 * @param[in] key_id.len The length of the key identifier
 * @param[out] key.chars The key value
 * @param[out] key.len The length of the key
 * @param[in] key_table The key table that the new key needs to be added to
 * @param[out] key_table The key table with the new added key
 *
 * @return 0 on success, 1 on error
 */
int key_table_add(CharBuf key_id, CharBuf * key);

/**
 * <pre>
 * This function performs lookup of Keys by Identifier. Note the mutex needs to be unlocked by the function that calls this function.
 * </pre>
 *
 * @param[in] key_id.chars The key identifier assumed to be null terminated
 * @param[in] key_id.len Length of Key Identifier
 * @param[out] key.chars The key value
 * @param[out] key.len The length of the key
 * @param[in] key_table The key table that has the key and is used for the lookup
 * @param[out] key_table The key table returned if key was added
 * @param max_key_entries The max number of key entries for the table default set at 100000
 *
 * @return 0 on success, 1 on failure
 */
int key_table_lookup(CharBuf key_id, CharBuf * key);

#endif /* INCLUDE_KEY_TABLE_H_ */
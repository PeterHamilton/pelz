/*
 * key_table_test_suite.c
 */

#include "key_table_test_suite.h"
#include "test_helper_functions.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <charbuf.h>
#include <pelz_log.h>

// Adds all key table tests to main test runner.
int key_table_suite_add_tests(CU_pSuite suite)
{
  if (NULL == CU_add_test(suite, "Test Key Table Initialization/Destruction", test_table_initDestroy))
  {
    return (1);
  }
  if (NULL == CU_add_test(suite, "Test all combinations for adding keys to Key Table", test_table_initAddDestroy))
  {
    return (1);
  }
  if (NULL == CU_add_test(suite, "Test all Key Table Lookup combinations", test_table_initLookupAddDestroy))
  {
    return (1);
  }
  if (NULL == CU_add_test(suite, "Test Key Table Delete", test_table_initLookupAddDeleteDestroy))
  {
    return (1);
  }
  return (0);
}

void test_table_initDestroy(void)
{
  CU_ASSERT(key_table_init() == 0);
  CU_ASSERT(key_table_destroy() == 0);
}

void test_table_initAddDestroy(void)
{
  charbuf tmp;
  charbuf key;
  char *prefix = "file:";
  char *valid_id[3] = { "/test/key1.txt", "/test/key2.txt", "/test/key3.txt" };
  char *tmp_id;

  pelz_log(LOG_DEBUG, "Test Key Table Add Function");
  CU_ASSERT(key_table_init() == 0);

  //Test that the keys are added to the key table
  for (int i = 0; i < 3; i++)
  {
    tmp = copy_CWD_to_id(prefix, valid_id[i]);
    CU_ASSERT(key_table_add(tmp, &key) == 0);
    free_charbuf(&tmp);
    free_charbuf(&key);
  }

  //Test that keys are added if valid without checking if already in table
  for (int i = 0; i < 3; i++)
  {
    tmp = copy_CWD_to_id(prefix, valid_id[i]);
    CU_ASSERT(key_table_add(tmp, &key) == 0);
    free_charbuf(&tmp);
    free_charbuf(&key);
  }

  //Test that non-valid keys are not added
  tmp_id = "/test/key7.txt";
  tmp = copy_CWD_to_id(prefix, tmp_id);
  CU_ASSERT(key_table_add(tmp, &key) == 1);
  free_charbuf(&tmp);
  free_charbuf(&key);

  CU_ASSERT(key_table_destroy() == 0);
  pelz_log(LOG_DEBUG, "Test Key Table Add Function Complete");
}

void test_table_initLookupAddDestroy(void)
{
  charbuf key;
  charbuf tmp;
  char *prefix = "file:";
  char *valid_id[3] = { "/test/key1.txt", "/test/key2.txt", "/test/key3.txt" };
  char *tmp_id;

  pelz_log(LOG_DEBUG, "Test Key Table Lookup Function");
  CU_ASSERT(key_table_init() == 0);

  //Initial check if the keys are added when the lookup does not find them
  for (int i = 0; i < 3; i++)
  {
    tmp = copy_CWD_to_id(prefix, valid_id[i]);
    CU_ASSERT(key_table_add(tmp, &key) == 0);
    secure_free_charbuf(&key);
    free_charbuf(&tmp);
  }

  //Check that the keys are found and not added twice
  for (int i = 0; i < 3; i++)
  {
    tmp = copy_CWD_to_id(prefix, valid_id[i]);
    CU_ASSERT(key_table_lookup(tmp, &key) == 0);
    secure_free_charbuf(&key);
    free_charbuf(&tmp);
  }

  //Check that non-valid file does not load key
  tmp_id = "/test/key7.txt";
  tmp = copy_CWD_to_id(prefix, tmp_id);
  CU_ASSERT(key_table_lookup(tmp, &key) == 1);
  secure_free_charbuf(&key);
  free_charbuf(&tmp);

  tmp_id = "/test/key1txt";
  tmp = copy_CWD_to_id(prefix, tmp_id);
  CU_ASSERT(key_table_lookup(tmp, &key) == 1);
  secure_free_charbuf(&key);
  free_charbuf(&tmp);

  CU_ASSERT(key_table_destroy() == 0);
  pelz_log(LOG_DEBUG, "Test Key Table Lookup Function");
}

void test_table_initLookupAddDeleteDestroy(void)
{
  charbuf key;
  charbuf tmp;
  char *prefix = "file:";

  char *valid_id[6] = { "/test/key1.txt", "/test/key2.txt", "/test/key3.txt",
    "/test/key4.txt", "/test/key5.txt", "/test/key6.txt"
  };
  char *tmp_id;

  pelz_log(LOG_DEBUG, "Test Key Table Lookup Function");
  CU_ASSERT(key_table_init() == 0);
  //Initial load of keys into the key table
  for (int i = 0; i < 6; i++)
  {
    tmp = copy_CWD_to_id(prefix, valid_id[i]);
    CU_ASSERT(key_table_add(tmp, &key) == 0);
    secure_free_charbuf(&key);
    free_charbuf(&tmp);
  }

  //Testing the delete function
  tmp = copy_CWD_to_id(prefix, valid_id[3]);
  CU_ASSERT(key_table_delete(tmp) == 0);
  free_charbuf(&tmp);

  tmp = copy_CWD_to_id(prefix, valid_id[5]);
  CU_ASSERT(key_table_delete(tmp) == 0);
  free_charbuf(&tmp);

  tmp = copy_CWD_to_id(prefix, valid_id[0]);
  CU_ASSERT(key_table_delete(tmp) == 0);
  free_charbuf(&tmp);

  //Testing that if the delete function does not find key_id then does not delete for valid files and non-valid files
  tmp_id = "/test/key.txt";
  tmp = copy_CWD_to_id(prefix, tmp_id);
  CU_ASSERT(key_table_delete(tmp) == 1);
  free_charbuf(&tmp);

  tmp_id = "/test/key1txt";
  tmp = copy_CWD_to_id(prefix, tmp_id);
  CU_ASSERT(key_table_delete(tmp) == 1);
  free_charbuf(&tmp);

  tmp = new_charbuf(strlen("adaj;ldkjidka;dfkjai"));
  memcpy(tmp.chars, "adaj;ldkjidka;dfkjai", tmp.len);
  CU_ASSERT(key_table_delete(tmp) == 1);
  free_charbuf(&tmp);

  CU_ASSERT(key_table_destroy() == 0);
  pelz_log(LOG_DEBUG, "Test Key Table Lookup Function");
}

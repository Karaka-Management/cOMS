#include "../TestFramework.h"
#include "../../stdlib/HashMap.h"

static void test_hash_alloc() {
    HashMap hm = {};
    hashmap_alloc(&hm, 3, sizeof(HashEntryInt32));

    ASSERT_NOT_EQUALS(hm.table, NULL);
    ASSERT_TRUE(hm.buf.count > 0);

    hashmap_free(&hm);
    ASSERT_EQUALS(hm.buf.size, 0);
    ASSERT_EQUALS(hm.buf.memory, NULL);
}

static void test_hash_insert_int32() {
    HashMap hm = {};
    hashmap_alloc(&hm, 3, sizeof(HashEntryInt32));

    HashEntryInt32* entry;

    hashmap_insert(&hm, "test1", 1);
    entry = (HashEntryInt32 *) hashmap_get_entry(&hm, "test1");
    ASSERT_NOT_EQUALS(entry, NULL);
    ASSERT_EQUALS(entry->value, 1);

    hashmap_insert(&hm, "test2", 2);
    entry = (HashEntryInt32 *) hashmap_get_entry(&hm, "test2");
    ASSERT_NOT_EQUALS(entry, NULL);
    ASSERT_EQUALS(entry->value, 2);

    hashmap_insert(&hm, "test3", 3);
    entry = (HashEntryInt32 *) hashmap_get_entry(&hm, "test3");
    ASSERT_NOT_EQUALS(entry, NULL);
    ASSERT_EQUALS(entry->value, 3);

    entry = (HashEntryInt32 *) hashmap_get_entry(&hm, "invalid");
    ASSERT_EQUALS(entry, NULL);

    hashmap_free(&hm);
}

#ifdef UBER_TEST
    #ifdef main
        #undef main
    #endif
    #define main StdlibHashMapTest
#endif

int main() {
    TEST_INIT(25);

    RUN_TEST(test_hash_alloc);
    RUN_TEST(test_hash_insert_int32);

    TEST_FINALIZE();

    return 0;
}
#include "hash_table.h"
#include "dns.h"

HashTable* InitHashTable()
{
	HashTable* now = malloc(sizeof(HashTable));
	if (now != NULL) {
		now->size = ALL_SIZE;
		now->items = calloc(now->size, sizeof(ItemList*));
		if (now->items != NULL) {
			return now;
		}
		free(now);
	}
	return INVALID_HASH_TABLE;
}

ItemList* InitItemList(const char* key, IPADDRESS value) {
	ItemList* item = malloc(sizeof(ItemList));
	if (item != NULL) {
		size_t length = strlen(key) + 1;
		item->key = calloc(length, sizeof(char));
		if (item->key != NULL) {
			strcpy_s(item->key, length, key);
			item->next = NULL;
			item->value = value;
			return item;
		}
		free(item);
	}
	return INVALID_ITEM;
}

void DestroyItemList(ItemList* item) {
	ItemList* next = NULL;
	while (item != NULL) {
		next = item->next;
		free(item->key);
		free(item);
		item = next;
	}
}

void DestroyHashTable(HashTable* table) {
	for (unsigned i = 0; i < table->size; i++) {
		DestroyItemList(table->items[i]);
	}
	free(table->items);
	free(table);
	table = INVALID_HASH_TABLE;
}


void InsertToHashTable(HashTable* table, const char* key, IPADDRESS value) {
	ItemList* toInsert = InitItemList(key, value);
	if (toInsert == INVALID_ITEM) return;
	ItemList** now = &table->items[Hash(key)];
	while (*now != NULL) {
		now = &(*now)->next;
	}
	*now = toInsert;
}

IPADDRESS FindInHashTable(HashTable* table, const char* key) {
	ItemList* now = table->items[Hash(key)];
	while (now != NULL) {
		if (strcmp(now->key, key) == 0) {
			return now->value;
		}
		now = now->next;
	}
	return INVALID_IP_ADDRESS;
}

unsigned int Hash(const char* key) {
	unsigned long hash = 5381;
	int c;
	while (c = *key++)
		hash = ((hash << 5) + hash) + c;
	return hash % ALL_SIZE;
}

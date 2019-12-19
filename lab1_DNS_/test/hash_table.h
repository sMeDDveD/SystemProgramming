#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "dns.h"

#define INVALID_HASH_TABLE 0
#define INVALID_ITEM 0
#define ALL_SIZE 8000;

/*************************************************************************
 STRUCTURE:
	ItemList

 DESCRIPTION:
	Структура для хранения списка

 FIELDS:
	value - хранимый IP-адрес
	key - имя домена по данному адресу
	next - указатель на следующий элемент списка
************************************************************************/
typedef struct {
	IPADDRESS value;
	char* key;
	struct ItemList* next;
} ItemList;




/*************************************************************************
 STRUCTURE:
	HashTable

 DESCRIPTION:
	Структура для хранения хеш-таблицы

 FIELDS:
	size - размер хеш-таблицы
	items - массив списков, в которых хранятся значения
************************************************************************/
typedef struct {
	unsigned size;
	ItemList** items;
} HashTable;




/*************************************************************************
 FUNCTION:
	InitItemList()

 DESCRIPTION:
	Создаёт корневой элемент списка, с ключом - const char* key и
	значением - IPADDRESS value

 PARAMETERS:
	key - имя домена
	value - его IP-адрес

 RETURN VALUE:
	Указатель на начало списка, если получилось выделить память
	INVALID_ITEM - иначе
************************************************************************/
ItemList* InitItemList(const char* key, IPADDRESS value);




/*************************************************************************
 FUNCTION:
	DestroyItemList()

 DESCRIPTION:
	Освобождает память, выделенную под хранение списка

 PARAMETERS:
	item - указатель на начало списка
************************************************************************/
void DestroyItemList(ItemList* item);




/*************************************************************************
 FUNCTION:
	InitHashTable()

 DESCRIPTION:
	Создаёт хеш-таблицу, выделяя необходимые для её хранения ресурсы

 RETURN VALUE:
	Указатель на хеш-таблицу, если получилось выделить память
	INVALID_HASH_TABLE - иначе
************************************************************************/
HashTable* InitHashTable();




/*************************************************************************
 FUNCTION:
	DestroyHashTable()

 DESCRIPTION:
	Освобождает память, выделенную под хранение хеш-таблицы, а также её
	составных элементов

 PARAMETERS:
	table - указатель на хеш-таблицу
************************************************************************/
void DestroyHashTable(HashTable* table);




/*************************************************************************
 FUNCTION:
	InsertToHashTable()

 DESCRIPTION:
	Добавляет новый элемент в хеш-таблицу

 PARAMETERS:
	table - указатель на хеш-таблицу
	(key - имя домена, value - его IP-адрес) - элемент таблицы

 RETURN VALUE:
	Нет

 NOTE:
	Не проверяет валидность таблицы
************************************************************************/
void InsertToHashTable(HashTable* table, const char* key, IPADDRESS value);




/*************************************************************************
 FUNCTION:
	FindInHashTable()

 DESCRIPTION:
	Поиск элемент в хеш-таблице

 PARAMETERS:
	table - указатель на хеш-таблицу
	key - имя домена, для которого ищется IP-адрес

 RETURN VALUE:
	Если такой домен есть в таблице, то возвращает его IP-адрес,
	иначе - INVALID_IP_ADDRESS

 NOTE:
	Не проверяет валидность таблицы
************************************************************************/
IPADDRESS FindInHashTable(HashTable*, const char* key);




/*************************************************************************
 FUNCTION:
	Hash()

 DESCRIPTION:
	Вычисляет значение хеша для строки
	djb2 - http://www.cse.yorku.ca/~oz/hash.html

 PARAMETERS:
	key - строка, для которой нужно вычислить хеш

 RETURN VALUE:
	Хеш, полученной строки
************************************************************************/
static unsigned int Hash(const char* key);



#endif // !_HASH_TABLE_H


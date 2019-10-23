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
	��������� ��� �������� ������

 FIELDS:
	value - �������� IP-�����
	key - ��� ������ �� ������� ������
	next - ��������� �� ��������� ������� ������
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
	��������� ��� �������� ���-�������

 FIELDS:
	size - ������ ���-�������
	items - ������ �������, � ������� �������� ��������
************************************************************************/
typedef struct {
	unsigned size;
	ItemList** items;
} HashTable;




/*************************************************************************
 FUNCTION:
	InitItemList()

 DESCRIPTION:
	������ �������� ������� ������, � ������ - const char* key �
	��������� - IPADDRESS value

 PARAMETERS:
	key - ��� ������
	value - ��� IP-�����

 RETURN VALUE:
	��������� �� ������ ������, ���� ���������� �������� ������
	INVALID_ITEM - �����
************************************************************************/
ItemList* InitItemList(const char* key, IPADDRESS value);




/*************************************************************************
 FUNCTION:
	DestroyItemList()

 DESCRIPTION:
	����������� ������, ���������� ��� �������� ������

 PARAMETERS:
	item - ��������� �� ������ ������
************************************************************************/
void DestroyItemList(ItemList* item);




/*************************************************************************
 FUNCTION:
	InitHashTable()

 DESCRIPTION:
	������ ���-�������, ������� ����������� ��� � �������� �������

 RETURN VALUE:
	��������� �� ���-�������, ���� ���������� �������� ������
	INVALID_HASH_TABLE - �����
************************************************************************/
HashTable* InitHashTable();




/*************************************************************************
 FUNCTION:
	DestroyHashTable()

 DESCRIPTION:
	����������� ������, ���������� ��� �������� ���-�������, � ����� �
	��������� ���������

 PARAMETERS:
	table - ��������� �� ���-�������
************************************************************************/
void DestroyHashTable(HashTable* table);




/*************************************************************************
 FUNCTION:
	InsertToHashTable()

 DESCRIPTION:
	��������� ����� ������� � ���-�������

 PARAMETERS:
	table - ��������� �� ���-�������
	(key - ��� ������, value - ��� IP-�����) - ������� �������

 RETURN VALUE:
	���

 NOTE:
	�� ��������� ���������� �������
************************************************************************/
void InsertToHashTable(HashTable* table, const char* key, IPADDRESS value);




/*************************************************************************
 FUNCTION:
	FindInHashTable()

 DESCRIPTION:
	����� ������� � ���-�������

 PARAMETERS:
	table - ��������� �� ���-�������
	key - ��� ������, ��� �������� ������ IP-�����

 RETURN VALUE:
	���� ����� ����� ���� � �������, �� ���������� ��� IP-�����,
	����� - INVALID_IP_ADDRESS

 NOTE:
	�� ��������� ���������� �������
************************************************************************/
IPADDRESS FindInHashTable(HashTable*, const char* key);




/*************************************************************************
 FUNCTION:
	Hash()

 DESCRIPTION:
	��������� �������� ���� ��� ������
	djb2 - http://www.cse.yorku.ca/~oz/hash.html

 PARAMETERS:
	key - ������, ��� ������� ����� ��������� ���

 RETURN VALUE:
	���, ���������� ������
************************************************************************/
static unsigned int Hash(const char* key);



#endif // !_HASH_TABLE_H


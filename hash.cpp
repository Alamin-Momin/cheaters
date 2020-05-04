//hash.cpp
// Created by Alamin Momin on 4/30/20.
//cpp file for hash.h

#include <vector>
#include <string>
#include <iostream>
#include <cmath>

#include "hash.h"

HashTable::HashTable() {                        //initialize all the pointers in the hash table to NULL
    for (int i = 0; i < TABLE_SIZE; i++) {
        htable[i] = NULL;
    }
};

HashTable::~HashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode * headTemp = htable[i];
        HashNode * prev = NULL;
        while (headTemp != NULL) {
            prev = headTemp;
            headTemp = headTemp -> next;
            delete(prev);
        }
    }
}

int HashTable::hashFunc(const string key) {
    signed int hash = 0;
    for (int i = 0; i < key.size(); i++) {
        char k = key[key.size() - 1 - i];
        int temp  = key[key.size() - 1 - i];
        int mult = pow(7, i);
        temp = temp * mult;

        if(hash + temp < 0) {               //if overflow, then divide hash by 10 and just add the value of the character
            hash = hash/10;
            hash = hash+temp;
        }
        else {
            hash = hash+temp;
        }
    }

    int index;
    index = hash % TABLE_SIZE;
    if (index < 0) {
        index = index*-1;
    }
    return index;

}

void HashTable::add(const string &chunk, int fileIndex) {
    int hashIndex = hashFunc(chunk);

    HashNode *temp = new HashNode;
    temp -> fileIndex = fileIndex;
    temp -> next = NULL; //if there are no other nodes in this index, this will be the default pointer

    //check if node with this file index already exists in this hashTable index
    HashNode *checker = htable[hashIndex];
    bool repeat = false;

    while(checker != NULL) {
        if (checker->fileIndex == temp->fileIndex) {
            repeat = true;
            break;
        }
        checker = checker -> next;
    }

    if (!repeat) {
        temp -> next = htable[hashIndex];
        htable[hashIndex] = temp;
    }
}

void HashTable::printTable() const {
    for (int i = 0; i < TABLE_SIZE; i++) {
        cout << i;
        HashNode *head = htable[i];
        while (head != NULL) {
            cout <<  "--->" << head ->fileIndex;
            head = head->next;
        }
        cout << endl;
    }
}

int HashTable::getTableSize() const {
    return TABLE_SIZE;
}

void HashTable::getCounts(int *grid[]) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode * head = htable[i];
        HashNode * trail = htable[i];
        while (trail != NULL) {
            head = trail->next;
            while (head != NULL) {
                if(trail->fileIndex > head->fileIndex) {
                    grid[trail->fileIndex][head->fileIndex]++;
                }
                else {
                    grid[head->fileIndex][trail->fileIndex]++;
                }
                head = head->next;
            }
            trail = trail->next;
        }
    }



}

//hash.h
// Created by Alamin Momin on 4/30/20.
//header file for the hash table

#ifndef UNTITLED6_HASH_H
#define UNTITLED6_HASH_H

#include <vector>
#include <string>
#include <iostream>

using namespace std;

class HashTable {
    public:
        struct HashNode {
            int fileIndex;
            HashNode *next;
        };
        HashTable();
        ~HashTable();
        int hashFunc(const string key);
        void add(const string &chunk, int fileIndex);
        void printTable() const;
        int getTableSize() const;
        void getCounts(int *grid[]);

    private:
        static const int TABLE_SIZE  = 100007;
        HashNode* htable[TABLE_SIZE];

};





#endif //UNTITLED6_HASH_H

//plagarismCatcher.cpp
// Created by Alamin Momin on 4/30/20.
//main driver of the program

#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "hash.h"

using namespace std;

//struct for the vector table to store pairs of files with greatest similarity
struct similar {
    int simCount;
    string file1;
    string file2;
};
typedef struct similar Similar;

bool compare(Similar i, Similar j) {
    return (i.simCount < j.simCount);
}

int getdir (string dir, vector<string> &files);
void cleanUp (string &word);


int main(int argc, char *argv[]) {
    HashTable *hashT = new HashTable; // create new hash table

    string dir = string(argv[2]);         //get directory name
    vector<string> files; //create a vector of file names
    getdir(dir, files);                  //load all the files into the vector

    files.erase(files.begin());     //remove .home and parent directory before continuing
    files.erase(files.begin());

    //for each file in the vector of files: open file, retrieve chunk, put the chunk through hash function, place file index number in table
    int chunkLen = atoi(argv[3]);

    for (int i = 0; i < files.size(); i++) {
        string fileptr = dir + "/" + files[i];
        ifstream inputFile;
        inputFile.open(fileptr.c_str());

        if(inputFile.is_open()) {
            string word;
            vector<string> words;

            while(!inputFile.eof()) {
                while (words.size() < chunkLen && !inputFile.eof()) {
                    inputFile >> word;
                    cleanUp(word);

                    if (word.size() > 0) {
                        words.push_back(word);
                    }
                }

                string chunk;

                for (string j: words) {
                    chunk += j;
                }

                hashT->add(chunk, i);
                words.erase(words.begin());
            }
        }

        inputFile.close();
    }

    //create a 2-D Array to input values
    int numRows = files.size();
    int numCols = files.size();
    int *counts[numRows];
    for(int i=0; i<numRows; i++) {
        counts[i]=(int *) malloc(numCols*sizeof(int));
    }

    for(int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++){
            counts[i][j]=0;
        }
    }

    //populate the 2-D array
    hashT->getCounts(counts);

    //pull values greater than threshold and input them into a vector of 'similar' struct
    int threshold = atoi(argv[4]);
    vector<Similar> TopCheaters;
    for(int i =0; i <numRows; i++) {
        for (int j = 0; j<numCols; j++) {
            if (i>j) {
                if (counts[i][j] >= threshold) {
                    Similar submit;
                    submit.simCount = counts[i][j];
                    submit.file1 = files[i];
                    submit.file2 = files[j];
                    TopCheaters.push_back(submit);
                }
            }
        }
    }

    //sort the vector of similar files in descending order
    std::sort(TopCheaters.begin(), TopCheaters.end(), compare);
    std::reverse(TopCheaters.begin(), TopCheaters.end());

    //print the similar files and thier counts
    for (int i = 0; i < TopCheaters.size(); i++) {
        cout << TopCheaters[i].simCount << ": " << TopCheaters[i].file1 << ", " << TopCheaters[i].file2 << endl;
    }

    //delete 2-D array
    for(int i =0; i<numRows; i++) {
        free(counts[i]);
    }

    //destruct htable
    delete(hashT);

    return 0;
}

int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

void cleanUp (string &word) {
    for (int i = 0; i < word.size(); i++) {
        if ('A' <= word[i] && word[i] <= 'Z') { //make all letters lowercase
            word[i] = word[i] + 32;
        }
        if ((word[i] < 'a' || 'z' < word[i]) && (word[i] < '0' || '9' < word[i])) { //keeps only alphanumeric characters
            word.erase(word.begin()+i);
            i--;
        }
    }
}
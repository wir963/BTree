//
//  main.cpp
//  BTree
//
//  Created by Welles Robinson on 11/24/12.
//
//  Main is designed with the following constraints - the input parameter is a filename that contains another filename to load the values into the BTree as well as a series of operations to perform on the B Tree
//  


#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include "BTree.h"

using namespace std;

int main (int argc, const char * argv[])
{
    ifstream inFile;
    string lineFromFile;
    if(argc != 2){
        cerr << "Usage: inputStream filename " << endl;
        exit(1);
    }
    inFile.open(argv[1]);
    if (!inFile.good()) {
        cerr << "Error opening file " << argv[1] << endl;
    } 
    BTree<int, 6> bt;
    while( std::getline(inFile, lineFromFile) ){
        stringstream s(lineFromFile);
        string opcode;
        string instr;
        s >> opcode;
        
        
        if (opcode.compare("L:") == 0) {
            //cout << "got an L: " << endl;
            ifstream inFile1;
            string filename1;
            string lineFromFile1;
            string val;
            s >> filename1;
            const char* filename = filename1.c_str();
            cout << "filename is " << filename << endl;
            inFile1.open(filename);
            if (!inFile1.good()) {
                cerr << "Error opening file " << filename << endl;
            } 
            while( std::getline(inFile1, lineFromFile1) ){
                stringstream s1(lineFromFile1);
                while( s1 >> val) {
                    bt.insert(atoi(val.c_str()));
                }
            }
        }
        else if (opcode.compare("A:") == 0) {
            //cout << "got an A: " << endl;
            string val;
            s >> val;
            bt.insert(atoi(val.c_str()));
        }
        else if (opcode.compare("D:") == 0) {
            //cout << "got an D: " << endl;
            string val;
            s >> val;
            bt.remove(atoi(val.c_str()));
        }
        else if (opcode.compare("S:") == 0) {
            //cout << "got an S: " << endl;
            string val;
            s >> val;
            if (bt.find(atoi(val.c_str()))) {
                cout << "the value, " << val << ", is in the tree" << endl;
            }
            else {
                cout << "the value, " << val << ", is not in the tree" << endl;
            }
        }
        else if (opcode.compare("P:") == 0) {
            //cout << "got an P: " << endl;
            bt.printByLevel();
        }
        else if (opcode.compare("T:") == 0) {
            //cout << "got an T: " << endl;
            cout << "terminating program..." << endl;
            return 0;
        }
        else {
            std::cout << "invalid command: ignoring and executing the next command" << endl;
        }
    }
    bt.printByLevel();
    
    return 0;
}


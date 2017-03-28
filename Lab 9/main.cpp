/**
Compiler Design Lab 9
Objective : To create a predictive top down parser (LL(1))
Name : Aditya Dev Sharma
Roll : UE143003
Email : aditya.sharma15696@gmail.com
Date : Mar 22, 2017
*/

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <map>
#include <unordered_map>
#include <iterator>
#include <algorithm>
#include <set>

#include "defines.hpp"

using namespace std;

int main() {
    cout << "Enter the productions of an LL(1) Grammar (enter \"X\" to stop)...\n";
    string line;
    int count = 0;
    unordered_map< string, unordered_map<string, int> > table;
    do {
        cout << "> ";
        cin >> line;
        std::string st(1, line[0]);
        cin.ignore();
        if (line != "X") {
            int index = line.find('>');
            string z = line.substr(0, index-1);
            lhs.push_back(std::make_pair(z, count));
            string right = line.substr(index+1);
            for (char s: right) {
                if ('A' <= s && s <= 'Z') {   // Non Terminal occuring on the right hand side
                    std::string st(1, s); 
                    rhs.push_back(std::make_pair(st, count));
                }
            }
            productions.push_back(line);
        }
        count++;
    }while (line != "X");

    //First
    for (auto p: lhs)
        First(p.first, p.second);   

    //Prints First Entries
    for (auto it = first.begin(); it != first.end(); it++) {
        cout << "First[" << it->first << "] = { ";
        auto v = it->second;
        for (auto val : v)
            cout << "(" <<val.first << "," << val.second << "), ";
        cout << "} \n";
        cout << "\n";
    }

    cout << "\n\n";

    //Follow
    std::string startSymbol(1, productions[0][0]);
    vector<pair<string, int> > v;
    v.push_back(std::make_pair("$", 0));
    follow[startSymbol] = v;
    for (auto p: rhs) {
        Follow(p.first, p.second);
    }
     
    
    //Prints Follow Entries
    for (auto it = follow.begin(); it != follow.end(); it++) {
        cout << "Follow[" << it->first << "] = { ";
        auto v = it->second;
        for (auto val : v)
            cout << "(" <<val.first << "," << val.second << "), ";
        cout << "} \n";
        cout << "\n";
    }
    

    // Creating table from first and follow sets
    for (auto nonTerminal: lhs) {
        string ss = nonTerminal.first;
        vector<pair<string, int> > vec = first[ss];
        unordered_map<string, int> row;
        int followIndex;
        for (auto v: vec) {
            if (v.first == "e") {
                followIndex = v.second;
                auto zec = follow[ss];
                for (auto symbol: zec) {
                    if (symbol.first != "e") {
                        row[symbol.first] = followIndex;
                    }
                }
                continue;
            }
            row[v.first] = v.second;
        }

        table[ss] = row;
    }

    
    // Table
    cout << "The parse table is...\n\n";
    for (auto key: table) {
        string nonTerminal = key.first;
        for (auto nextKey: key.second) {
            string terminal = nextKey.first;
            int index = nextKey.second;

            cout << "Table[" << nonTerminal << "][" << terminal << "] = " << productions[index] << "\n";
        }
    }

    
}

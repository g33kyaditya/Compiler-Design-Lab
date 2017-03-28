/**
Compiler Design Lab 8
Objective : To find the first and follow of the given productions 
Name : Aditya Dev Sharma
Roll : UE143003
Email : aditya.sharma15696@gmail.com
Date : Mar 15, 2017
*/

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <map>
#include <iterator>
#include <algorithm>

using namespace std;

map<string, vector<string> > first;
map<string, vector<string> > follow;
vector<string> productions;
vector<pair<string, int> > lhs;
vector<pair<string, int> > rhs;

bool isTerminal(char ch) {
    return ('a' <= ch && ch <= 'z');
}

void First(string s, int index) {
    string current = productions[index] + "|";
    int i = current.find('>');

    //Adding Epsilon Productions
    if (current.find('e') != string::npos) {
        vector<string> v = first[s];
        string temp = "e";
        if (std::find(v.begin(), v.end(), temp) == v.end())
            v.push_back(temp);
        first[s] = v;
    }

    int wall = 0;
    while (i != string::npos && i+1 < current.length() && wall < current.length()) {
        if (isTerminal(current[i+1])) {
            vector<string> v = first[s];
            string temp (1, current[i+1]);
            if (std::find(v.begin(), v.end(), temp) == v.end())
                v.push_back(temp);
            first[s] = v;
        }
        else {
            string nonTerminal(1, current[i+1]);
            int pos = 0;
            for (auto p: lhs) {
                if (p.first == nonTerminal) {
                    pos = p.second;
                    break;
                }
            }
            First(nonTerminal, pos);
            vector<string> v = first[nonTerminal];
            vector<string> currentV = first[s];
            for (string value: v) {
                if (std::find(currentV.begin(), currentV.end(), value) == currentV.end())
                    currentV.push_back(value);
            }
            first[s] = currentV;

            bool removed = false;
            // Adding the next symbol
            auto it = std::find(currentV.begin(), currentV.end(), "e");
            int legion = i+1;
            while (it != currentV.end()) {
                removed = true;
                legion++;
                currentV.erase(it);
                if (legion < current.length()) {
                    string next (1, current[legion]);
                    if (isTerminal(current[legion])) {
                        vector<string> v = first[s];
                        string temp (1, current[legion]);
                        if (std::find(v.begin(), v.end(), temp) == v.end())
                            v.push_back(temp);
                        first[s] = v;
                        currentV = v;
                    }
                    else {
                        string next (1, current[legion]);
                        int pos = 0;
                        for (auto p: lhs) {
                            if (p.first == next) {
                                pos = p.second;
                                break;
                            }
                        }
                        First(next, pos);
                        vector<string> v = first[next];
                        for (string value: v) {
                            if (std::find(currentV.begin(), currentV.end(), value) == currentV.end())
                                currentV.push_back(value);
                        }
                        first[s] = currentV;
                    }
                    it = std::find(currentV.begin(), currentV.end(), "e");
                }
                else
                    break;
            }
            if (removed) {
                currentV = first[s];
                currentV.push_back("e");
                first[s] = currentV;
            }
        }
        i = current.find('|', wall);
        wall = i+1;
    }
}

void Follow(string s, int index) {
    // All productions that have the Non Terminal s on the RHS
    vector<int> indices;
    for (auto p: rhs) {
        if (p.first == s) {
            indices.push_back(p.second);
        }
    }
    vector<string> filteredProductions;
    for (int index: indices) {
        for (int i = 0; i < productions.size(); i++) {
            if (index == i)
                filteredProductions.push_back(productions[i]);
        }
    }

    bool isEpsilon = false;
    for (string production: filteredProductions) {
        production += "|";
        for (int index = 0; index < production.length(); index++) {
            if (production.substr(index, 1) == s) {
                // Follow(s) = First(next symbol of s)
                if (production[index+1] == '|') {
                    string ss = production.substr(0, 1);
                    vector<string> current = follow[production.substr(index, 1)];
                    for (auto s: follow[ss]) {
                        if (std::find(current.begin(), current.end(), s) == current.end())
                            current.push_back(s);
                    }
                    follow[production.substr(index, 1)] = current;
                    if (std::find(current.begin(), current.end(), "e") != current.end())
                        isEpsilon = true;
                }
                else {
                    string next = production.substr(index+1, 1);
                    vector<string> current = follow[production.substr(index, 1)];
                    for (auto s: first[next]) {
                        if (std::find(current.begin(), current.end(), s) == current.end())
                            current.push_back(s);
                    }
                    follow[production.substr(index, 1)] =  current;

                    // Case when there are epsilon in the next's
                    auto it = std::find(current.begin(), current.end(), "e");
                    int z = 1;
                    while (it != current.end()) {
                        isEpsilon = true;
                        current.erase(it);
                        z++;
                        int nextOfNext = index + z;
                        if (nextOfNext >= production.length()) {
                            string ss = production.substr(0, 1);
                            vector<string> current = follow[production.substr(index, 1)];
                            for (auto s: first[ss]) {
                                if (std::find(current.begin(), current.end(), s) == current.end())
                                    current.push_back(s);
                            }
                            follow[production.substr(index, 1)] = current;
                        }
                        else {
                            string next = production.substr(nextOfNext, 1);
                            vector<string> c = follow[production.substr(index, 1)];
                            for (auto s: first[next]) {
                                if (std::find(current.begin(), current.end(), s) == current.end()) {
                                    c.push_back(s);
                                }
                            }
                            follow[production.substr(index, 1)] =  c;
                        }
                        it = std::find(current.begin(), current.end(), "e");
                    }
                }
                if (isEpsilon) {
                    vector<string> current = follow[production.substr(index, 1)];
                    if (std::find(current.begin(), current.end(), "e") == current.end())
                        current.push_back("e");
                    follow[production.substr(index, 1)] = current;
                }
            }
        }
    }

}

int main() {
    cout << "Enter the productions (enter \"X\" to stop)...\n";
    string line;
    int count = 0;

    do {
        cout << "> ";
        cin >> line;
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
            cout << val << ", ";
        cout << "} \n";
        cout << "\n";
    }

    cout << "\n\n";

    //Follow
    std::string startSymbol(1, productions[0][0]);
    vector<string> v;
    v.push_back("$");
    follow[startSymbol] = v;
    for (auto p: rhs) {
        Follow(p.first, p.second);
    }

    //Prints Follow Entries
    for (auto it = follow.begin(); it != follow.end(); it++) {
        cout << "Follow[" << it->first << "] = { ";
        auto v = it->second;
        for (auto val : v)
            cout << val << ", ";
        cout << "} \n";
        cout << "\n";
    }
}

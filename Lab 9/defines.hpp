#ifndef DEFINES_H
#define DEFINES_H

#include <vector>
#include <string>
#include <utility>
#include <map>
#include <iterator>
#include <algorithm>

using namespace std;

map<string, vector<pair<string, int > > > first;
map<string, vector<pair<string, int > > > follow;
vector<string> productions;
vector<pair<string, int> > lhs;
vector<pair<string, int> > rhs;

bool isTerminal(char);
void First(string, int);
//void Follow(string, int);

bool found(pair<string, int> temp, vector<pair<string, int > > arr) {
    for (auto p: arr) {
        if (p == temp)
            return true; 
    }
    return false;
}

bool found(string e, vector<pair<string, int> > arr) {
    for (auto a: arr) {
        if (a.first == e)
            return true;
    }
    return false;
}

void remove(vector<pair<string, int> >& vec, string e) {
    vector<pair<string, int > > result;
    for (auto p: vec) {
        if (p.first == e)
            continue;
        result.push_back(p);
    }
    vec.clear();
    vec = result;
}

void remove(vector<pair<string, int> >& vec, pair<string, int> e) {
    vector<pair<string, int > > result;
    for (auto p: vec) {
        if (p == e)
            continue;
        result.push_back(p);
    }
    vec.clear();
    vec = result;
}

bool isTerminal(char ch) {
    return ('a' <= ch && ch <= 'z');
}

void First(string s, int index) {
    string current = productions[index];
    int i = current.find('>');

    //Adding Epsilon Productions
    if (current.find('e') != string::npos) {
        vector<pair<string, int > > v = first[s];
        auto temp = std::make_pair("e", index);
        if (!found(temp, v))
            v.push_back(temp);
        first[s] = v;
    }

        if (isTerminal(current[i+1])) {
            vector<pair<string, int > > v = first[s];
            string tempStr (1, current[i+1]);
            pair<string, int > temp = std::make_pair(tempStr, index);
            if (!found(temp, v))
                v.push_back(temp);
            first[s] = v;
        }
        else {
            string nonTerminal(1, current[i+1]);
            int pos = 0;
            vector<int> allIndices;
            for (auto p: lhs) {
                if (p.first == nonTerminal) {
                    allIndices.push_back(p.second);
                }
            }
            for (auto someIndex: allIndices)
                First(nonTerminal, someIndex);

            vector<pair<string, int> > v = first[nonTerminal];
            auto copy = v;
            for (auto& s: copy)
                s.second = index;
            vector<pair<string, int> > currentV = first[s];
            for (auto value: copy) {
                if (!found(value, currentV))
                    currentV.push_back(value);
            }
            first[s] = currentV;

            bool removed = false;
            // Adding the next symbol
            pair<string, int> e = std::make_pair("e", index);
            bool it = found(e, currentV);
            int legion = i+1;
            while (it) {
                removed = true;
                legion++;
                remove(currentV, e);
                if (legion < current.length()) {
                    string next (1, current[legion]);
                    if (isTerminal(current[legion])) {
                        vector<pair<string, int> > v = first[s];
                        string tempStr(1, current[legion]);
                        pair<string, int> temp = std::make_pair(tempStr, index);
                        if (!found(temp, v))
                            v.push_back(temp);
                        first[s] = v;
                        currentV = v;
                    }
                    else {
                        string next (1, current[legion]);
                        int pos = 0;
                        vector<int> allIndices;
                        for (auto p: lhs) {
                            if (p.first == next) {
                                allIndices.push_back(p.second);
                            }
                        }
                        for (auto someIndex: allIndices)
                            First(next, someIndex);

                        vector<pair<string, int> > v = first[next];
                        for (auto value: v) {
                            if (!found(value, currentV))
                                currentV.push_back(value);
                        }
                        first[s] = currentV;
                    }
                    pair<string, int> e = std::make_pair("e", index);
                    it = found(e, currentV);
                }
                else
                    break;
            }
            if (removed) {
                currentV = first[s];
                pair<string, int> e = std::make_pair("e", index);
                currentV.push_back(e);
                first[s] = currentV;
            }
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
    
    pair<string, int> e = std::make_pair("e", index);

    for (auto i: indices) {
        string production = productions[i];
        for (int j = 3; j < production.length(); j++) {
            string st(1, production[j]);
            if (s == st) {
                if (j+1 >= production.length()) {       // If last symbol, so follow[fist symbol]
                    vector<pair<string, int> > current = follow[s]; 
                    auto newVec = follow[production.substr(0,1)];
                    for (auto val: newVec) {
                        current.push_back(val);
                    }
                    follow[s] = current;
                }
                else {
                    string next(1, production[j+1]);
                    auto nextFirst = first[next];

                    auto currentV = follow[s];
                    for (auto val: nextFirst) {
                        currentV.push_back(val);
                    }
                    follow[s] = currentV;

                    // If epsilon
                    bool isEpsilon = found("e", currentV);
                    int z = j+1;
                    while (isEpsilon) {
                        remove(currentV, "e");
                        z++;

                        if (z >= production.length()) {
                            auto newVec = follow[production.substr(0,1)];
                            for (auto val: newVec) {
                                currentV.push_back(val);
                            }
                            follow[s] = currentV;
                            break;
                        }
                        else {
                            string next(1, production[z]);
                            bool isEpsilon = false;
                            auto nextFirst = first[next];

                            for (auto val: nextFirst) {
                                currentV.push_back(val);
                            }
                            follow[s] = currentV;
                            isEpsilon = found("e",currentV);
                        }
                    }
                }
            }
        }
    }
}




#endif  //DEFINES_H

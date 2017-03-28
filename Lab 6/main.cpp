/**
Compiler Design Lab 6
Objective : Remove Left Recursion and Left Association from an input CFG
Name : Aditya Dev Sharma
Roll : UE143003
Email : aditya.sharma15696@gmail.com
Date : Feb 22, 2017
*/

#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

vector< vector< string > > tokensWithLeftRecurrence;
vector< vector< string > > tokensWithoutLeftRecurrence;
vector< vector< string > > finalProductions;
vector< string > ansProductions;
vector< string > startSymbol;
vector< string > prods;


//////////////////////////////////////// RECURRENCE ///////////////////////////////////////////////


void tokenize(string str, int index) {
    //str[1] = -
    //str[2] = >
    string cur = "";
    for (int i = 3; i < str.size(); i++) {
        if (str[i] == '|') {
            bool leftRecurrence = (cur.substr(0, 1) == startSymbol[index]) ? true : false;
            if (leftRecurrence)
                tokensWithLeftRecurrence[index].push_back(cur);
            else
                tokensWithoutLeftRecurrence[index].push_back(cur);
            cur = "";
            continue;
        }
        else
            cur += str[i];
    }

    if (!cur.empty()) {
        bool leftRecurrence = (cur.substr(0,1) == startSymbol[index]) ? true : false;
         if (leftRecurrence)
            tokensWithLeftRecurrence[index].push_back(cur);
         else
            tokensWithoutLeftRecurrence[index].push_back(cur);
    }
}

void removeLeftRecurrence(int index) {
    if (tokensWithLeftRecurrence[index].empty()) {
        // If the production doesn't involve any recursion, add it as it is to the final productions and return
        string production = startSymbol[index] + "->";
        for (string& str : tokensWithoutLeftRecurrence[index])
            production = production + str + "|";
        production.pop_back();
        finalProductions[index].push_back(production);
        return;
    }

    // Adding Beta Productions
    string toAdd = startSymbol[index] + "~";
    string production = "";
    production += startSymbol[index];
    production += "->";
    for (string& str : tokensWithoutLeftRecurrence[index])
        production = production + str + toAdd + "|";

    production.pop_back();
    finalProductions[index].push_back(production);

    //Adding Alpha productions
    production = toAdd;
    production += "->";

    for (string& str : tokensWithLeftRecurrence[index]) {
        str = str.substr(1);
        str += toAdd;
        str += "|";
        production += str;
    }
    production += "e";
    finalProductions[index].push_back(production);

}


//////////////////////////////////////// FACTORING ///////////////////////////////////////////////


string findLongest(string str) {
    int index = str.find('>');
    vector<string> vec;
    string cur = "";
    for (int i = index + 1; i < str.length(); i++) {
        if (str[i] == '|') {
            vec.push_back(cur);
            cur = "";
        }
        else
            cur += str[i];
    }

    if (!cur.empty())
        vec.push_back(cur);

    if (vec.size() < 2)
        return vec[0];

    string ret = "";
    cur = "";

    for (int i = 0; i < vec[0].size(); i++) {
        bool present = true;
        for (int j = 1; j < vec.size(); j++) {
            if (i >= vec[j].size() || vec[0][i] != vec[j][i]) {
                present = false;
                break;
            }
        }
        if (!present)
            break;
        else
            cur += vec[0][i];
    }
    return cur;
}

void removeLeftFactors(int index) {
    string longestCommon = "";
    string str = prods[index];
    if (str.back() == 'e')
        str.pop_back();
    longestCommon = findLongest(str);
    if (longestCommon.empty())
        ansProductions.push_back(str);
    else {
        // A->abcD|abcE     ====> A->abcA^
        int len = longestCommon.length();
        int indexOfSign = str.find('>');
        string start = str.substr(0, indexOfSign-1);
        string production = start;
        production += "->";
        production += longestCommon + start + "^";
        ansProductions.push_back(production);

        //A^->D|E
        str += "|";
        production = "";
        production += start + "^->";
        int pos = 0, pos1 = 0;
        pos = str.find(longestCommon, pos);
        pos1 = str.find('|', pos1);
        bool status = false;
        while (pos != string::npos && pos1 != string::npos) {
            string p = str.substr(len + pos, pos1- (pos + len) );
            if (!p.empty()) {
                production += p + "|";
                status = true;
            }
            pos++;
            pos1++;
            pos = str.find(longestCommon, pos);
            pos1 = str.find('|', pos1);
        }
        if (status)
            ansProductions.push_back(production);
    }
}

int main() {
    string input;
    vector<string> inputs;
    string line;
    cout << "Input the CFG (e means epsilon, enter \"X\" when done): \n";
    do {
        cout << "> ";
        cin >> line;
        cin.ignore();
        if (line != "X") {
            inputs.push_back(line);
            startSymbol.push_back(line.substr(0,1));
        }

    } while (line != "X");

    tokensWithLeftRecurrence.resize(inputs.size());
    tokensWithoutLeftRecurrence.resize(inputs.size());
    finalProductions.resize(inputs.size());

    int count = 0;
    for (string& input: inputs) {
        tokenize(input, count);
        count++;
    }

    for (int i = 0; i < inputs.size(); i++)
        removeLeftRecurrence(i);

    cout << "\nAfter removal of left recurrences, the productions are...\n\n";
    for (auto vec: finalProductions) {
        for (auto str: vec) {
            prods.push_back(str);
        }
    }

    for (int i = 0; i < prods.size(); i++)
        cout << prods[i] << "\n";

    for (int i = 0; i < prods.size(); i++)
        removeLeftFactors(i);

    cout << "\n\nAfter removal of left factoring, the productions are...\n";
    cout << "(NOTE: Left Factoring from the longest common string among ALL strings is calculated)\n\n";
    for (auto str: ansProductions)
        cout << str << "\n";

    return 0;
}

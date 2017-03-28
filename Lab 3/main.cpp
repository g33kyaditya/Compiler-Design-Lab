/**
Compiler Design Lab 3
Objective : Extend the program from Lab 2 to identify basic identifiers
Name : Aditya Dev Sharma
Roll : UE143003
Email : aditya.sharma15696@gmail.com
Date : Feb 01, 2017
*/

#include <iostream>
#include <vector>
#include <utility>
#include <set>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdio>
#include <string>

using namespace std;

vector<string> keywords;
bool state = false;
enum Visibility {
    Visible,
    Invisible
};

///////////////////////////////////////////////////////////////////////////////////////////////////

void generateKeywords() {
    ifstream infile("keywords.txt", ios::in);
    stringstream ss;
    ss << infile.rdbuf();
    string word;
    while (getline(ss, word, '\r')) {
        if (word.empty())
            continue;
        word = word.substr(1);
        keywords.push_back(word);
    }

    infile.close();
}

bool delimiter(char& ch) {
    if (ch == '{' || ch == '}' || ch == ' ' || ch == ';' || ch == '(' || ch == ')' || ch == '\n' || ch == '.'
       || ch == '<' || ch == '>' || ch == '\t' || ch == '\0' || ch == ':' || ch == '[' || ch == ']' || ch == '#'
       || ch == ',')
        return true;

    return false;
}

bool match(string& str) {
    if (std::find(keywords.begin(), keywords.end(), str) != keywords.end())
        return false;

    if (str == "=" || str == "==")
        return false;

    if (isdigit(str[0]))
        return false;
    // Identifier Rules
    // First char is an Alphabet
    // All other characters can be Alphabet or Number
    // Can be Underscore

    if (isalpha(str[0])) {
        if (str.length() == 1)
            return true;

        for (int i = 1; i < str.length(); i++) {
            if ( (!isalnum(str[i])) && (str[i] != '_') )
                return false;
        }
    }
    return true;
}

Visibility checkDoubleInvertedComma(char ch) {
    if (ch == '"' && state == false) {
        state = true;
        return Visibility::Invisible;
    }
    else if (ch == '"' && state == true) {
        state = false;
        return Visibility::Visible;
    }
    else if (state == true)
        return Visibility::Invisible;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    generateKeywords();
    ifstream infile("test.cpp", ios::in);

    char ch;
    string curr = "";
    int lineNumber = 1;
    vector<pair<int, string> > identifiers;

    while (infile.get(ch)) {
        if (ch == '\r')
            lineNumber++;

        Visibility v = checkDoubleInvertedComma(ch);
        if (v == Visibility::Invisible)
            continue;

        if (delimiter(ch)) {
            if (match(curr) && !curr.empty())
                identifiers.push_back(std::make_pair(lineNumber,curr));

            curr = "";
        }
        else {
            curr.push_back(ch);
        }

        if (infile.eof())
            break;
    }

    for (auto word : identifiers) {
        if (word.second == " " | word.second == "\r" || word.second == "\n" || word.second == "\"")
            continue;
        cout << "L" << word.first << "  " << word.second << "\n";
    }
    return 0;
}

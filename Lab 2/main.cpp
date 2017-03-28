/**
Compiler Design Lab 2
Objective : Identify Basic Keywords
Name : Aditya Dev Sharma
Roll : UE143003
Email : aditya.sharma15696@gmail.com
Date : Jan 25, 2017
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <utility>

using namespace std;

vector<string> keywords;
vector<std::pair<int, char> > operators;
bool state = false;

enum Visibility {
    Visible,
    Invisible
};

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
       || ch == ',' || ch == '+' || ch == '-' || ch == '/' || ch == '*')
        return true;

    return false;
}

bool isOperator(char ch) {
    return (ch == '+' || ch == '*' || ch == '/' || ch == '-');
}

bool match(string& str) {
    if (std::find(keywords.begin(), keywords.end(), str) != keywords.end())
        return true;
    return false;
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

int main()
{
    generateKeywords();

    ifstream infile("test.cpp", ios::in);
    char ch;
    int lineNumber = 1;
    string curr = "";
    vector<pair<int, string> > found;
    while (infile.get(ch)) {
        if (ch == '\r')
            lineNumber++;

        Visibility v = checkDoubleInvertedComma(ch);
        if (v == Visibility::Invisible)
            continue;

        if (delimiter(ch)) {
            if (match(curr) && !curr.empty())
                found.push_back(std::make_pair(lineNumber,curr));
            else if (isOperator(ch)) {
                operators.push_back(std::make_pair(lineNumber, ch));
            }
            curr = "";
        }
        else {
            curr.push_back(ch);
        }

        if (infile.eof())
            break;
    }

    for (auto word : found)
        cout << "L" << word.first << " " << word.second << "\n";

    cout << "\nThe operators are...\n";
    for (auto word : operators)
        cout << "L" << word.first << " : " << word.second << "\n";
    return 0;
}

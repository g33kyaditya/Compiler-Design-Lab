/**
Compiler Design Lab 1
Objective : Remove comments from a source file, play with file I/O
Name : Aditya Dev Sharma
Roll : UE143003
Email : aditya.sharma15696@gmail.com
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

void truncateSpaces(string& str) {
    //Truncates all unnecessary spaces from the line
    //Unnecessary spaces are the ones at the starting and at the end of the line

    int index = str.find_first_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ(){}[]/*+-#");
    if (index != string::npos)
        str = str.substr(index);
    index = str.find_last_of(";");
    if (index != string::npos)
        str = str.substr(0, index+1);

}

bool removeSingleComments(string& str) {
    int index = str.find("//");
    if (index != string::npos) {
        str = str.substr(0, index);
        return true;
    }

    return false;
}

bool removeMultipleComments(string& str, bool& commentStatus) {
    int index = str.find("/*");
    int index2 = str.find("*/");

    if (index != string::npos && index2 != string::npos) {
        str = str.substr(index2+2);
        return true;
    }

    if (index != string::npos) {
        commentStatus = true;
        str = str.substr(0, index);
        return true;
    }
    else if (index2 != string::npos) {
        commentStatus = false;
        str.clear();
        return true;
    }

    if (commentStatus) {
        str.clear();
        return true;
    }
}

int main()
{
    ifstream infile("test.cpp", ios::in);
    stringstream ss;
    ss << infile.rdbuf();
    string line;
    bool commentStatus = false;
    int lineNumber = 1;
    while (getline(ss, line)) {
        if (line.empty() || line == "\r")
            continue;

        truncateSpaces(line);

        removeSingleComments(line);
        removeMultipleComments(line, commentStatus);
        if (!line.empty()) {
            cout << lineNumber << " " << line << "\n";
            lineNumber++;
        }

    }

    infile.close();
    return 0;
}

/**
Compiler Design Lab 10
Objective : Create a basic Bottom Up Shift Reduce Parser
Name : Aditya Dev Sharma
Roll : UE143003
Date: Apr 5, 2017
Email : aditya.sharma15696@gmail.com
*/

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

unordered_map<string, string> m;

void reduce (string& str) {
    int last = str.length()-1;
    if (str == "$S")
        return;
    for (int i = 1; i <= last; i++) {
        if (str == "$S")
            return;
        string aString = str.substr(i);
        if (aString == "S" || aString.empty())
            return;
        if (m.find(aString) != m.end()) {
            str.erase(i);
            str += m[aString];
            reduce(str);
        }
    }
}
int main() {
    vector<string> productions;
    cout << "Enter the productions of the grammar (Press \'X\' to quit)\n";
    do {
        string line;
        cout << "> ";
        cin >> line;
        if (line == "X")
            break;
        productions.push_back(line);
        string lhs = line.substr(0, 1);
        string rhs = line.substr(3);
        m[rhs] = lhs;
    }while (1);
    string toCheck;
    cout << "Enter the string you want to check: ";
    cin >> toCheck;
    toCheck += "$";

    string st = "$";
    for (int i = 0; i < toCheck.length(); i++) {
        if (toCheck[i] == '$' && st.back() == 'S') {
            cout << "ACCEPTED\n";
            return 0;
        }
        st += toCheck[i];
        reduce(st);
    }
    cout << "NOT ACCEPTED\n";
    return 0;
}

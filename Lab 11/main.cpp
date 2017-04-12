/**
Compiler Design Lab 11
Objective : Create a Bottom Up Shift Reduce Parser
Name : Aditya Dev Sharma
Roll : UE143003
Date: Apr 12, 2017
Email : aditya.sharma15696@gmail.com
*/

#include <iostream>
#include <string>
#include <unordered_map>
#include <stack>
using namespace std;

unordered_map<string, unordered_map<string, string> > table;
void fillOperatorPrecedenceTableStatically() {
    unordered_map<string, string> row_0;
    row_0["+"] = ">";
    row_0["*"] = ">";
    row_0["$"] = ">";
    table["i"] = row_0;

    unordered_map<string, string> row_1;
    row_1["i"] = "<";
    row_1["+"] = ">";
    row_1["*"] = "<";
    row_1["$"] = ">";
    table["+"] = row_1;

    unordered_map<string, string> row_2;
    row_2["i"] = "<";
    row_2["+"] = ">";
    row_2["*"] = ">";
    row_2["$"] = ">";
    table["*"] = row_2;

    unordered_map<string, string> row_3;
    row_3["i"] = "<";
    row_3["+"] = "<";
    row_3["*"] = "<";
    row_3["$"] = ">";
    table["$"] = row_3;
}

string toString(char ch) {
    string ret(1, ch);
    return ret;
}

bool parse(string str) {
    str += "$";
    stack<string> st;
    st.push("$");
    int index = 0;
    string ip;
    while (1) {
        if (index >= str.length())
            return true;
        ip = str[index];
        cout << "Stack top = " << st.top() << " and tape symbol = " << ip << "\n";
        if (st.top() == "$" && ip == "$")
            return true;
        else {
            string a = st.top();
            string b = ip;

            if (table[a][b] == "<" || table[a][b] == "=") {
                cout << "Pushing " << b << " to stack\n";
                st.push(b);
                index++;
            }
            else if (table[a][b] == ">") {
                string c;
                do {
                    c = st.top();
                    cout << "Popping from stack\n";
                    st.pop();
                    if (st.top() == "$")
                        break;
                }while (table[c][st.top()] == ">");
            }
            else
                return false;
        }
    }
}

int main() {
    fillOperatorPrecedenceTableStatically();
    string toCheck;
    cout << "Enter the string you want to parse: ";
    cin >> toCheck;
    bool status = parse(toCheck);
    if (status)
        cout << "ACCEPTED\n";
    else
        cout << "NOT ACCEPTED\n";
    return 0;
}

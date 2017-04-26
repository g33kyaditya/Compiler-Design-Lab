/**
Compiler Design Lab 12
Objective : To implement an LR(0) parsing algorithm
Name : Aditya Dev Sharma
Roll : UE143003
Date: Apr 25, 2017
Email : aditya.sharma15696@gmail.com

Reference: http://digital.cs.usu.edu/~allan/Compilers/Notes/LRParsing.pdf
Grammar
0: P -> S$
1: S -> aSbS
2: S -> a
*/

#include <iostream>
#include <stack>
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
using namespace std;

unordered_map<string, unordered_map<string, string> > Action;
unordered_map<string, unordered_map<string, string> > Goto;
stack<string> st;

void fillActionTable() {
    unordered_map<string, string> buf;
    buf["a"] = "S2";
    Action["0"] = buf;
    buf.clear();

    buf["$"] = "acc";
    Action["1"] = buf;
    buf.clear();

    buf["a"] = "S2";
    buf["b"] = "R2";
    buf["$"] = "R2";
    Action["2"] = buf;
    buf.clear();

    buf["b"] = "S4";
    Action["3"] = buf;
    buf.clear();

    buf["a"] = "S2";
    Action["4"] = buf;
    buf.clear();

    buf["b"] = "R1";
    buf["$"] = "R1";
    Action["5"] = buf;
    buf.clear();
}

void fillGotoTable() {
    unordered_map<string, string> buf;
    buf["S"] = "1";
    Goto["0"] = buf;
    buf.clear();

    buf["S"] = "3";
    Goto["2"] = buf;
    buf.clear();

    buf["S"] = "5";
    Goto["4"] = buf;
    buf.clear();

}

string toString(char ch) {
    string ret = "";
    ret = ret + ch;
    return ret;
}

void printStack() {
    stack<string> temp = st;
    vector<string> vec;
    while (!temp.empty()) {
        vec.push_back(temp.top());
        temp.pop();
    }

    std:reverse(vec.begin(), vec.end());
    for (auto s: vec)
        cout << s;
}

int main() {
    fillActionTable();
    fillGotoTable();

    string str;
    cout << "Enter the string you want to parse: ";
    cin >> str;
    str = str + "$";
    st.push("0");
    int index = 0;
    cout << "\n\nStack \t\t\t Input \t\t\t Action \n\n";
    cout << "-------------------------------------------------------------------\n";
    while (1) {
        printStack();
        string pr = str.substr(index);
        cout << "\t\t" << pr;

        if (st.top() == "1" && str[index] == '$') {
            cout << "\t\t\t\t\t ACCEPTED \n";
            return 0;
        }

        string top = st.top();
        string tapeSymbol = toString(str[index]);
        if (Action.find(top) == Action.end()) {
            cout << "\t\t\t\t\t Error !";
            return 0;
        }
        auto row = Action[top];
        if (row.find(tapeSymbol) == row.end()) {
            cout << "\t\t\t\t\t Error !";
            return 0;
        }
        string currentAction = Action[top][tapeSymbol];
        if (currentAction[0] == 'S') {
            // Shift
            st.push(toString(str[index]));
            index++;
            st.push(toString(currentAction[1]));
            cout << "\t\t\t Shift " << currentAction[1] << "\n\n";
        }
        else {
            // Reduce
            int state = currentAction[1] - '0';
            int rhsLength;  // From grammar
            string lhs;
            switch(state) {
                case 0:
                    rhsLength = 2;
                    lhs = "P";
                    break;
                case 1:
                    rhsLength = 4;
                    lhs = "S";
                    break;
                case 2:
                    rhsLength = 1;
                    lhs = "S";
                    break;
                default:
                    break;
            }
            int numberOfTimesToPop = rhsLength * 2;
            while (numberOfTimesToPop--) {
                if (st.empty()) {
                    cout << " Error!";
                    return 0;
                }
                st.pop();
            }
            string curTop = st.top();
            string newState = Goto[curTop][lhs];
            st.push(lhs);
            st.push(newState);
            cout << "\t\t\t Reduce " << state << "\n\n";
        }
    }
}

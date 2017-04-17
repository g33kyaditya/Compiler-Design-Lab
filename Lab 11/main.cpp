/**
Compiler Design Lab 11
Objective : Create a Bottom Up Shift Reduce Parser
Name : Aditya Dev Sharma
Roll : UE143003
Date: Apr 12, 2017
Email : aditya.sharma15696@gmail.com
Reference: http://www.bytelabs.org/pub/ct/slides/rb@uoe/comp06.pdf
*/

#include <iostream>
#include <string>
#include <unordered_map>
#include <stack>
#include <set>
#include <vector>
using namespace std;

unordered_map<string, unordered_map<string, string> > table;
unordered_map<string, set<string> > first;
unordered_map<string, set<string> > last;

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

bool isNonTerminal(char ch) {
    return ('A' <= ch && ch <= 'Z');
}

void addToFirst(string symbol) {
    for (auto st: first[symbol]) {
        addToFirst(st);
        auto cur = first[st];
        for (auto unit: cur)
            first[symbol].insert(unit);
    }
}

void addToLast(string symbol) {
    for (auto st: last[symbol]) {
        addToFirst(st);
        auto cur = last[st];
        for (auto unit: cur)
            last[symbol].insert(unit);
    }
}

void createFirstAndLastTable(vector<string>& productions) {
    // FirstOp - I
    for (auto line: productions) {
        string ss = toString(line[0]);
        if (isNonTerminal(line[3])) {
            bool added = false;
            int index = 3;
            while (!added && index < line.length()) {
                set<string> currentSet = first[ss];
                if (ss != toString(line[index]))
                    currentSet.insert(toString(line[index]));
                first[ss] = currentSet;
                if (!isNonTerminal(line[index]))
                    added = true;
                index++;
            }
        }
        else {
            set<string> currentSet = first[ss];
            if (ss != toString(line[3]))
                currentSet.insert(toString(line[3]));
            first[ss] = currentSet;
        }
    }

    // LastOp - I
     for (auto line: productions) {
        string ss = toString(line[0]);
        if (isNonTerminal(line.back())) {
            bool added = false;
            int index = line.length()-1;
            while (!added && index >= 3) {
                set<string> currentSet = last[ss];
                if (ss != toString(line[index]))
                    currentSet.insert(toString(line[index]));
                last[ss] = currentSet;
                if (!isNonTerminal(line[index]))
                    added = true;
                index--;
            }
        }
        else {
            set<string> currentSet = last[ss];
            if (ss != toString(line.back()))
                currentSet.insert(toString(line.back()));
            last[ss] = currentSet;
        }
    }

    // Now we need to refine the first and last sets we created earlier
    for (auto elem: first) {
        string symbol = elem.first;
        addToFirst(symbol);
    }

    for (auto elem: last) {
        string symbol = elem.first;
        addToLast(symbol);
    }

    // Now we remove the non terminals from the operator precedence table
    unordered_map<string, set<string> > temp;
    for (auto unit: first) {
        for (string something: unit.second) {
            if (!isNonTerminal(something[0]))
                temp[unit.first].insert(something);
        }
    }
    first = temp;
    temp.clear();

    for (auto unit: last) {
        for (string something: unit.second) {
            if (!isNonTerminal(something[0]))
                temp[unit.first].insert(something);
        }
    }
    last = temp;
    temp.clear();

    // Now we remove keys with empty values
    for (auto unit: first) {
        if (!unit.second.empty()) {
            temp[unit.first] = unit.second;
        }
    }
    first.clear();
    first = temp;

    for (auto unit: last) {
        if (!unit.second.empty()) {
            temp[unit.first] = unit.second;
        }
    }
    last.clear();
    last = temp;
}

void createPrecedenceTable(vector<string>& productions) {
    // <
    for (string production: productions) {
        for (int i = 3; i < production.length()-1; i++) {
            if (!isNonTerminal(production[i]) && isNonTerminal(production[i+1])) {
                set<string> firstList = first[toString(production[i+1])];
                for (string elem: firstList)
                    table[toString(production[i])][elem] = "<";
            }
        }
    }

    // >
    for (string production: productions) {
        for (int i = 3; i < production.length()-1; i++) {
            if (isNonTerminal(production[i]) && !isNonTerminal(production[i+1])) {
                set<string> lastList = last[toString(production[i])];
                for (string elem: lastList)
                    table[toString(production[i+1])][elem] = ">";
            }
        }
    }

    // =
    for (string production: productions) {
        for (int i = 3; i < production.length()-2; i++) {
            if ( !isNonTerminal(production[i]) && !isNonTerminal(production[i+1]) )
                table[toString(production[i])][toString(production[i+1])] = "=";
            else if (!isNonTerminal(production[i]) && isNonTerminal(production[i+1]) && !isNonTerminal(production[i+2]) )
                table[toString(production[i])][toString(production[i+2])] = "=";
        }
    }

    // Adding $ < a for all a in first and a > $ 
    for (auto unit: first) {
        for (string val: unit.second) {
            table["$"][val] = "<";
        }
    }

    for (auto unit: last) {
        for (string val: unit.second) {
            table[val]["$"] = ">";
        }
    }
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
    //fillOperatorPrecedenceTableStatically();
    vector<string> productions;
    cout << "Enter productions of the operator grammar (press \"X\" to stop)\n";
    string str;
    while (1) {
        cin >> str;
        if (str == "X")
            break;
        productions.push_back(str);
    }
    createFirstAndLastTable(productions);
    createPrecedenceTable(productions);

    /*
    // Print FirstOp+ and LastOp+ Lists//
    cout << "\n\n"; 
    for (auto str: first) {
        auto s = str.second;
        cout << str.first << " : ";
        for (auto p: s)
            cout << p << ", ";
        cout << "\n";
    }

    cout << "\n\n"; 
    for (auto str: last) {
        auto s = str.second;
        cout << str.first << " : ";
        for (auto p: s)
            cout << p << ", ";
        cout << "\n";
    }
    //////////
    */

    // Print Precedence Table //

    for (auto k1: table) {
        for (auto k2: k1.second) {
            cout << "table["<<k1.first << "][" << k2.first << "] = " << k2.second << "\n";
        }
    }

    //////////////////////////

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

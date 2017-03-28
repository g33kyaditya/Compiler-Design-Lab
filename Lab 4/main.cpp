/**
Compiler Design Lab 3
Objective : Create a Finite Automata Table for input Regular Expression
Name : Aditya Dev Sharma
Roll : UE143003
Email : aditya.sharma15696@gmail.com
Date : Feb 08, 2017
Reference: https://en.wikipedia.org/wiki/Thompson's_construction
*/

#include <iostream>
#include <stack>
#include <vector>
#include <fstream>

using namespace std;

struct nfaStructure {
    vector<int> a;
    vector<int> b;
    vector<int> epsilon;
};

vector<nfaStructure> nfa;

nfaStructure initialState;
int size = 0;

stack<int> st;

//////////////////////////// INPUT PROCESSING //////////////////////////////////

int priority(char ch) {
    if (ch == '*')
        return 3;
    else if (ch == '.')
        return 2;
    else if (ch == '|')
        return 1;
    else
        return 0;
}

string convertToPostfix(string& str) {
    stack<char> st;
    string postfix = "";

    for (int i = 0; i < str.length(); i++) {
        if (isalpha(str[i]) )
            postfix += str[i];
        else if (str[i] == '(')
            st.push(str[i]);
        else if (str[i] == ')') {
            while (!st.empty() && st.top() != '(') {
                if (st.top() != '(')
                    postfix += st.top();
                st.pop();
            }
            if (!st.empty())
                st.pop();
        }
        else {
            while (!st.empty()) {
                char top = st.top();
                if (priority(top) > priority(str[i]) ) {
                    postfix += top;
                    st.pop();
                }
                else
                    break;
            }
            st.push(str[i]);
        }
    }

    while (!st.empty()) {
        postfix += st.top();
        st.pop();
    }

    return postfix;
}

///////////////////////////////////////////////////////////////////////////////
void checkStack() {
    if (st.empty()) {
        cout << "Invalid Expression. ABORTING !!!\n";
        exit(1);
    }
}

void processChar(int index) {
    nfa.push_back(initialState);
    nfa.push_back(initialState);
    if (index == 0)
        nfa[size].a.push_back(size + 1);
    else
        nfa[size].b.push_back(size + 1);

    st.push(size);
    size++;
    st.push(size);
    size++;
}

void processStar() {
    nfa.push_back(initialState);
    nfa.push_back(initialState);

    checkStack();
    int top = st.top();
    st.pop();
    checkStack();
    int secondTop = st.top();
    st.pop();

    nfa[size].epsilon.push_back(secondTop);
    nfa[size].epsilon.push_back(size + 1);

    nfa[top].epsilon.push_back(secondTop);
    nfa[top].epsilon.push_back(size + 1);

    st.push(size);
    size++;
    st.push(size);
    size++;
}

void processUnion() {
    nfa.push_back(initialState);
    nfa.push_back(initialState);
    
    checkStack();
    int fourth = st.top();
    st.pop();
    checkStack();
    int third = st.top();
    st.pop();
    checkStack();
    int second = st.top();
    st.pop();
    checkStack();
    int first = st.top();
    st.pop();

    nfa[size].epsilon.push_back(first);
    nfa[size].epsilon.push_back(third);
    nfa[second].epsilon.push_back(size + 1);
    nfa[fourth].epsilon.push_back(size + 1);

    st.push(size);
    size++;
    st.push(size);
    size++;
}

void processConcatenation() {
    checkStack();
    int fourth = st.top();
    st.pop();
    checkStack();
    int third = st.top();
    st.pop();
    checkStack();
    int second = st.top();
    st.pop();
    checkStack();
    int first = st.top();
    st.pop();
    nfa[second].epsilon.push_back(third);
    st.push(first);
    st.push(fourth);
}

void constructNFA(string expression) {
    for (auto ch : expression) {
        switch(ch) {
            case 'a':
                processChar(ch - 'a');
                break;
            case 'b':
                processChar(ch - 'a');
                break;
            case '*':
                processStar();
                break;
            case '+':
                processUnion();
                break;
            case '.':
                processConcatenation();
                break;
        }
    }
}

void displayNFA()
{
    cout << "\n";
    cout << "-----------------------------------------------------------------------------------------------------------------\n";
    cout << "State\t\t|\t\tA\t\t|\t\tB\t\t|\t\tEpsilon\t\t|"<< endl;
    cout << "-----------------------------------------------------------------------------------------------------------------\n";
    for(int i = 0; i < nfa.size(); i++) {
        cout << i << "\t\t|\t\t";

        for(int j = 0; j < nfa[i].a.size(); j++)
            cout<< nfa[i].a[j] << ", ";
        cout << "\t\t|\t\t";

        for(int j = 0; j < nfa[i].b.size(); j++)
            cout<< nfa[i].b[j] << ", ";
        cout << "\t\t|\t\t";

        for(int j = 0; j < nfa[i].epsilon.size(); j++)
            cout << nfa[i].epsilon[j] << ", ";

        cout<< "\t\t|\n";
    }
    cout << "-----------------------------------------------------------------------------------------------------------------\n";
}

/////////////////////////// CREATE GRAPH ///////////////////////////////////

void saveToFile(int finalState) {
    ofstream outfile;
    outfile.open("dotFile");
    outfile << "digraph G {" << "\n";
    outfile << "node [shape = doublecircle];" << finalState << "\n";
    outfile << "node [shape = circle];" << "\n";
    for(int i = 0; i < nfa.size(); i++) {

        for(int j = 0; j < nfa[i].a.size(); j++) 
            outfile << i << "->" << nfa[i].a[j] << " [label=a];\n";


        for(int j = 0; j < nfa[i].b.size(); j++)
            outfile << i << "->" << nfa[i].b[j] << "[label=b];\n";

        for(int j = 0; j < nfa[i].epsilon.size(); j++)
            outfile << i << "->" << nfa[i].epsilon[j] << "[label=eps];\n";
    }
    outfile << "}";
    outfile.close();
}

//////////////////////////////////////////////////////////////////////////


int main() {
    string str;
    cout << "Input the Regular Expression: ";
    cin >> str;

    //Parse input to PostFix
    string postfix = convertToPostfix(str);
    //Post Fix
    cout << "PostFix: " << postfix << "\n";

    constructNFA(postfix);
    int finalState = st.top();
    displayNFA();
    saveToFile(finalState);
}

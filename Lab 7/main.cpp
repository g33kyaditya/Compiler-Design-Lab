/**
Compiler Design Lab 7
Objective : To create a trivial recursive descent parser with backtracking 
Name : Aditya Dev Sharma
Roll : UE143003
Email : aditya.sharma15696@gmail.com
Date : Mar 01, 2017
*/

#include <iostream>
#include <string>
using namespace std;
/*
    String := Minus Legion | Minus Lenny
    Legion := "a"
    Lenny := "b"
    Minus := "-"
*/
int index = 0;
bool Legion(char ch) {
    if (ch == 'a') {
        index++;
        return true;
    }
    return false;
}

bool Lenny(char ch) {
    if (ch == 'b') {
        index++;
        return true;
    }
    return false;
}

bool Minus(char ch) {
    if (ch == '-') {
        index++;
        return true;
    }
    return false;
}

bool String(string s) {
    if (Minus(s[index])) {
        if ( Legion(s[index]) || Lenny(s[index]) ) {
            if (index >= s.length())
                return true;
        }
        return false;
    }
    else
        return false;
}
int main()
{
    string str;
    cout << "Enter the string you want to check: ";
    cin >> str;

    if ( String(str) )
        cout << "Accepted\n";
    else
        cout << "Not Accepted\n";
}

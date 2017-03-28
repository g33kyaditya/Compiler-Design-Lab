%{
#include <stdio.h>
int total = 0;
int coutString = 0;
%}
SINGLE_LINE_COMMENT \/\/*
MULTI_LINE_COMMENT \*([^*]|[\r\n]|(\*+([^*\/]|[\r\n])))*\*+
KEYWORD auto|double|int|struct|break|else|long|using|namespace|switch|case|enum|register|typdef|char|extern|return|union|continue|signed|void|do|static|default|goto|volatile|const|float|short|unsigned|if|for|while|sizeof
EXTRAS iostream|cmath|string|main|include|cin|return|define|std|#define|#include|#ifdef|#ifndef|\(|\)|\ +|\{|\}|,|\\n|\"
EXTRACTION_OPERATOR <<
OPERATOR [+<>=\*\-%]
IDENTIFIER [a-zA-Z]+[a-zA-Z_0-9]*
DIGIT [0-9]
COUT     cout
SEMICOLON ;

%%
{SINGLE_LINE_COMMENT} {
    total++;
    printf("COMMENT\n");
}

{MULTI_LINE_COMMENT} {
    total++;
    printf("COMMENT\n");
}
{EXTRAS} {
}

"\n" {
}
{SEMICOLON} {
    if (coutString)
        coutString = 0;
}

{COUT} {
    coutString = 1;
}

{EXTRACTION_OPERATOR} {
}

{KEYWORD} {
    if (!coutString) {
        printf("Keyword %s\n ", yytext);
    }
}

{IDENTIFIER} {
  if (!coutString) {
        printf("Identifier %s\n ", yytext);
    }
}

{DIGIT} {
    printf("Digit %s\n ", yytext);
}

{OPERATOR} {
    printf("Operator %s\n ", yytext);
}

%%

int main()
{
  yylex();
  printf("Total number of comments = %d\n", total);
  return 0;
}

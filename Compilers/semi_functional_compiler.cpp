//  main.cpp
//  syntax_analyzer_5th_iteration
//
//  Created by Eugene Lee on 5/1/19.
//  Copyright © 2019 Eugene Lee. All rights reserved.

#include <iostream>
#include <map>
#include <stack>
#include <fstream>
#include <vector>

using namespace std;

//1: E->TE'
//2: Q->+TE'
//3: Q->-TE'
//4: Epsilon case
//5: T->FT'
//6: R->*FT'
//7: R->/FT'

enum Symbols{
    //Terminal Symbols
    L_PARENS,
    R_PARENS,
    PLUS,
    MINUS,
    TIMES,
    DIVIDE,
    EOS,
    INVALID,
    A,B,C,D,E,F,G,N,X,Z,

    //Non-terminal symbols
    NTS_E,NTS_Q,NTS_T,NTS_R

};

Symbols lexer(char c){
    switch(c){
        case '(': return L_PARENS;
        case ')': return R_PARENS;
        case '+': return PLUS;
        case '-': return MINUS;
        case '*': return TIMES;
        case '/': return DIVIDE;
        case '\0': return EOS;
        case 'a': return A;
        case 'b': return B;
        case 'c': return C;
        case 'd': return D;
        case 'e': return E;
        case 'f': return F;
        case 'g': return G;
        case 'n': return N;
        case 'x': return X;
        case 'z': return Z;
        default: return INVALID;
    }
}


int main(int argc, const char * argv[]) {

    char c_read;
    vector<char>lexemes;
    ifstream c("/Users/eugenelee/Documents/School Stuff/CPSC 323/test.txt",ifstream::in);

    while(c.get(c_read)){
        lexemes.push_back(c_read);
    }
    c.close();

    for (vector<char>::const_iterator it = lexemes.begin(); it != lexemes.end(); ++it)
    {
        if (*it == '!') {
            do
            {
                vector<char>::const_iterator temp = it;
                //*it++;
                lexemes.erase(temp);
            } while (*it != '!');
            it = lexemes.erase(it);
        }
    }

    map<Symbols, map<Symbols,int>> table;
    stack<Symbols> mystack;
    char *buffer;

    mystack.push(EOS);  //this is the end of the stack
    mystack.push(E);    //Non-terminal E


    //Parsing table

    table[NTS_E][A]=1;          //TE'
    table[NTS_E][B]=1;          //TE'
    table[NTS_E][C]=1;          //TE'
    table[NTS_E][D]=1;          //TE'
    table[NTS_E][E]=1;          //TE'
    table[NTS_E][F]=1;          //TE'
    table[NTS_E][G]=1;          //TE'
    table[NTS_E][N]=1;          //TE'
    table[NTS_E][X]=1;          //TE'
    table[NTS_E][Z]=1;          //TE'
    table[NTS_E][L_PARENS]=1;   //TE'
    table[NTS_Q][PLUS]=2;       //+TE'
    table[NTS_Q][MINUS]=3;      //-TE'
    table[NTS_Q][L_PARENS]=4;   //Epsilon
    table[NTS_Q][EOS]=4;        //Epsilon
    table[NTS_T][A]=5;          //FT'
    table[NTS_T][B]=5;          //FT'
    table[NTS_T][C]=5;          //FT'
    table[NTS_T][D]=5;          //FT'
    table[NTS_T][E]=5;          //FT'
    table[NTS_T][F]=5;          //FT'
    table[NTS_T][G]=5;          //FT'
    table[NTS_T][N]=5;          //FT'
    table[NTS_T][X]=5;          //FT'
    table[NTS_T][Z]=5;          //FT'
    table[NTS_T][L_PARENS]=5;   //FT'
    table[NTS_R][PLUS]=4;       //Epsilon
    table[NTS_R][MINUS]=4;      //Epsilon
    table[NTS_R][TIMES]=6;      //*FT'
    table[NTS_R][DIVIDE]=7;     ///FT'
    table[NTS_R][R_PARENS]=4;   //Epsilon
    table[NTS_R][EOS]=4;        //Epsilon

    while(mystack.size()>0){
        //Initialize buffer
        buffer=&lexemes[0];
        if(lexer(*buffer)==mystack.top()){
            cout<<"Matched symbols: "<<lexer(*buffer)<<endl;
            buffer++;
            mystack.pop();
        }
        else{
            cout<<"Rule "<<table[mystack.top()][lexer(*buffer)]<<endl;
            switch(table[mystack.top()][lexer(*buffer)]){
                case 1://  E  -> TE'
                    mystack.pop();
                    mystack.push(NTS_Q);
                    break;
                case 2://  E' -> +TE'
                    mystack.pop();
                    mystack.push(PLUS);
                    break;
                case 3://E' -> -TE'
                    mystack.pop();
                    mystack.push(MINUS);
                    break;
                case 4://E' -> ε || T' -> ε
                    mystack.pop();
                    break;
                case 5://T ->FT'
                    mystack.pop();
                    mystack.push(NTS_R);
                    break;
                case 6: //T' -> *FT'
                    mystack.pop();
                    mystack.push(TIMES);
                    break;
                case 7: //T' -> /FT'
                    mystack.pop();
                    mystack.push(DIVIDE);
                    break;
                default:
                    cout<<"Syntax Error\n";
                    break;
            }
        }
    }
    cout<<"Successfully compiled\n";
    return 0;
}

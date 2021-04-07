#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

string tokenLido;

string analisadorLexico(){
    int s = 0;
    char c;
    string lexema;
    while(s != 3 && s != 6 && s != 8 && s != 11 && s != 18){
        if(!cin.eof()){
            lexema = "";
            cin.get(c);
            switch(s){
                case 0:
                    if(c >= 65 && c <= 90 || c == '_' || c >= 97 && c <= 122){ //letras ou _
                        s = 1;
                        lexema += c;
                    }
                    else{
                        if(c == ':'){
                            s = 9;
                            lexema += c;
                        }
                        else{
                            if(c == '<'){
                                s = 4;
                                lexema += c;
                            }
                            else{
                                if(c == '>'){
                                    s = 10;
                                    lexema += c;
                                }
                                else{
                                    if(c == '/'){
                                        s = 2;
                                        lexema += c;
                                    }
                                    else{
                                        if(c >= 49 && c <= 57){ //1-9
                                            s = 13;
                                            lexema += c;
                                        }
                                        else{
                                            if(c == '0'){
                                                s = 12;
                                                lexema += c;
                                            }
                                            else{
                                                if(c == '-' || c == '+' || c == '*' || c == '%' || c == '=' || c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' || c == ';'){
                                                    s = 8;
                                                    lexema += c;
                                                }
                                                else{
                                                    if(c == '\''){
                                                        s = 19;
                                                        lexema += c;
                                                    }
                                                    else{
                                                        if(!(c == ' ' || c == '\n')){
                                                            //erro
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    break;
                case 1:
                    if((c >= 65 && c <= 90) || c == '_' || (c >= 48 && c <= 57) || c >= 97 && c <= 122){ //letra, digito ou _
                        s = 1;
                        lexema += c;
                    }
                    else{
                        s = 6;
                        cin.unget();
                    }
                    break;
                case 2:
                    if(c == '*'){
                        s = 5;
                    }
                    else{
                        s = 6;
                        cin.unget();
                    }
                    break;
                case 4:
                    if(c == '>' || c == '='){
                        s = 11;
                        lexema += c;
                    }
                    else{
                        s = 6;
                        cin.unget();
                    }
                    break;
                case 5:
                    if(c == '*'){
                        s = 7;
                    }
                    break;
                case 7:
                    if(c == '/'){
                        s = 0;
                        lexema = "";
                    }
                    else{
                        s = 5;
                    }
                    break;
                case 9:
                    if(c == '='){
                        s = 3;
                        lexema += c;
                    }
                    else{
                        s = 6;
                        cin.unget();
                    }
                    break;
                case 10:
                    if(c == '='){
                        s = 11;
                        lexema += c;
                    }
                    else{
                        s = 6;
                        cin.unget();
                    }
                    break;
                case 12:

                    if(c >= 48 && c <= 57){
                        s = 16;
                        lexema += c;
                    }
                    else{
                        if(c >= 65 && c <= 70){
                            s = 14;
                            lexema += c;
                        }
                        else{
                            s = 6;
                            cin.unget();
                        }
                    }
                    break;
                case 13:
                    if(c >= 48 && c <= 57){
                        s = 13;
                        lexema += c;
                    }
                    else{
                        s = 6;
                        cin.unget();
                    }
                    break;
                case 14:
                    if((c >= 48 && c <= 57) || (c >= 65 && c <= 70)){
                        s = 15;
                        lexema += c;
                    }
                    else{
                        //erro
                    }
                    break;
                case 15:
                    if(c == 'h'){
                        s = 18;
                        lexema += c;
                    }
                    else{
                        //erro
                    }
                    break;
                case 16:
                    if((c >= 48 && c <= 57) || (c >= 65 && c <= 70)){
                        s = 17;
                        lexema += c;
                    }
                    else{
                        s = 6;
                        cin.unget();
                    }
                    break;
                case 17:
                    if(c >= 48 && c <= 57){
                        s = 13;
                        lexema += c;
                    }
                    else{
                        if(c == 'h'){
                            s = 18;
                            lexema += c;
                        }
                        else{
                            s = 6;
                            cin.unget();
                        }
                    }
                    break;
                case 19:
                    s = 20;
                    lexema += c;
                    break;
                case 20:
                    if(c == '\''){
                        s = 8;
                        lexema += c;
                    }
                    else{
                        //erro?
                    }
            }
        }
        else{
            s = 8;
            lexema = "";
        }
    }
    return lexema;
}


/* Grámatica
*
* S-> {D} main '{'{C}'}' eof
*
* Declaracoes
* D-> T L | K
*
* T-> char | string | int | boolean
* L-> idM
* M->  = valor | '['tam']'
* K-> final id = valor
*
* Comandos
* C-> id['['E']'] := E;
* C-> for'('A;E;A')' B
* C-> if'('E')' then B [else B]
* C-> ;
* C-> readln'('V')';
* C-> write'('{E}')'; | writeln'('{E}')';
* 
* A-> [C{,C}]
* B-> (C; | '{'{C;}'}')
* V-> id['['E']']
* 
* Expressoes
* E-> F [ (=|<>|<|>|<=|>=) F]
* F-> [+|-] G {(+|-|or) G} 
* G-> H {(*|and|/|%) H}
* H-> id ['['E']'] | const | not H | '('E')'
*/

void analisadorSintatico(){
    while(!cin.eof()){
        tokenLido = analisadorLexico();
        if(tokenLido == null){
            tokenLido = ""; //EOF
        }
        
    }
}

int main(){
    analisadorSintatico();
    return 0;
}

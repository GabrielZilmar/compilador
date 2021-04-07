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
* D->  L | K
*
* T-> char | string | int | boolean
* L-> idM{,idM}
* M-> [= const] | '['const']'
* K-> final id = const
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
* B-> C | '{'{C}'}'
* V-> id['['E']']
* 
* Expressoes
* E-> F [ (=|<>|<|>|<=|>=) F]
* F-> [+|-] G {(+|-|or) G} 
* G-> H {(*|and|/|%) H}
* H-> id ['['E']'] | const | not H | '('E')'
*/

void analisadorSintatico(){
    tokenLido = analisadorLexico();
    if(tokenLido == null){
        tokenLido = ""; //EOF
    }
    procedimentoS();
}

void casaToken(string tokenEsp){
    string token = null;
    if(tokenEsp == tokenLido){
        token = tokenLido;
        tokenLido = analisadorLexico();
    }
    else{
        //erro
    }
}

void procedimentoS(){
    while(tokenLido == "char" || tokenLido == "int" || tokenLido == "string" || tokenLido == "boolean" || tokenLido == "final"){
        procedimentoD();
    }
    casaToken('main');
    casaToken('{');
    while(tokenLido == "id" || tokenLido == "for" || tokenLido == "if" || tokenLido == ";" || tokenLido == "readln" || tokenLido == "write" || tokenLido == "writeln"){
        ProcedimentoC();
    }
    casaToken('}');

}

//Declaracoes
void procedimentoD(){
    if(tokenLido == "char" || tokenLido == "int" || tokenLido == "string" || tokenLido == "boolean"){
        procedimentoT();
        procedimentoL();
    }
    else{
        if(tokenLido == "final"){
            procedimentoK();
        }
        else{
            //erro
        }
    }
}
void procedimentoT(){
    if(tokenLido == "char"){
            casaToken("char");
        }
        else{
            if(tokenLido == "int"){
                casaToken("int");
            }
            else{
                if(tokenLido == "string"){
                    casaToken("string");
                }
                else{
                    if(tokenLido == "boolean"){
                        casaToken("boolean");
                    }
                    else{
                        //erro
                    }
                }
            }
        }
    }
}

void procedimentoL(){
    casaToken("id");
    procedimentoM();
    while(tokenLido == ","){
        casaToken(",");
        casaToken("id");
        procedimentoM();
    }
}

void procedimentoM(){
    if(tokenLido == "["){
        casaToken("[");
        casaToken("const");
        casaToken("]");
    }
    else{
        if(tokenLido == "="){
            casaToken("=");
            casaToken("const");
        }
    }
}

void procedimentoK(){
    casaToken("final");
    casaToken("id");
    casaToken("=");
    casaToken("const");
}

//Comandos
void procedimentoC(){
    if(tokenLido == "id"){
        casaToken("id");
        if(tokenLido == "["){
            casaToken("[");
            procedimentoE();
            casaToken("]");
        }
        casaToken(":=");
        procedimentoE();
    }
    else{
        if(tokenLido == "for"){
            casaToken("for");
            casaToken("(");
            procedimentoA();
            casaToken(";");
            procedimentoE();
            casaToken(";");
            procedimentoA();
            casaToken(")");
            procedimentoB();
        }
        else{
            if(tokenLido == "if"){
                casaToken("if");
                casaToken("(");
                procedimentoE();
                casaToken(")");
                casaToken("then");
                procedimentoB();
                if(tokenLido == "else"){
                    casaToken("else");
                    procedimentoB();
                }
            }
            else{
                if(tokenLido == "readln"){
                    casaToken("readln");
                    casaToken("(");
                    procedimentoV();
                    casaToken(")");
                    casaToken(";");
                }
                else{
                    if(tokenLido == "write"){
                        casaToken("write");
                        casaToken("(");
                        procedimentoE();
                        casaToken(")");
                        casaToken(";");
                    }
                    else{
                        if(tokenLido == "writeln"){
                            casaToken("writeln");
                            casaToken("(");
                            procedimentoE();
                            casaToken(")");
                            casaToken(";");
                        }
                        else{
                            if(tokenLido == ";"){
                                casaToke(";");
                            }
                            else{
                                //erro
                            }
                        }
                    }
                }
            }
        }
    }
}

void procedimentoA(){
    if(tokenLido == "id" || tokenLido == "for" || tokenLido == "if" || tokenLido == ";" || tokenLido == "readln" || tokenLido == "write" || tokenLido == "writeln"){
        procedimentoC();
        while(tokenLido == ","){
            casaToken(",");
            procedimentoC();
        }
    }
}

void procedimentoB(){
    if(tokenLido == "{"){
        casaToken("{");
        while(tokenLido == "id" || tokenLido == "for" || tokenLido == "if" || tokenLido == ";" || tokenLido == "readln" || tokenLido == "write" || tokenLido == "writeln"){
            procedimentoC();
        }
        casaToken("}");
    }
    else{
        if(tokenLido == "id" || tokenLido == "for" || tokenLido == "if" || tokenLido == ";" || tokenLido == "readln" || tokenLido == "write" || tokenLido == "writeln"){
            procedimentoC();
        }
        else{
            //erro
        }
    }
}

procedimentoV(){
    casaToken("id");
    if(tokenLido == "["){
        casaToken("[");
        procedimentoE();
        casaToken("]");
    }
}

//expressoes
procedimentoE(){
    procedimentoF();
    if(tokenLido == "=" || tokenLido == "<>" || tokenLido == "<" tokenLido == ">" ||  tokenLido == "<=" || tokenLido == ">="){
        casaToken(tokenLido); //tem que ver isso aqui no futuro
        procedimentoF();
    }
}

procedimentoF(){
    if(tokenLido == "+" || tokenLido == "-"){
        casaToken(tokenLido);
    }
    procedimentoG();
    while(tokenLido == "+" || tokenLido == "-" || tokenLido == "or"){
        casaToken(tokenLido);
        procedimentoG();
    }
}

procedimentoG(){
    procedimentoH();
    while(tokenLido == "*" || tokenLido == "and" || tokenLido == "/" || tokenLido == "%"){
        casaToken(tokenLido);
        procedimentoH();
    }
}

procedimentoH(){
    if(tokenLido == "id"){
        casaToken("id");
        if(tokenLido == "["){
            casaToken("[");
            procedimentoE();
            casaToken("]");
        }
    }
    else{
        if(tokenLido == "const"){
            casaToken("const");
        }
        else{
            if(tokenLido == "not"){
                casaToken("not");
                procedimentoH();
            }
            else{
                if(tokenLido("("){
                    casaToken("(");
                    procedimentoE();
                    casaToken(")");
                }
            }
        }
    }
}

int main(){
    analisadorSintatico();
    return 0;
}

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class TabelaSimbolos {
    int nextPos;
    
    public:
        unordered_map<string, int> hash;
        TabelaSimbolos();
        void addLexema(string);

};

TabelaSimbolos::TabelaSimbolos() {
        hash["final"] = 1;
        hash["int"] = 2;
        hash["char"] = 3;
        hash["string"] = 4;
        hash["boolean"] = 5;
        hash["for"] = 6;
        hash["if"] = 7;
        hash["else"] = 8;
        hash["then"] = 9;
        hash["TRUE"] = 10;
        hash["FALSE"] = 11;
        hash["end"] = 12;
        hash["or"] = 13;
        hash["not"] = 14;
        hash[":="] = 15;
        hash["="] = 16;
        hash["("] = 17;
        hash[")"] = 18;
        hash["{"] = 19;
        hash["}"] = 20;
        hash["["] = 21;
        hash["]"] = 22;
        hash["<"] = 23;
        hash[">"] = 24;
        hash["<>"] = 25;
        hash[">="] = 26;
        hash["<="] = 27;
        hash[","] = 28;
        hash["+"] = 29;
        hash["-"] = 30;
        hash["*"] = 31;
        hash["/"] = 32;
        hash[";"] = 33;
        hash["write"] = 34;
        hash["writeln"] = 35;
        hash["readln"] = 36;
        hash["%"] = 37;
        hash["main"] = 38;
        hash["eof"] = 39;
        nextPos = 40;
    }

    void TabelaSimbolos::addLexema(string lex){
        if(hash[lex] == 0){
            hash[lex] = nextPos;
            nextPos++;
        }
    }


//atributos globais
string tokenLido;
TabelaSimbolos ts;
int linha = 1; //linha atual do programa
enum Status {EXECUTANDO, ERRO, COMPILADO};
Status stat;
enum Erro {EOF_INESPERADO, LEXEMA_INESPERADO, TOKEN_INESPERADO, CARACTERE_INVALIDO};

void mensagemErro(Erro erro, string lex){
    stat = ERRO;
    cout << linha << "\n";
    switch(erro){
        case EOF_INESPERADO:
            cout << "fim de arquivo nao esperado.";
            break;
        case LEXEMA_INESPERADO: 
            cout << "lexema nao identificado [" << lex << "].";
            break;
        case TOKEN_INESPERADO:
            cout << "token nao esperado [" << lex << "].";
            break;
        case CARACTERE_INVALIDO:
            cout << "caractere invalido.";
            break;
    }
    exit(0);
}

string analisadorLexico(){
    int s = 0;
    char c;
    string lexema = "";
    while(s != 3 && s != 6 && s != 8 && s != 11 && s != 18){
        if(!cin.eof()){
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
                                                    tokenLido = c;
                                                    lexema += c;
                                                }
                                                else{
                                                    if(c == '\''){
                                                        s = 19;
                                                        lexema += c;
                                                    }
                                                    else{
                                                        if(c == '\"'){
                                                            s = 22;
                                                            lexema += c;
                                                        }
                                                        else{
                                                            if(c == '\n'){
                                                                linha++;
                                                            }
                                                            else{
                                                                if(!(c == ' ')){
                                                                    mensagemErro(CARACTERE_INVALIDO, "");
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
                        if(ts.hash[lexema] > 0 && ts.hash[lexema] < 40){ //caso o lexema seja uma palavra reservada
                            tokenLido = lexema;
                        }
                        else{
                            tokenLido = "id";
                        }
                        ts.addLexema(lexema); //adiciona o identificador na tabela de simbolos

                        cin.unget();
                    }
                    break;
                case 2:
                    if(c == '*'){
                        s = 5;
                    }
                    else{
                        s = 6;
                        tokenLido = '/';
                        cin.unget();
                    }
                    break;
                case 4:
                    if(c == '>' || c == '='){
                        s = 11;
                        if(c == '>'){
                            tokenLido = "<>";
                        }
                        else{
                            tokenLido = "<=";
                        }
                        lexema += c;
                    }
                    else{
                        s = 6;
                        tokenLido = "<";
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
                        tokenLido = ":=";
                        lexema += c;
                    }
                    else{
                        s = 6;
                        tokenLido = ":";
                        cin.unget();
                    }
                    break;
                case 10:
                    if(c == '='){
                        s = 11;
                        tokenLido = ">=";
                        lexema += c;
                    }
                    else{
                        s = 6;
                        tokenLido = ">";
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
                            tokenLido = "const";
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
                        tokenLido = "const";
                        cin.unget();
                    }
                    break;
                case 14:
                    if((c >= 48 && c <= 57) || (c >= 65 && c <= 70)){
                        s = 15;
                        lexema += c;
                    }
                    else{
                        mensagemErro(LEXEMA_INESPERADO, lexema);
                    }
                    break;
                case 15:
                    if(c == 'h'){
                        s = 18;
                        tokenLido = "char";
                        lexema += c;
                    }
                    else{
                        mensagemErro(LEXEMA_INESPERADO, lexema);
                    }
                    break;
                case 16:
                    if(c >= 48 && c <= 57){
                        s = 17;
                        lexema += c;
                    }
                    else{
                        if(c >= 65 && c <= 70){
                            s = 21;
                            lexema += c;
                        }
                        else{
                            s = 6;
                            tokenLido = "const";
                            cin.unget();
                        }
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
                            tokenLido = "char";
                            lexema += c;
                        }
                        else{
                            s = 6;
                            tokenLido = "const";
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
                        tokenLido = "char";
                        lexema += c;
                    }
                    else{
                        mensagemErro(LEXEMA_INESPERADO, lexema);
                    }
                    break;
                case 21:
                    if(c == 'h'){
                        s = 18;
                        tokenLido = "char";
                        cin.unget();
                    }
                    else{
                        mensagemErro(LEXEMA_INESPERADO, lexema);
                    }
                    break;
                case 22: 
                    if(c != '\n' && c != '$'){
                        if(c == '\"'){
                            s = 6;
                            tokenLido = "const";
                            lexema += c; 
                        }
                        else{
                            lexema += c;
                        }
                    }   
                    else{
                        mensagemErro(LEXEMA_INESPERADO, lexema);
                    }
            }
        }
        else{
            s = 8;
        }
    }
    cout << lexema << " ";
    return lexema;
}


/* Gramatica
*
* S-> {D} main '{'{C}'}' eof
*
* Declaracoes (D)
* D-> TN | K 
*
* Tipo (T)
* T-> char | string | int | boolean 
* Nome de declaracao (N)
* N-> idM{,idM};
* Modo de declaracao (M)
* M-> [:= const] | '['const']'
* Declaracao de constantes (K)
* K-> final id = const
*
* Comandos (C)
* R-> id['['E']'] := E
* C-> for'('A;E;A')' B
* C-> if'('E')' then B [else B]
* C-> ;
* C-> readln'('V')';
* C-> write'('E{,E}')'; | writeln'('E{,E}')';
  C-> R;
* 
* Blocos de comandos(A e B)
* A-> [R{,R}]
* B-> C | '{'{C}'}'
* Valor (V)
* V-> id['['E']']
* 
* Expressoes (E, F, G e H)
* E-> F [ (=|<>|<|>|<=|>=) F]
* F-> [+|-] G {(+|-|or) G} 
* G-> H {(*|and|/|%) H}
* H-> id ['['E']'] | const | not H | '('E')'
*/

void procedimentoS();
void procedimentoD();
void procedimentoT();
void procedimentoN();
void procedimentoM();
void procedimentoK();
void procedimentoC();
void procedimentoA();
void procedimentoB();
void procedimentoV();
void procedimentoE();
void procedimentoF();
void procedimentoG();
void procedimentoH();

void analisadorSintatico(){
    tokenLido = analisadorLexico();
    procedimentoS();
}

void casaToken(string tokenEsp){
    string token;
    cout << tokenLido << " " << tokenEsp << "\n";
    if(tokenEsp == tokenLido){
        token = tokenLido;
        analisadorLexico();
    }
    else{
        mensagemErro(TOKEN_INESPERADO, tokenLido);
    }
}

//S-> {D} main '{'{C}'}' eof
void procedimentoS(){
    while(tokenLido == "char" || tokenLido == "int" || tokenLido == "string" || tokenLido == "boolean" || tokenLido == "final"){
        procedimentoD();
    }
    casaToken("main");
    casaToken("{");
    while(tokenLido == "id" || tokenLido == "for" || tokenLido == "if" || tokenLido == ";" || tokenLido == "readln" || tokenLido == "write" || tokenLido == "writeln"){
        procedimentoC();
    }
    casaToken("}");

}

//Declaracoes
//D-> TN | K 
void procedimentoD(){
    if(tokenLido == "char" || tokenLido == "int" || tokenLido == "string" || tokenLido == "boolean"){
        procedimentoT();
        procedimentoN();
    }
    else{
        if(tokenLido == "final"){
            procedimentoK();
        }
        else{
            mensagemErro(TOKEN_INESPERADO, tokenLido);
        }
    }
}

//T-> char | string | int | boolean
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
                    mensagemErro(TOKEN_INESPERADO, tokenLido);
                }
            }
        }
    }
}

//N-> idM{,idM};
void procedimentoN(){
    casaToken("id");
    procedimentoM();
    while(tokenLido == ","){
        casaToken(",");
        casaToken("id");
        procedimentoM();
    }
    casaToken(";");
}

//M-> [:= const] | '['const']'
void procedimentoM(){
    if(tokenLido == "["){
        casaToken("[");
        casaToken("const");
        casaToken("]");
    }
    else{
        if(tokenLido == ":="){
            casaToken(":=");
            casaToken("const");
        }
    }
}

//K-> final id = const;
void procedimentoK(){
    casaToken("final");
    casaToken("id");
    casaToken("=");
    casaToken("const");
    casaToken(";");
}

//Comandos
//C-> id['['E']'] := E
//C-> for'('A;E;A')' B
//C-> if'('E')' then B [else B]
//C-> ;
//C-> readln'('V')';
//C-> write'('{E}')'; | writeln'('{E}')';
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
        casaToken(";");
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
                        while(tokenLido == ","){
                            casaToken(",");
                            procedimentoE();
                        }
                        casaToken(")");
                        casaToken(";");
                    }
                    else{
                        if(tokenLido == "writeln"){
                            casaToken("writeln");
                            casaToken("(");
                            procedimentoE();
                            while(tokenLido == ","){
                                casaToken(",");
                                procedimentoE();
                            }
                            casaToken(")");
                            casaToken(";");
                        }
                        else{
                            if(tokenLido == ";"){
                                casaToken(";");
                            }
                            else{
                                mensagemErro(TOKEN_INESPERADO, tokenLido);
                            }
                        }
                    }
                }
            }
        }
    }
}


//A-> [C{,C}]
void procedimentoA(){
    if(tokenLido == "id" || tokenLido == "for" || tokenLido == "if" || tokenLido == ";" || tokenLido == "readln" || tokenLido == "write" || tokenLido == "writeln"){
        procedimentoC();
        while(tokenLido == ","){
            casaToken(",");
            procedimentoC();
        }
    }
}

//* B-> C | '{'{C}'}'
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
            //casaToken(";");
        }
        else{
            mensagemErro(TOKEN_INESPERADO, tokenLido);
        }
    }
}

//V-> id['['E']']
void procedimentoV(){
    casaToken("id");
    if(tokenLido == "["){
        casaToken("[");
        procedimentoE();
        casaToken("]");
    }
}

//Expressoes
//E-> F [ (=|<>|<|>|<=|>=) F]
void procedimentoE(){
    procedimentoF();
    if(tokenLido == "=" || tokenLido == "<>" || tokenLido == "<" || tokenLido == ">" ||  tokenLido == "<=" || tokenLido == ">="){
        casaToken(tokenLido); //tem que ver isso aqui no futuro
        procedimentoF();
    }
}

//* F-> [+|-] G {(+|-|or) G} 
void procedimentoF(){
    if(tokenLido == "+" || tokenLido == "-"){
        casaToken(tokenLido);
    }
    procedimentoG();
    while(tokenLido == "+" || tokenLido == "-" || tokenLido == "or"){
        casaToken(tokenLido);
        procedimentoG();
    }
}

//G-> H {(*|and|/|%) H}
void procedimentoG(){
    procedimentoH();
    while(tokenLido == "*" || tokenLido == "and" || tokenLido == "/" || tokenLido == "%"){
        casaToken(tokenLido);
        procedimentoH();
    }
}

//H-> id ['['E']'] | const | not H | '('E')'
void procedimentoH(){
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
                if(tokenLido == "("){
                    casaToken("(");
                    procedimentoE();
                    casaToken(")");
                }
            }
        }
    }
}

int main(){
    stat = EXECUTANDO;
    analisadorSintatico();
    stat = EXECUTANDO;
    if(!(stat == ERRO)){
        cout << linha << " linhas compiladas.";
    }
    return 0;
}

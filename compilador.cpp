#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>


using namespace std;

class RegistroLexico{
    public:
        string token;
        string lexema;
        string tipo;
        int tamanho;
        Simbolo* endereco;
    public:
        RegistroLexico(string token, string lexema, string tipo, int tamanho, Simbolo* endereco);
};

class Simbolo{
    public:
        int codigo;
        string token;
        string lexema;
        string classe; //[final, var, void ("")]
        string tipo; //[int, bool, char]
        int tamanho; //[Unitario (= 0), vetor (> 0)]

    public:
        Simbolo(int codigo, string token, string lexema, string classe, string tipo, int tamanho);
};

Simbolo::Simbolo(int codigo, string token, string lexema, string classe, string tipo, int tamanho){
    this->codigo = codigo;
    this->token = token;
    this->lexema = lexema;
    this->classe = classe;
    this->tipo = tipo;
    this->tamanho = tamanho;
}

class TabelaSimbolos {
    int nextPos;
    
    public:
        unordered_map<string, Simbolo> hash;
        TabelaSimbolos();
        Simbolo* addLexema(Simbolo);

};

TabelaSimbolos::TabelaSimbolos() {
        hash["final"] = Simbolo(1, "final", "final", "", "", 0);
        hash["int"] = Simbolo(2, "int", "int", "", "", 0);
        hash["char"] = Simbolo(3, "char", "char", "", "", 0);
        hash["string"] = Simbolo(4, "string", "string", "", "", 0);
        hash["boolean"] = Simbolo(5, "boolean", "boolean", "", "", 0);
        hash["for"] = Simbolo(6, "for", "for", "", "", 0);
        hash["if"] = Simbolo(7, "if", "if", "", "", 0);
        hash["else"] = Simbolo(8, "else", "else", "", "", 0);
        hash["then"] = Simbolo(9, "then", "then", "", "", 0);
        hash["TRUE"] = Simbolo(10, "TRUE", "TRUE", "", "", 0);
        hash["FALSE"] = Simbolo(11, "FALSE", "FALSE", "", "", 0);
        hash["end"] = Simbolo(12, "end", "end", "", "", 0);
        hash["or"] = Simbolo(13, "or", "or", "", "", 0);
        hash["not"] = Simbolo(14, "not", "not", "", "", 0);
        hash[":="] = Simbolo(15, ":=", ":=", "", "", 0);
        hash["="] = Simbolo(16, "=", "=", "", "", 0);
        hash["("] = Simbolo(17, "(", "(", "", "", 0);
        hash[")"] = Simbolo(18, ")", ")", "", "", 0);
        hash["{"] = Simbolo(19, "{", "{", "", "", 0);
        hash["}"] = Simbolo(20, "}", "}", "", "", 0);
        hash["["] = Simbolo(21, "[", "[", "", "", 0);
        hash["]"] = Simbolo(22, "]", "]", "", "", 0);
        hash["<"] = Simbolo(23, "<", "<", "", "", 0);
        hash[">"] = Simbolo(24, ">", ">", "", "", 0);
        hash["<>"] = Simbolo(25, "<>", "<>", "", "", 0);
        hash[">="] = Simbolo(26, ">=", ">=", "", "", 0);
        hash["<="] = Simbolo(27, "<=", "<=", "", "", 0);
        hash[","] = Simbolo(28, ",", ",", "", "", 0);
        hash["+"] = Simbolo(29, "+", "+", "", "", 0);
        hash["-"] = Simbolo(30, "-", "-", "", "", 0);
        hash["*"] = Simbolo(31, "*", "*", "", "", 0);
        hash["/"] = Simbolo(32, "/", "/", "", "", 0);
        hash[";"] = Simbolo(33, ";", ";", "", "", 0);
        hash["write"] = Simbolo(34, "write", "write", "", "", 0);
        hash["writeln"] = Simbolo(35, "writeln", "writeln", "", "", 0);
        hash["readln"] = Simbolo(36, "readln", "readln", "", "", 0);
        hash["%"] = Simbolo(37, "%", "%", "", "", 0);
        hash["main"] = Simbolo(38, "main", "main", "", "", 0);
        hash["eof"] = Simbolo(39, "eof", "eof", "", "", 0);
        nextPos = 40;
    }

    //Metodo que adiciona um lexema na tabela de simbolos caso ele ainda nao exista
    Simbolo* TabelaSimbolos::addLexema(Simbolo simb){
        simb.codigo = nextPos;
        if (this->hash.find(simb.lexema) == this->hash.end()){
            hash[simb.lexema] = simb;
            nextPos++;
        }
        return &simb;
    }


//atributos globais
RegistroLexico tokenAnte; // token anterior
RegistroLexico tokenLido; // token atual
TabelaSimbolos ts; //tabela de simbolos
int linha = 1; //linha atual do programa
enum Status {EXECUTANDO, ERRO, COMPILADO}; //status possiveis de execucao
Status stat; //status de execucao
enum Erro {
    EOF_INESPERADO,
    LEXEMA_INESPERADO, 
    TOKEN_INESPERADO, 
    CARACTERE_INVALIDO,
    ID_NAO_DECLARADO,
    ID_JA_DECLARADO,
    CLASSE_INCOMPATIVEL,
    TIPOS_INCOMPATIVEIS,
    TAM_VETOR_EXCEDE_MAX
}; //codigo do erro

//printa a mensagem de erro na tela e encerra a execucao do compilador
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
        case ID_NAO_DECLARADO:
            cout << "identificador nao declarado [" << lex << "].";
            break;
        case ID_JA_DECLARADO:
            cout << "identificador ja declarado [" << lex << "].";
            break;
        case CLASSE_INCOMPATIVEL:
            cout << "classe de identificador incompativel [" << lex << "].";
            break;
        case TIPOS_INCOMPATIVEIS:
            cout << "tipos incompativeis.";
            break;
        case TAM_VETOR_EXCEDE_MAX:
            cout << "tamanho do vetor excede o maximo permitido.";
            break;
    }
    exit(0);
}

//verifica se o caractere e valido
bool caractereValido(char c){
    return ((c>=0 && c<=34) || c == 37 || (c>=39 && c <= 93) || c == 95 || (c >= 96 && c <= 123) || c == 125); //caracteres possiveis
}

Simbolo analisadorLexico(){
    int s = 0; //estado do automato
    char c; //caractere lido
    bool idValido = false; //checa se o id eh valido. ids compostos somente por underline sao invalidos

    string lexema = ""; //lexema lido
    string token = ""; //token que sera atribuido ao tokenLido


    Simbolo* endereco; //endereco do token lido na tabela de simbolos
    string tipo = ""; //tipo do token lido

    while(s != 3 && s != 6 && s != 8 && s != 11 && s != 18){
        if(!cin.eof()){
            cin.get(c);
            if(c == '\n'){
                if(!cin.eof()){
                    linha++;
                }
            }
            if(!caractereValido(c)){
                mensagemErro(CARACTERE_INVALIDO,"");
            }
            switch(s){
                case 0:
                    if(c >= 65 && c <= 90 || c == '_' || c >= 97 && c <= 122){ //letras ou _
                        if(c != '_'){ //checa se o id e composto somente de underline
                            idValido = true;
                        }
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
                                        if(c >= 49 && c <= 57){ //numeros 1-9
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
                                                    token = c;
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
                                                            if(c != ' ' && c != '\r' && c != '\t' && c != '\n'){
                                                                lexema += c;
                                                                mensagemErro(LEXEMA_INESPERADO, lexema);
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
                        if(c != '_'){ //checa se o id e composto somente de underline
                            idValido = true;
                        }
                        if(lexema.length() < 32){
                            s = 1;
                            lexema += c;
                        }
                    }
                    else{
                        s = 6;
                        if(idValido){
                            int posTabela = ts.hash[lexema].codigo; //posicao do lexema na tabela de simbolos
                            if(posTabela > 0 && posTabela < 40){ //caso o lexema seja uma palavra reservada
                                if(posTabela == 10 || posTabela == 11){ //lexema = "TRUE" ou "FALSE"
                                    token = "const";
                                    tipo = "boolean";
                                }
                                else{
                                    token = lexema;
                                }
                            }
                            else{
                                Simbolo tmpS = Simbolo(0,"id", lexema, "", "", 0);
                                endereco = ts.addLexema(tmpS); //adiciona o identificador na tabela de simbolos
                            }
                        }
                        else{
                            mensagemErro(LEXEMA_INESPERADO, lexema);
                        }
                        if(c != '\n'){
                            cin.unget();
                        }
                    }
                    break;
                case 2:
                    if(c == '*'){ //checa se eh comentario
                        s = 5;
                    }
                    else{
                        s = 6;
                        token = "/";
                        lexema = "/";
                        if(c != '\n'){
                            cin.unget();
                        }
                    }
                    break;
                case 4:
                    if(c == '>' || c == '='){
                        s = 11;
                        if(c == '>'){
                            token = "<>";
                        }
                        else{
                            token = "<=";
                        }
                        lexema += c;
                    }
                    else{
                        s = 6;
                        token = "<";
                        lexema = "<";
                        if(c != '\n'){
                            cin.unget();
                        }
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
                        if(c != '*'){
                            s = 5;
                        }
                    }
                    break;
                case 9:
                    if(c == '='){
                        s = 3;
                        tokenLido.token = ":=";
                        lexema += c;
                    }
                    else{
                        s = 6;
                        token = ":";
                        lexema = ":";
                        if(c != '\n'){
                            cin.unget();
                        }
                    }
                    break;
                case 10:
                    if(c == '='){
                        s = 11;
                        token = ">=";
                        lexema += c;
                    }
                    else{
                        s = 6;
                        token = ">";
                        lexema = ">";
                        if(c != '\n'){
                            cin.unget();
                        }
                    }
                    break;
                case 12:
                    if(c >= 48 && c <= 57){ //numeros 0-9 (int ou char)
                        s = 16;
                        lexema += c;
                    }
                    else{
                        if(c >= 65 && c <= 70){ //letras A-F (char)
                            s = 14;
                            lexema += c;
                        }
                        else{
                            s = 6;
                            token = "const";
                            tipo = "int";
                            if(c != '\n'){
                                cin.unget();
                            }
                        }
                    }
                    break;
                case 13:
                    if(c >= 48 && c <= 57){ //numeros 0-9 (int)
                        s = 13;
                        lexema += c;
                    }
                    else{
                        s = 6;
                        token = "const";
                        tipo = "int";
                        if(c != '\n'){
                            cin.unget();
                        }
                    }
                    break;
                case 14: //se chegou aqui ja eh hexa (char)
                    if((c >= 48 && c <= 57) || (c >= 65 && c <= 70)){ //A-F ou 0-9
                        s = 15;
                        lexema += c;
                    }
                    else{
                        mensagemErro(LEXEMA_INESPERADO, lexema);
                    }
                    break;
                case 15:
                    if(c == 'h'){ //h que finaliza um hexa (char)
                        s = 18;
                        token = "const";
                        tipo = "char";
                        lexema += c;
                    }
                    else{
                        //lexema += c;
                        mensagemErro(LEXEMA_INESPERADO, lexema);
                    }
                    break;
                case 16:
                    if(c >= 48 && c <= 57){ //0-9 (int ou char)
                        s = 17;
                        lexema += c;
                    }
                    else{
                        if(c >= 65 && c <= 70){//A-F (char)
                            s = 21;
                            lexema += c;
                        }
                        else{
                            s = 6;
                            token = "const";
                            tipo = "int";
                            if(c != '\n'){
                                cin.unget();
                            }
                        }
                    }
                    break;
                case 17:
                    if(c >= 48 && c <= 57){ //0-9 (int)
                        s = 13;
                        lexema += c;
                    }
                    else{
                        if(c == 'h'){ //h que finaliza um hexa (char)
                            s = 18;
                            token = "const";
                            tipo = "char";
                            lexema += c;
                        }
                        else{
                            s = 6;
                            token = "const";
                            tipo = "int";
                            if(c != '\n'){
                                cin.unget();
                            }
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
                        token = "const";
                        tipo = "char";
                        lexema += c;
                    }
                    else{
                        mensagemErro(LEXEMA_INESPERADO, lexema);
                    }
                    break;
                case 21:
                    if(c == 'h'){//h que finaliza um hexa (char)
                        s = 18;
                        token = "const";
                        tipo = "char";
                        lexema += "h";
                    }
                    else{
                        mensagemErro(LEXEMA_INESPERADO, lexema);
                    }
                    break;
                case 22: 
                    if(c != '\n' && c != '$'){
                        if(c == '\"'){
                            s = 6;
                            token = "const";
                            tipo = "string";
                            lexema += c; 
                        }
                        else{
                            lexema += c;
                        }
                    }   
                    else{
                        mensagemErro(LEXEMA_INESPERADO, lexema);
                    }
                    break;
            }
        }
        else{
            s = 8;
            if(stat == EXECUTANDO){
                mensagemErro(EOF_INESPERADO, "");
            }
        }
    }
    tokenLido.token = token;
    tokenLido.lexema = lexema;
    tokenLido.tipo = tipo;
    tokenLido.endereco = endereco;
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
* M-> [:= [-]const] | '['const']'
* Declaracao de constantes (K)
* K-> final id = [-]const
*
* Comandos (C)
* C-> for'('A;E;A')' B
* C-> if'('E')' then B [else B]
* C-> ;
* C-> readln'('V')';
* C-> write'('E{,E}')'; | writeln'('E{,E}')';
* C-> R;
* R-> id['['E']'] := E
* 
* Blocos de comandos(A e B)
* A-> [R{,R}]
* B-> C | '{'C{C}'}'
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
void procedimentoR();
void procedimentoA();
void procedimentoB();
void procedimentoV();
void procedimentoE();
void procedimentoF();
void procedimentoG();
void procedimentoH();

void analisadorSintatico(){
    analisadorLexico();
    procedimentoS();
}

void casaToken(string tokenEsp){
    //cout << tokenLido << " " << tokenEsp << "\n";
    if(tokenEsp == tokenLido.token){
        tokenAnte = tokenLido;
        analisadorLexico();
    }
    else{
        mensagemErro(TOKEN_INESPERADO, tokenLido.lexema);
    }
}

//S-> {D} main '{'{C}'}' eof
void procedimentoS(){
    while(tokenLido.token == "char" || tokenLido.token == "int" || tokenLido.token == "string" || tokenLido.token == "boolean" || tokenLido.token == "final"){
        procedimentoD();
    }
    casaToken("main");
    casaToken("{");
    while(tokenLido.token == "id" || tokenLido.token == "for" || tokenLido.token == "if" || tokenLido.token == ";" || tokenLido.token == "readln" || tokenLido.token == "write" || tokenLido.token == "writeln"){
        procedimentoC();
    }
    stat = COMPILADO;
    casaToken("}");
}

//Declaracoes
//D-> TN | K 
void procedimentoD(){
    if(tokenLido.token == "char" || tokenLido.token == "int" || tokenLido.token == "string" || tokenLido.token == "boolean"){
        string tipoID;
        procedimentoT(&tipoID);
        procedimentoN(tipoID);
    }
    else{
        if(tokenLido.token == "final"){
            procedimentoK();
        }
        else{
            mensagemErro(TOKEN_INESPERADO, tokenLido.lexema);
        }
    }
}

//T-> char | string | int | boolean
void procedimentoT(string* tipoID){
    if(tokenLido.token == "char"){
            casaToken("char");
            *tipoID = "char";
    }
    else{
        if(tokenLido.token == "int"){
            casaToken("int");
            *tipoID = "int";
        }
        else{
            if(tokenLido.token == "string"){
                casaToken("string");
            }
            else{
                if(tokenLido.token == "boolean"){
                    casaToken("boolean");
                    *tipoID = "boolean";
                }
                else{
                    mensagemErro(TOKEN_INESPERADO, tokenLido.lexema);
                }
            }
        }
    }
}

//N-> idM{,idM};
void procedimentoN(string tipoID){
    casaToken("id");
    RegistroLexico id = tokenAnte;
    if(id.endereco->classe != ""){
        mensagemErro(ID_JA_DECLARADO, id.lexema);
    }
    else{
        id.endereco->classe = "var";
        id.endereco->tipo = tipoID;
    }

    int tamanhoID;
    procedimentoM(&tamanhoID, tipoID);
    id.endereco->tamanho = tamanhoID;
    while(tokenLido.token == ","){
        casaToken(",");
        casaToken("id");
        id = tokenAnte;
        if(id.endereco->classe != ""){
            mensagemErro(ID_JA_DECLARADO, id.lexema);
        }
        else{
            id.endereco->classe = "var";
            id.endereco->tipo = tipoID;
        }
        procedimentoM(&tamanhoID, tipoID);
        id.endereco->tamanho = tamanhoID;
    }
    casaToken(";");
}

//M-> [:= [-]const] | '['const']'
void procedimentoM(int* tamanhoID, string tipoID){

    *tamanhoID = 0;

    if(tokenLido.token == "["){
        casaToken("[");
        casaToken("const");
        if(tokenAnte.tipo != "int"){
            mensagemErro(TIPOS_INCOMPATIVEIS, "");
        }
        *tamanhoID = stoi(tokenAnte.lexema);
        casaToken("]");
    }
    else{
        if(tokenLido.token == ":="){
            casaToken(":=");
            if(tokenLido.token == "-"){
                casaToken("-");
            }
            casaToken("const");
            if(tokenAnte.tipo != tipoID){
                mensagemErro(TIPOS_INCOMPATIVEIS, "");
            }
        }
    }
}

//K-> final id = const;
void procedimentoK(){
    casaToken("final");
    casaToken("id");
    RegistroLexico id = tokenAnte;
    if(id.endereco->classe != ""){
        mensagemErro(ID_JA_DECLARADO, id.lexema);
    }
    else{
        tokenAnte.endereco->classe = "const";
    }
    casaToken("=");
    if(tokenLido.token == "-"){
        casaToken("-");
    }
    casaToken("const");
    id.endereco->tipo = tokenAnte.tipo;
    id.endereco->tamanho = 0;
    casaToken(";");
}

//Comandos
//C-> for'('A;E;A')' B
//C-> if'('E')' then B [else B]
//C-> ;
//C-> readln'('V')';
//C-> write'('E{,E}')'; | writeln'('E{,E}')';
//C-> R;
void procedimentoC(){
    if(tokenLido.token == "id"){
        procedimentoR();
        casaToken(";");
    }
    else{
        if(tokenLido.token == "for"){
            casaToken("for");
            casaToken("(");
            procedimentoA();
            casaToken(";");
            string tipoE;
            int tamanhoE;
            procedimentoE(&tipoE, &tamanhoE);
            if(tipoE != "boolean"){
                mensagemErro(TIPOS_INCOMPATIVEIS, "");
            }
            casaToken(";");
            procedimentoA();
            casaToken(")");
            procedimentoB();
        }
        else{
            if(tokenLido.token == "if"){
                casaToken("if");
                casaToken("(");

                string tipoE;
                int tamanhoE;
                procedimentoE(&tipoE, &tamanhoE);
                if(tipoE != "boolean"){
                    mensagemErro(TIPOS_INCOMPATIVEIS, "");
                }

                casaToken(")");
                casaToken("then");
                procedimentoB();
                if(tokenLido.token == "else"){
                    casaToken("else");
                    procedimentoB();
                }
            }
            else{
                if(tokenLido.token == "readln"){
                    casaToken("readln");
                    casaToken("(");
                    procedimentoV();
                    casaToken(")");
                    casaToken(";");
                }
                else{
                    if(tokenLido.token == "write"){
                        casaToken("write");
                        casaToken("(");
                        procedimentoE();
                        while(tokenLido.token == ","){
                            casaToken(",");
                            procedimentoE();
                        }
                        casaToken(")");
                        casaToken(";");
                    }
                    else{
                        if(tokenLido.token == "writeln"){
                            casaToken("writeln");
                            casaToken("(");
                            procedimentoE();
                            while(tokenLido.token == ","){
                                casaToken(",");
                                procedimentoE();
                            }
                            casaToken(")");
                            casaToken(";");
                        }
                        else{
                            if(tokenLido.token == ";"){
                                casaToken(";");
                            }
                            else{
                                mensagemErro(TOKEN_INESPERADO, tokenLido.lexema);
                            }
                        }
                    }
                }
            }
        }
    }
}

//R-> id['['E']'] := E
void procedimentoR(){
    casaToken("id");
    if(tokenAnte.endereco->classe == ""){
        mensagemErro(ID_NAO_DECLARADO, "");
    }
    else{
        if(tokenAnte.endereco->classe == "const"){
            mensagemErro(CLASSE_INCOMPATIVEL, tokenAnte.lexema);
        }
    }
    string tipoR = tokenAnte.endereco->tipo;
    int tamanhoR = tokenAnte.endereco->tamanho;
    if(tokenLido.token == "["){
        casaToken("[");
        if(tamanhoR == 0){
            mensagemErro(TIPOS_INCOMPATIVEIS, "");
        }
        string tipoE1;
        int tamanhoE1;
        procedimentoE(&tipoE1, &tamanhoE1);
        if(tipoE1 != "int" || tamanhoE1 > 0){
            mensagemErro(TIPOS_INCOMPATIVEIS, "");
        }
        casaToken("]");
        tamanhoR = 0;
    }
    else{
        if(tamanhoR != 0){
            mensagemErro(TIPOS_INCOMPATIVEIS, "");
        }
    }
    casaToken(":=");
    string tipoE2;
    int tamanhoE2;
    procedimentoE(&tipoE2, &tamanhoE2);
    if(tipoE2 != tipoR || tamanhoE2 != 0){
        mensagemErro(TIPOS_INCOMPATIVEIS, "");
    }
}

//A-> [R{,R}]
void procedimentoA(){
    if(tokenLido.token == "id" || tokenLido.token == "for" || tokenLido.token == "if" || tokenLido.token == ";" || tokenLido.token == "readln" || tokenLido.token == "write" || tokenLido.token == "writeln"){
        procedimentoR();
        while(tokenLido.token == ","){
            casaToken(",");
            procedimentoR();
        }
    }
}

//* B-> C | '{'{C}'}'
void procedimentoB(){
    if(tokenLido.token == "{"){
        casaToken("{");
        procedimentoC();
        while(tokenLido.token == "id" || tokenLido.token == "for" || tokenLido.token == "if" || tokenLido.token == ";" || tokenLido.token == "readln" || tokenLido.token == "write" || tokenLido.token == "writeln"){
            procedimentoC();
        }
        casaToken("}");
    }
    else{
        if(tokenLido.token == "id" || tokenLido.token == "for" || tokenLido.token == "if" || tokenLido.token == ";" || tokenLido.token == "readln" || tokenLido.token == "write" || tokenLido.token == "writeln"){
            procedimentoC();
        }
        else{
            mensagemErro(TOKEN_INESPERADO, tokenLido.lexema);
        }
    }
}

//V-> id['['E']']
void procedimentoV(){
    casaToken("id");
    if(tokenAnte.endereco->classe == ""){
        mensagemErro(ID_NAO_DECLARADO, tokenAnte.lexema);
    }
    int tamanhoV = tokenAnte.endereco->tamanho;
    if(tokenLido.token == "["){
        casaToken("[");
        if(tamanhoV == 0){
            mensagemErro(TIPOS_INCOMPATIVEIS, "");
        }

        string tipoE;
        int tamanhoE;
        procedimentoE(&tipoE, &tamanhoE);
        if(tipoE != "int" || tamanhoE > 0){
            mensagemErro(TIPOS_INCOMPATIVEIS, "lexemaV");
        }
        casaToken("]");
    }
}

//Expressoes
//E-> F [ (=|<>|<|>|<=|>=) F]
void procedimentoE(string* tipoE, int* tamanhoE){
    *tipoE = "";
    *tamanhoE = 0;

    string tipoF1;
    int tamanhoF1;
    procedimentoF(&tipoF1, &tamanhoF1);
    *tipoE = tipoF1;
    *tamanhoE = tamanhoF1;

    string tipoF2;
    int tamanhoF2;
    if(tokenLido.token == "=" || tokenLido.token == "<>" || tokenLido.token == "<" || tokenLido.token == ">" ||  tokenLido.token == "<=" || tokenLido.token == ">="){
        if(tokenLido.token == "="){
            casaToken("=");
            procedimentoF(&tipoF2, &tamanhoF2);
            if(*tipoE != tipoF2){
                mensagemErro(TIPOS_INCOMPATIVEIS, "");
            }
            else{
                if(*tipoE == "char"){ //checa comparacao de array de chars (string)
                    if((tamanhoE > 0 && tamanhoF2 == 0) || (tamanhoE == 0 && tamanhoF2 > 0)){
                        mensagemErro(TIPOS_INCOMPATIVEIS, "");
                    }
                }
                else{
                    if(tamanhoE != 0 || tamanhoF2 != 0){ //checa array de tipo != char
                        mensagemErro(TIPOS_INCOMPATIVEIS, "");
                    }
                }
            }
        }
        else{
            casaToken(tokenLido.token); //tem que ver isso aqui no futuro
            procedimentoF(&tipoF2, &tamanhoF2);
            if(*tipoE != tipoF2){
                mensagemErro(TIPOS_INCOMPATIVEIS, "");
            }
            if(tamanhoE != 0 || tamanhoF2 != 0){
                mensagemErro(TIPOS_INCOMPATIVEIS, "");
            }
        }
    }
}

//F-> [+|-] G {(+|-|or) G} 
void procedimentoF(string* tipoF, int* tamanhoF){

    *tipoF = ""; 
    *tamanhoF = 0;

    bool temSinal = false;
    if(tokenLido.token == "+" || tokenLido.token == "-"){
        casaToken(tokenLido.token);
        temSinal = true;
    }

    string tipoG1;
    int tamanhoG1;
    procedimentoG(&tipoG1, &tamanhoG1);
    if(temSinal && (tipoG1 != "int" || tamanhoG1 != 0)){
        mensagemErro(TIPOS_INCOMPATIVEIS, "");
    }
    *tipoF = tipoG1;
    *tamanhoF = tamanhoG1;


    while(tokenLido.token == "+" || tokenLido.token == "-" || tokenLido.token == "or"){
        string oper = tokenLido.token;
        casaToken(tokenLido.token);

        string tipoG2;
        int tamanhoG2;
        procedimentoG(&tipoG2, &tamanhoG2);

        if(oper == "or"){
            if(*tipoF != "boolean" || tipoG2 != "boolean"){
                mensagemErro(TIPOS_INCOMPATIVEIS, "");
            }
        }
        else{
            if(*tipoF != "int" || tipoG2 != "int"){
                mensagemErro(TIPOS_INCOMPATIVEIS, "");
            }
        }
    }
}

//G-> H {(*|and|/|%) H}
void procedimentoG(string* tipoG, int* tamanhoG){
    *tipoG = "";
    *tamanhoG = 0;

    string tipoH1;
    int tamanhoH1;
    procedimentoH(&tipoH1, &tamanhoH1);
    *tipoG = tipoH1;
    *tamanhoG = tamanhoH1;

    while(tokenLido.token == "*" || tokenLido.token == "and" || tokenLido.token == "/" || tokenLido.token == "%"){
        string oper = tokenLido.token;
        casaToken(tokenLido.token);

        string tipoH2;
        int tamanhoH2;
        procedimentoH(&tipoH2, &tamanhoH2);
        if(tamanhoG != 0 || tamanhoH2 != 0){
            mensagemErro(TIPOS_INCOMPATIVEIS, "");
        }
        if(oper == "&"){
            if(*tipoG != "boolean" || tipoH2 != "boolean"){
                mensagemErro(TIPOS_INCOMPATIVEIS, "");
            }
        }
        else{
            if(*tipoG != "int" || tipoH2 != "int"){
                mensagemErro(TIPOS_INCOMPATIVEIS, "");
            }
        }     
    }
}

//H-> id ['['E']'] | const | not H | '('E')'
void procedimentoH(string* tipoH, int* tamanhoH){

    *tipoH = "";
    *tamanhoH = 0;

    if(tokenLido.token == "id"){
        casaToken("id");
        if(tokenAnte.endereco->classe == ""){
            mensagemErro(TIPOS_INCOMPATIVEIS, "");
        }
        *tipoH = tokenAnte.endereco->tipo;
        *tamanhoH = tokenAnte.endereco->tamanho;
        if(tokenLido.token == "["){
            casaToken("[");

            if(tamanhoH == 0){
                mensagemErro(TIPOS_INCOMPATIVEIS, "");
            }
            string tipoE1;
            int tamanhoE1;
            procedimentoE(&tipoE1, &tamanhoE1);
            if(tipoE1 != "int" || tamanhoE1 > 0){
                mensagemErro(TIPOS_INCOMPATIVEIS, "");
            }

            casaToken("]");
            *tamanhoH = 0;
        }
        else{
            if(tamanhoH != 0){
                mensagemErro(TIPOS_INCOMPATIVEIS, "");
            }
        }
    }
    else{
        if(tokenLido.token == "const"){
            casaToken("const");
            *tipoH = tokenAnte.tipo;
            *tamanhoH = tokenAnte.tamanho;
        }
        else{
            if(tokenLido.token == "not"){
                casaToken("not");
                string tipoH1;
                int tamanhoH1;
                procedimentoH(&tipoH1, &tamanhoH1);
                if(tipoH1 != "boolean"){
                    mensagemErro(TIPOS_INCOMPATIVEIS, "");
                }
                *tipoH = tipoH1;
                *tamanhoH = tamanhoH1;
            }
            else{
                if(tokenLido.token == "("){
                    casaToken("(");
                    string tipoE2;
                    int tamanhoE2;
                    procedimentoE(&tipoE2, &tamanhoE2);
                    *tipoH = tipoE2;
                    *tamanhoH = tamanhoE2;
                    casaToken(")");
                }
            }
        }
    }
}

int main(){
    stat = EXECUTANDO;
    analisadorSintatico();
    if(!(stat == ERRO)){
        cout << linha << " linhas compiladas.";
    }
    return 0;
}

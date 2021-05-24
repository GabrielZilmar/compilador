#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>


/* Grupo 30
* Erick Hermano Lage Novais
* Gabriel Campos Zilmar Caetano
* Joao Carlos Rodrigues Couto
*/

using namespace std;

class Simbolo;
class RegistroLexico;
void procedimentoS();
void procedimentoD();
void procedimentoT(string *tipoID);
void procedimentoN(string tipoID);
void procedimentoM(int *tamanhoID, string tipoID);
void procedimentoK();
void procedimentoC();
void procedimentoR();
void procedimentoA();
void procedimentoB();
void procedimentoV();
void procedimentoE(string *tipoE, int *tamanhoE, int *endE);
void procedimentoF(string *tipoF, int *tamanhoF, int *endF);
void procedimentoG(string *tipoG, int *tamanhoG, int *endG);
void procedimentoH(string *tipoH, int *tamanhoH, int *endH);

class RegistroLexico
{
public:
    string token;
    string lexema;
    string tipo;
    int tamanho;
    Simbolo *endereco;

public:
    RegistroLexico();
    RegistroLexico(string token, string lexema, string tipo, int tamanho, Simbolo *endereco);
};

RegistroLexico::RegistroLexico()
{
    this->token = "";
    this->lexema = "";
    this->tipo = "";
    this->tamanho = 0;
    this->endereco = nullptr;
}

RegistroLexico::RegistroLexico(string token, string lexema, string tipo, int tamanho, Simbolo *endereco)
{
    this->token = token;
    this->lexema = lexema;
    this->tipo = tipo;
    this->tamanho = tamanho;
    this->endereco = endereco;
}

class Simbolo
{
public:
    int codigo;
    string token;
    string lexema;
    string classe; //[final, var, void ("")]
    string tipo;   //[int, bool, char]
    int tamanho;   //[Unitario (= 0), vetor (> 0)]
    int end;  //endereco na memoria


public:
    Simbolo();
    Simbolo(int codigo, string token, string lexema, string classe, string tipo, int tamanho);
};

Simbolo::Simbolo()
{
    this->codigo = 0;
    this->token = "";
    this->lexema = "";
    this->classe = "";
    this->tipo = "";
    this->tamanho = 0;
}

Simbolo::Simbolo(int codigo, string token, string lexema, string classe, string tipo, int tamanho)
{
    this->codigo = codigo;
    this->token = token;
    this->lexema = lexema;
    this->classe = classe;
    this->tipo = tipo;
    this->tamanho = tamanho;
}

class TabelaSimbolos
{
    int nextPos;

public:
    unordered_map<string, Simbolo> hash;
    TabelaSimbolos();
    Simbolo *addLexema(Simbolo &simb);
    void printTabela();
};

TabelaSimbolos::TabelaSimbolos()
{
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
    hash["and"] = Simbolo(12, "and", "and", "", "", 0);
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

void TabelaSimbolos::printTabela(){
    for (auto const &pair: hash){  
        cout << "{" << pair.first << ": " << pair.second.lexema << "}\n";     
    }
}

//Metodo que adiciona um lexema na tabela de simbolos caso ele ainda nao exista
Simbolo *TabelaSimbolos::addLexema(Simbolo &simb)
{
    if (this->hash.find(simb.lexema) == this->hash.end())
    {
        simb.codigo = nextPos;
        hash[simb.lexema] = simb;
        nextPos++;
    }
    return &hash[simb.lexema];
}

//atributos globais
int proxEndereco = 0x4000; // proximo endereco disponivel na memoria 
int proxEndTemp = 0;       // proximo endereco disponivel na memoria dos temporarios
int proxRot = 1;           // proximo rotulo para as instrucoes de salto
RegistroLexico tokenAnte;  // token anterior
RegistroLexico tokenLido;  // token atual
TabelaSimbolos ts;         // tabela de simbolos
int linha = 1;             // linha atual do programa
ofstream fout("exec.asm"); // arquivo de saida

enum Status
{
    EXECUTANDO,
    ERRO,
    COMPILADO
};           //status possiveis de execucao
Status stat; //status de execucao atual
enum Erro
{
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
void mensagemErro(Erro erro, string lex)
{
    stat = ERRO;
    cout << linha << "\n";
    switch (erro)
    {
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
bool caractereValido(char c)
{
    return ((c >= 0 && c <= 34) || c == 37 || (c >= 39 && c <= 93) || c == 95 || (c >= 96 && c <= 123) || c == 125); //caracteres possiveis
}

void analisadorLexico()
{
    int s = 0;             //estado do automato
    char c;                //caractere lido
    bool idValido = false; //checa se o id eh valido. ids compostos somente por underline sao invalidos

    string lexema = ""; //lexema lido
    string token = "";  //token que sera atribuido ao tokenLido

    Simbolo *endereco; //endereco do token lido na tabela de simbolos
    string tipo = "";  //tipo do token lido
    int tamanho = 0;

    while (s != 3 && s != 6 && s != 8 && s != 11 && s != 18)
    {
        if (!cin.eof())
        {
            cin.get(c);
            if (c == '\n')
            {
                if (!cin.eof())
                {
                    linha++;
                }
            }
            if (!caractereValido(c))
            {
                mensagemErro(CARACTERE_INVALIDO, "");
            }
            switch (s)
            {
            case 0:
                if (c >= 65 && c <= 90 || c == '_' || c >= 97 && c <= 122)
                { //letras ou _
                    if (c != '_')
                    { //checa se o id e composto somente de underline
                        idValido = true;
                    }
                    s = 1;
                    lexema += c;
                }
                else
                {
                    if (c == ':')
                    {
                        s = 9;
                        lexema += c;
                    }
                    else
                    {
                        if (c == '<')
                        {
                            s = 4;
                            lexema += c;
                        }
                        else
                        {
                            if (c == '>')
                            {
                                s = 10;
                                lexema += c;
                            }
                            else
                            {
                                if (c == '/')
                                {
                                    s = 2;
                                    lexema += c;
                                }
                                else
                                {
                                    if (c >= 49 && c <= 57)
                                    { //numeros 1-9
                                        s = 13;
                                        lexema += c;
                                    }
                                    else
                                    {
                                        if (c == '0')
                                        {
                                            s = 12;
                                            lexema += c;
                                        }
                                        else
                                        {
                                            if (c == '-' || c == '+' || c == '*' || c == '%' || c == '=' || c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' || c == ';')
                                            {
                                                s = 8;
                                                lexema += c;
                                                token = c;
                                            }
                                            else
                                            {
                                                if (c == '\'')
                                                {
                                                    s = 19;
                                                    lexema += c;
                                                }
                                                else
                                                {
                                                    if (c == '\"')
                                                    {
                                                        s = 22;
                                                        lexema += c;
                                                    }
                                                    else
                                                    {
                                                        if (c != ' ' && c != '\r' && c != '\t' && c != '\n')
                                                        {
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
                if ((c >= 65 && c <= 90) || c == '_' || (c >= 48 && c <= 57) || c >= 97 && c <= 122)
                { //letra, digito ou _
                    if (c != '_')
                    { //checa se o id e composto somente de underline
                        idValido = true;
                    }
                    if (lexema.length() < 32)
                    {
                        s = 1;
                        lexema += c;
                    }
                }
                else
                {
                    s = 6;
                    if (idValido)
                    {
                        int posTabela = (ts.hash.find(lexema) == ts.hash.end()) ? 0 : ts.hash[lexema].codigo; //posicao do lexema na tabela de simbolos
                        if (posTabela > 0 && posTabela < 40)
                        { //caso o lexema seja uma palavra reservada
                            if (posTabela == 10 || posTabela == 11)
                            { //lexema = "TRUE" ou "FALSE"
                                token = "const";
                                tipo = "boolean";
                            }
                            else
                            {
                                token = lexema;
                            }
                        }
                        else
                        {
                            token = "id";
                            Simbolo tmpS = Simbolo(0, token, lexema, "", "", 0);
                            endereco = ts.addLexema(tmpS); //adiciona o identificador na tabela de simbolos
                        }
                    }
                    else
                    {
                        mensagemErro(LEXEMA_INESPERADO, lexema);
                    }
                    if (c != '\n')
                    {
                        cin.unget();
                    }
                }
                break;
            case 2:
                if (c == '*')
                { //checa se eh comentario
                    s = 5;
                }
                else
                {
                    s = 6;
                    token = "/";
                    lexema = "/";
                    if (c != '\n')
                    {
                        cin.unget();
                    }
                }
                break;
            case 4:
                if (c == '>' || c == '=')
                {
                    s = 11;
                    if (c == '>')
                    {
                        token = "<>";
                    }
                    else
                    {
                        token = "<=";
                    }
                    lexema += c;
                }
                else
                {
                    s = 6;
                    token = "<";
                    lexema = "<";
                    if (c != '\n')
                    {
                        cin.unget();
                    }
                }
                break;
            case 5:
                if (c == '*')
                {
                    s = 7;
                }
                break;
            case 7:
                if (c == '/')
                {
                    s = 0;
                    lexema = "";
                }
                else
                {
                    if (c != '*')
                    {
                        s = 5;
                    }
                }
                break;
            case 9:
                if (c == '=')
                {
                    s = 3;
                    token = ":=";
                    lexema += c;
                }
                else
                {
                    s = 6;
                    token = ":";
                    lexema = ":";
                    if (c != '\n')
                    {
                        cin.unget();
                    }
                }
                break;
            case 10:
                if (c == '=')
                {
                    s = 11;
                    token = ">=";
                    lexema += c;
                }
                else
                {
                    s = 6;
                    token = ">";
                    lexema = ">";
                    if (c != '\n')
                    {
                        cin.unget();
                    }
                }
                break;
            case 12:
                if (c >= 48 && c <= 57)
                { //numeros 0-9 (int ou char)
                    s = 16;
                    lexema += c;
                }
                else
                {
                    if (c >= 65 && c <= 70)
                    { //letras A-F (char)
                        s = 14;
                        lexema += c;
                    }
                    else
                    {
                        s = 6;
                        token = "const";
                        tipo = "int";
                        if (c != '\n')
                        {
                            cin.unget();
                        }
                    }
                }
                break;
            case 13:
                if (c >= 48 && c <= 57)
                { //numeros 0-9 (int)
                    s = 13;
                    lexema += c;
                }
                else
                {
                    s = 6;
                    token = "const";
                    tipo = "int";
                    if (c != '\n')
                    {
                        cin.unget();
                    }
                }
                break;
            case 14: //se chegou aqui ja eh hexa (char)
                if ((c >= 48 && c <= 57) || (c >= 65 && c <= 70))
                { //A-F ou 0-9
                    s = 15;
                    lexema += c;
                }
                else
                {
                    mensagemErro(LEXEMA_INESPERADO, lexema);
                }
                break;
            case 15:
                if (c == 'h')
                { //h que finaliza um hexa (char)
                    s = 18;
                    token = "const";
                    tipo = "char";
                    lexema += c;
                }
                else
                {
                    //lexema += c;
                    mensagemErro(LEXEMA_INESPERADO, lexema);
                }
                break;
            case 16:
                if (c >= 48 && c <= 57)
                { //0-9 (int ou char)
                    s = 17;
                    lexema += c;
                }
                else
                {
                    if (c >= 65 && c <= 70)
                    { //A-F (char)
                        s = 21;
                        lexema += c;
                    }
                    else
                    {
                        s = 6;
                        token = "const";
                        tipo = "int";
                        if (c != '\n')
                        {
                            cin.unget();
                        }
                    }
                }
                break;
            case 17:
                if (c >= 48 && c <= 57)
                { //0-9 (int)
                    s = 13;
                    lexema += c;
                }
                else
                {
                    if (c == 'h')
                    { //h que finaliza um hexa (char)
                        s = 18;
                        token = "const";
                        tipo = "char";
                        lexema += c;
                    }
                    else
                    {
                        s = 6;
                        token = "const";
                        tipo = "int";
                        if (c != '\n')
                        {
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
                if (c == '\'')
                {
                    s = 8;
                    token = "const";
                    tipo = "char";
                    lexema += c;
                }
                else
                {
                    mensagemErro(LEXEMA_INESPERADO, lexema);
                }
                break;
            case 21:
                if (c == 'h')
                { //h que finaliza um hexa (char)
                    s = 18;
                    token = "const";
                    tipo = "char";
                    lexema += "h";
                }
                else
                {
                    mensagemErro(LEXEMA_INESPERADO, lexema);
                }
                break;
            case 22:
                if (c != '\n' && c != '$')
                {
                    if (c == '\"')
                    {
                        s = 6;
                        token = "const";
                        tipo = "char";
                        tamanho = lexema.length()-1;
                        lexema += c;
                    }
                    else
                    {
                        lexema += c;
                    }
                }
                else
                {
                    mensagemErro(LEXEMA_INESPERADO, lexema);
                }
                break;
            }
        }
        else
        {
            s = 8;
            if (stat == EXECUTANDO)
            {
                mensagemErro(EOF_INESPERADO, "");
            }
        }
    }
    tokenLido.token = token;
    tokenLido.lexema = lexema;
    tokenLido.tipo = tipo;
    tokenLido.tamanho = tamanho;
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
* T-> char | int | boolean 
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

int novoTemp(string tipo, int tamanho){
    int endereco = proxEndTemp;
    if(tamanho > 0){
        if(tipo == "char"){
            proxEndTemp += tamanho + 1;
        }
        else{
            proxEndTemp += tamanho * 2;
        }
    }
    else{
        proxEndTemp += (tipo == "char" ? 1 : 2);
    }
    return endereco;  
}

string novoRot(){
    string rotulo = "R" + to_string(proxRot);
    proxRot++;
    return rotulo;
}

void analisadorSintatico()
{
    analisadorLexico();
    procedimentoS();
}

void casaToken(string tokenEsp)
{
    //cout << tokenLido.lexema << "/" << tokenLido.token << "\\" << tokenEsp << "\n";
    if (tokenEsp == tokenLido.token)
    {
        tokenAnte = tokenLido;
        analisadorLexico();

    }
    else
    {
        mensagemErro(TOKEN_INESPERADO, tokenLido.lexema);
    }
}

//S-> {D} main '{'{C}'}' eof
void procedimentoS()
{
    fout << "sseg SEGMENT STACK ;início seg. pilha" << endl <<
            "byte 4000h DUP(?) ;dimensiona pilha" << endl <<
            "sseg ENDS ;fim seg. pilha" << endl << endl <<
            "dseg SEGMENT PUBLIC ;início seg. dados" << endl <<
            "byte 4000h DUP(?) ;temporários" << endl;
    while (tokenLido.token == "char" || tokenLido.token == "int" || tokenLido.token == "string" || tokenLido.token == "boolean" || tokenLido.token == "final")
    {
        procedimentoD();
    }
    fout << "dseg ENDS ;fim seg. dados" << endl << endl;
    casaToken("main");
    fout << "cseg SEGMENT PUBLIC ;início seg. código" << endl <<
            "ASSUME CS:cseg, DS:dseg" << endl <<
            "strt:" << endl <<
            "mov ax, dseg" << endl <<
            "mov ds, ax" << endl;
    casaToken("{");
    while (tokenLido.token == "id" || tokenLido.token == "for" || tokenLido.token == "if" || tokenLido.token == ";" || tokenLido.token == "readln" || tokenLido.token == "write" || tokenLido.token == "writeln")
    {
        procedimentoC();
    }
    fout << "mov ah, 4Ch" << endl <<
            "int 21h" << endl << 
            "cseg ENDS ;fim seg. código" << endl <<
            "END strt ;fim programa" << endl;
    stat = COMPILADO;
    casaToken("}");
}

//Declaracoes
//D-> TN | K
void procedimentoD()
{
    if (tokenLido.token == "char" || tokenLido.token == "int" || tokenLido.token == "string" || tokenLido.token == "boolean")
    {
        string tipoID;
        procedimentoT(&tipoID);
        procedimentoN(tipoID);
    }
    else
    {
        if (tokenLido.token == "final")
        {
            procedimentoK();
        }
        else
        {
            mensagemErro(TOKEN_INESPERADO, tokenLido.lexema);
        }
    }
}

//T-> char | int | boolean
void procedimentoT(string *tipoID)
{
    if (tokenLido.token == "char")
    {
        casaToken("char");
        *tipoID = "char";
    }
    else
    {
        if (tokenLido.token == "int")
        {
            casaToken("int");
            *tipoID = "int";
        }
        else
        {
            if (tokenLido.token == "boolean")
            {
                casaToken("boolean");
                *tipoID = "boolean";
            }
            else
            {
                mensagemErro(TOKEN_INESPERADO, tokenLido.lexema);
            }
        }
    }
}

//N-> idM{,idM};
void procedimentoN(string tipoID)
{
    casaToken("id");
    RegistroLexico id = tokenAnte;
    //ts.printTabela();
    if (id.endereco->classe != "")
    {
        mensagemErro(ID_JA_DECLARADO, id.lexema);
    }
    else
    {
        id.endereco->classe = "var";
        id.endereco->tipo = tipoID;
    }
    int tamanhoID;
    procedimentoM(&tamanhoID, tipoID);
    id.endereco->tamanho = tamanhoID;
    if(tamanhoID > 0){
        proxEndereco += tamanhoID * (tipoID == "char" ? 1 : 2);
    }
    else{
        proxEndereco += (tipoID == "char" ? 1 : 2);
    }
    while (tokenLido.token == ",")
    {
        casaToken(",");
        casaToken("id");
        id = tokenAnte;
        if (id.endereco->classe != "")
        {
            mensagemErro(ID_JA_DECLARADO, id.lexema);
        }
        else
        {
            id.endereco->classe = "var";
            id.endereco->tipo = tipoID;
        }
        procedimentoM(&tamanhoID, tipoID);
        id.endereco->tamanho = tamanhoID;
        id.endereco->end = proxEndereco;
        if(tamanhoID > 0){
            if(tipoID == "char"){
                proxEndereco += tamanhoID + 1;
            }
            else{
                proxEndereco += tamanhoID * 2;
            }
        }
        else{
            proxEndereco += (tipoID == "char" ? 1 : 2);
        }
    }
    casaToken(";");
}

//M-> [:= [-]const] | '['const']'
void procedimentoM(int *tamanhoID, string tipoID)
{
    bool temSinal = false;
    *tamanhoID = 0;

    if(tipoID == "char"){
        fout << "byte ";
    }
    else{
        fout << "sword ";
    }
    if (tokenLido.token == "[")
    {
        casaToken("[");
        casaToken("const");
        if (tokenAnte.tipo != "int")
        {
            mensagemErro(TIPOS_INCOMPATIVEIS, "");
        }
        if((tipoID == "int" && stoi(tokenAnte.lexema) >  4096) || stoi(tokenAnte.lexema) > 8191){
            mensagemErro(TAM_VETOR_EXCEDE_MAX, "");
        }
        *tamanhoID = stoi(tokenAnte.lexema);
        fout << *tamanhoID+1 << " DUP(?)" << endl;
        casaToken("]");
    }
    else
    {
        if (tokenLido.token == ":=")
        {
            casaToken(":=");
            if (tokenLido.token == "-")
            {
                temSinal = true;
                casaToken("-");
                fout << "-";
            }
            casaToken("const");
            if(temSinal && tokenAnte.tipo != "int"){
                mensagemErro(TIPOS_INCOMPATIVEIS, "");
            }
            if (tokenAnte.tipo != tipoID || tokenAnte.tamanho > 0)
            {
                mensagemErro(TIPOS_INCOMPATIVEIS, "");
            }
            if(tokenAnte.lexema == "TRUE" || tokenAnte.lexema == "FALSE"){
                fout << ((tokenAnte.lexema == "TRUE") ? 1 : 0) << endl;
            }
            else{
                fout << tokenAnte.lexema << endl;
            }
        }
        else{
            fout << "?" << endl;
        }
    }
}

//K-> final id = const;
void procedimentoK()
{
    casaToken("final");
    casaToken("id");
    RegistroLexico id = tokenAnte;
    if (id.endereco->classe != "")
    {
        mensagemErro(ID_JA_DECLARADO, id.lexema);
    }
    else
    {
        tokenAnte.endereco->classe = "const";
    }
    casaToken("=");
    if (tokenLido.token == "-")
    {
        casaToken("-");
    }
    casaToken("const");
    if(tokenAnte.tipo == "char"){
        fout << "byte ";
    }
    else{
        fout << "sword ";
    }
    if(tokenAnte.lexema == "TRUE" || tokenAnte.lexema == "FALSE"){
        fout << ((tokenAnte.lexema == "TRUE") ? 1 : 0) << endl;
    }
    else{
        fout << tokenAnte.lexema << endl;
    }
    if(tokenAnte.tamanho > 0){
        mensagemErro(TIPOS_INCOMPATIVEIS, "");
    }
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
void procedimentoC()
{
    if (tokenLido.token == "id")
    {
        procedimentoR();
        casaToken(";");
    }
    else
    {
        if (tokenLido.token == "for")
        {
            casaToken("for");
            casaToken("(");
            procedimentoA();
            casaToken(";");
            string tipoE;
            int tamanhoE;
            int endE;

            string rotInicio = novoRot();
            string rotFim = novoRot();
            string rotCmd = novoRot(); //comandos do for
            string rotInc = novoRot(); //incremento do for

            fout << rotInicio << ":" << endl;

            procedimentoE(&tipoE, &tamanhoE, &endE);
            if (tipoE != "boolean")
            {
                mensagemErro(TIPOS_INCOMPATIVEIS, "");
            }
            fout << "mov AX, DS:[" << endE << "]" << endl <<
                    "cmp AX, 1" << endl <<
                    "je " << rotCmd << endl <<
                    "jmp " << rotFim << endl <<
                    rotInc << ":" << endl;

            casaToken(";");
            procedimentoA();
            casaToken(")");

            fout << "jmp " << rotInicio << endl <<
                    rotCmd << ":" << endl;
            procedimentoB();
            fout << "jmp " << rotInc << endl <<
                    rotFim << ":" << endl;
            proxEndTemp = 0;
        }
        else
        {
            if (tokenLido.token == "if")
            {
                casaToken("if");
                casaToken("(");

                string tipoE;
                int tamanhoE;
                int endE;

                string rotFalso = novoRot(); //caso o if seja falso
                string rotFim = novoRot();
                procedimentoE(&tipoE, &tamanhoE, &endE);
                if (tipoE != "boolean")
                {
                    mensagemErro(TIPOS_INCOMPATIVEIS, "");
                }
                fout << "mov AX, DS:[" << endE << "]" << endl <<
                        "cmp AX, 1" << endl <<
                        "jne " << rotFalso << endl;


                casaToken(")");
                casaToken("then");
                procedimentoB();
                fout << "jmp " << rotFim << endl <<
                        rotFalso << ":" << endl;

                if (tokenLido.token == "else")
                {
                    casaToken("else");
                    procedimentoB();
                }
                fout << rotFim << ":" << endl;
                proxEndTemp = 0;
            }
            else
            {
                if (tokenLido.token == "readln")
                {
                    casaToken("readln");
                    casaToken("(");
                    procedimentoV();
                    casaToken(")");
                    casaToken(";");
                    proxEndTemp = 0;
                }
                else
                {
                    if (tokenLido.token == "write")
                    {
                        casaToken("write");
                        casaToken("(");
                        string tipoE;
                        int tamanhoE;
                        int endE;
                        procedimentoE(&tipoE, &tamanhoE, &endE);

                        string j1 = novoRot();
                        string j2 = novoRot();
                        string j3 = novoRot();

                        int stringEnd;
                        if(tipoE == "char"){
                            stringEnd = novoTemp("char", 255);

                            fout << "mov AX, DS:[" << endE << "]" << endl;

                            fout << "mov DI, " << stringEnd << endl <<
                                    "mov CX, 0" << endl <<
                                    "cmp AX, 0" << endl <<
                                    "jge " << j1 << endl <<
                                    "mov BL, 2Dh" << endl <<
                                    "mov DS:[DI], BL" << endl <<
                                    "add DI, 1" << endl <<
                                    "neg AX" << endl <<
                                    j1 << ":" << endl <<
                                    "mov BX, 10" << endl <<
                                    j2 << ":" << endl <<
                                    "add CX, 1" << endl <<
                                    "mov DX, 0" << endl <<
                                    "idiv BX" << endl <<
                                    "push DX" << endl <<
                                    "cmp AX, 0" << endl <<
                                    "jne " << j2 << endl <<
                                    j3 << ":" << endl <<
                                    "pop DX" << endl <<
                                    "add DX, 30h" << endl <<
                                    "mov DS:[DI], DL" << endl <<
                                    "add DI, 1" << endl <<
                                    "add CX, -1" << endl <<
                                    "cmp CX, 0" << endl <<
                                    "jne " << j3 << endl;

                            fout << "mov DL, 024h" << endl <<
                                    "mov DS:[DI], DL" << endl <<
                                    "mov DX, " << stringEnd << endl;
                        }else{
                            fout << "mov DX, " << endE << endl;          
                        }

                        if(tipoE != "char" && tamanhoE > 0){
                            mensagemErro(TIPOS_INCOMPATIVEIS, "");
                        }
                        while (tokenLido.token == ",")
                        {
                            casaToken(",");
                            procedimentoE(&tipoE, &tamanhoE, &endE);

                            string j1 = novoRot();
                            string j2 = novoRot();
                            string j3 = novoRot();

                            int stringEnd;
                            if(tipoE == "char"){
                                stringEnd = novoTemp("char", 255);

                                fout << "mov AX, DS:[" << endE << "]" << endl;

                                fout << "mov DI, " << stringEnd << endl <<
                                        "mov CX, 0" << endl <<
                                        "cmp AX, 0" << endl <<
                                        "jge " << j1 << endl <<
                                        "mov BL, 2Dh" << endl <<
                                        "mov DS:[DI], BL" << endl <<
                                        "add DI, 1" << endl <<
                                        "neg AX" << endl <<
                                        j1 << ":" << endl <<
                                        "mov BX, 10" << endl <<
                                        j2 << ":" << endl <<
                                        "add CX, 1" << endl <<
                                        "mov DX, 0" << endl <<
                                        "idiv BX" << endl <<
                                        "push DX" << endl <<
                                        "cmp AX, 0" << endl <<
                                        "jne " << j2 << endl <<
                                        j3 << ":" << endl <<
                                        "pop DX" << endl <<
                                        "add DX, 30h" << endl <<
                                        "mov DS:[DI], DL" << endl <<
                                        "add DI, 1" << endl <<
                                        "add CX, -1" << endl <<
                                        "cmp CX, 0" << endl <<
                                        "jne " << j3 << endl;

                                fout << "mov DL, 024h" << endl <<
                                        "mov DS:[DI], DL" << endl <<
                                        "mov DX, " << stringEnd << endl;
                            }else{
                                fout << "mov DX, " << endE << endl;          
                            }

                            fout << "mov AH, 09h" << endl <<
                                    "int 21h" << endl;

                            if(tipoE != "char" && tamanhoE > 0){
                                mensagemErro(TIPOS_INCOMPATIVEIS, "");
                            }
                        }

                        casaToken(")");
                        casaToken(";");
                        proxEndTemp = 0;
                    }
                    else
                    {
                        if (tokenLido.token == "writeln")
                        {
                            casaToken("writeln");
                            casaToken("(");
                            string tipoE;
                            int tamanhoE;
                            int endE;
                            procedimentoE(&tipoE, &tamanhoE, &endE);

                            string j1 = novoRot();
                            string j2 = novoRot();
                            string j3 = novoRot();

                            int stringEnd;
                            if(tipoE == "char"){
                                stringEnd = novoTemp("char", 255);

                                fout << "mov AX, DS:[" << endE << "]" << endl;

                                fout << "mov DI, " << stringEnd << endl <<
                                        "mov CX, 0" << endl <<
                                        "cmp AX, 0" << endl <<
                                        "jge " << j1 << endl <<
                                        "mov BL, 2Dh" << endl <<
                                        "mov DS:[DI], BL" << endl <<
                                        "add DI, 1" << endl <<
                                        "neg AX" << endl <<
                                        j1 << ":" << endl <<
                                        "mov BX, 10" << endl <<
                                        j2 << ":" << endl <<
                                        "add CX, 1" << endl <<
                                        "mov DX, 0" << endl <<
                                        "idiv BX" << endl <<
                                        "push DX" << endl <<
                                        "cmp AX, 0" << endl <<
                                        "jne " << j2 << endl <<
                                        j3 << ":" << endl <<
                                        "pop DX" << endl <<
                                        "add DX, 30h" << endl <<
                                        "mov DS:[DI], DL" << endl <<
                                        "add DI, 1" << endl <<
                                        "add CX, -1" << endl <<
                                        "cmp CX, 0" << endl <<
                                        "jne " << j3 << endl;

                                fout << "mov DL, 024h" << endl <<
                                        "mov DS:[DI], DL" << endl <<
                                        "mov DX, " << stringEnd << endl;
                            }else{
                                fout << "mov DX, " << endE << endl;          
                            }

                            fout << "mov AH, 09h" << endl <<
                                    "int 21h" << endl;

                            if(tipoE != "char" && tamanhoE > 0){
                                mensagemErro(TIPOS_INCOMPATIVEIS, "");
                            }
                            while (tokenLido.token == ",")
                            {
                                casaToken(",");
                                procedimentoE(&tipoE, &tamanhoE, &endE);
                                string j1 = novoRot();
                                string j2 = novoRot();
                                string j3 = novoRot();

                                int stringEnd;
                                if(tipoE == "char"){
                                    stringEnd = novoTemp("char", 255);

                                    fout << "mov AX, DS:[" << endE << "]" << endl;

                                    fout << "mov DI, " << stringEnd << endl <<
                                            "mov CX, 0" << endl <<
                                            "cmp AX, 0" << endl <<
                                            "jge " << j1 << endl <<
                                            "mov BL, 2Dh" << endl <<
                                            "mov DS:[DI], BL" << endl <<
                                            "add DI, 1" << endl <<
                                            "neg AX" << endl <<
                                            j1 << ":" << endl <<
                                            "mov BX, 10" << endl <<
                                            j2 << ":" << endl <<
                                            "add CX, 1" << endl <<
                                            "mov DX, 0" << endl <<
                                            "idiv BX" << endl <<
                                            "push DX" << endl <<
                                            "cmp AX, 0" << endl <<
                                            "jne " << j2 << endl <<
                                            j3 << ":" << endl <<
                                            "pop DX" << endl <<
                                            "add DX, 30h" << endl <<
                                            "mov DS:[DI], DL" << endl <<
                                            "add DI, 1" << endl <<
                                            "add CX, -1" << endl <<
                                            "cmp CX, 0" << endl <<
                                            "jne " << j3 << endl;

                                    fout << "mov DL, 024h" << endl <<
                                            "mov DS:[DI], DL" << endl <<
                                            "mov DX, " << stringEnd << endl;
                                }else{
                                    fout << "mov DX, " << endE << endl;          
                                }

                                fout << "mov AH, 09h" << endl <<
                                        "int 21h" << endl <<
                                        "mov AH, 02h" << endl <<
                                        "mov DL, 0Dh" << endl <<
                                        "int 21h" << endl <<
                                        "mov DL, 0Ah" << endl <<
                                        "int 21h" << endl;

                                if(tipoE != "char" && tamanhoE > 0){
                                    mensagemErro(TIPOS_INCOMPATIVEIS, "");
                                }
                            }
                            casaToken(")");
                            casaToken(";");
                            proxEndTemp = 0;
                        }
                        else
                        {
                            if (tokenLido.token == ";")
                            {
                                casaToken(";");
                            }
                            else
                            {
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
void procedimentoR()
{
    
    casaToken("id");
    if (tokenAnte.endereco->classe == "")
    {
        mensagemErro(ID_NAO_DECLARADO, "");
    }
    else
    {
        if (tokenAnte.endereco->classe == "const")
        {
            mensagemErro(CLASSE_INCOMPATIVEL, tokenAnte.lexema);
        }
    }
    string tipoR = tokenAnte.endereco->tipo;
    int tamanhoR = tokenAnte.endereco->tamanho;
    int endR = tokenAnte.endereco->end;
    bool array = false;

    string tipoE1;
    int tamanhoE1;
    int endE1;
    if (tokenLido.token == "[")
    {
        casaToken("[");
        if (tamanhoR == 0)
        {
            mensagemErro(TIPOS_INCOMPATIVEIS, "");
        }
        procedimentoE(&tipoE1, &tamanhoE1, &endE1);
        if (tipoE1 != "int" || tamanhoE1 > 0)
        {
            mensagemErro(TIPOS_INCOMPATIVEIS, "");
        }
        casaToken("]");
        tamanhoR = 0;
        array = true;
    }
    else
    {
        if (tipoR != "char" && tamanhoR != 0)
        {
            mensagemErro(TIPOS_INCOMPATIVEIS, "");
        }
    }
    casaToken(":=");
    string tipoE2;
    int tamanhoE2;
    int endE2;
    procedimentoE(&tipoE2, &tamanhoE2, &endE2);
    if (tipoE2 != tipoR)
    {
        mensagemErro(TIPOS_INCOMPATIVEIS, "");
    }
    else{
        if((tamanhoE2 != 0 && tamanhoR == 0) || (tamanhoE2 == 0 && tamanhoR != 0)){
            mensagemErro(TIPOS_INCOMPATIVEIS, "");
        }
        if(tamanhoE2 > tamanhoR){
            mensagemErro(TAM_VETOR_EXCEDE_MAX, "");
        }
    }

    if(tipoR == "char" && tamanhoR > 0){
        string rotIncio = novoRot();
        string rotCmd = novoRot();
        string rotInc = novoRot();
        string rotFim = novoRot();

        fout << "mov DI, " << endR << endl <<
                "mov SI, " << endE2 << endl <<
                rotIncio << ":" << endl <<
                "mov DX, DS:[DI]" << endl <<
                "cmp DX, 024h" << endl <<
                "je " << rotFim << endl <<
                "jmp " << rotCmd << endl <<
                rotInc << ":" << endl <<
                "add DI, 1" << endl <<
                "add SI, 1" << endl <<
                "jmp " << rotIncio << endl <<
                rotCmd << ":" << endl <<
                "mov DX, DS:[SI]" << endl <<
                "mov DS:[DI], DX" << endl <<
                "jmp " << rotInc << endl <<
                rotFim << ":" << endl;
    }
    else{
        fout << "mov AX, DS:[" << endE2 << "]" << endl;
        if(array){
            fout << "mov SI, DS:[" << endE1 << "]" << endl;
            if(tipoR == "int"){
                fout << "add SI, SI" << endl; 
            }
            fout << "add SI, " << endR << endl <<
                    "mov DS:[SI], AX" << endl; 
        }
        else{
            fout << "mov DS:[" << endR << "], AX" << endl;
        }
    }
    proxEndTemp = 0;
}

//A-> [R{,R}]
void procedimentoA()
{
    if (tokenLido.token == "id" || tokenLido.token == "for" || tokenLido.token == "if" || tokenLido.token == "readln" || tokenLido.token == "write" || tokenLido.token == "writeln")
    {
        procedimentoR();
        while (tokenLido.token == ",")
        {
            casaToken(",");
            procedimentoR();
        }
    }
}

//* B-> C | '{'{C}'}'
void procedimentoB()
{
    if (tokenLido.token == "{")
    {
        casaToken("{");
        procedimentoC();
        while (tokenLido.token == "id" || tokenLido.token == "for" || tokenLido.token == "if" || tokenLido.token == ";" || tokenLido.token == "readln" || tokenLido.token == "write" || tokenLido.token == "writeln")
        {
            procedimentoC();
        }
        casaToken("}");
    }
    else
    {
        if (tokenLido.token == "id" || tokenLido.token == "for" || tokenLido.token == "if" || tokenLido.token == ";" || tokenLido.token == "readln" || tokenLido.token == "write" || tokenLido.token == "writeln")
        {
            procedimentoC();
        }
        else
        {
            mensagemErro(TOKEN_INESPERADO, tokenLido.lexema);
        }
    }
}

//V-> id['['E']']
void procedimentoV()
{
    casaToken("id");
    if (tokenAnte.endereco->classe == "")
    {
        mensagemErro(ID_NAO_DECLARADO, tokenAnte.lexema);
    }
    else{
        if(tokenAnte.endereco->classe != "var"){
            mensagemErro(CLASSE_INCOMPATIVEL, tokenAnte.lexema);
        }
    }
    string tipoV = tokenAnte.endereco->tipo;
    int tamanhoV = tokenAnte.endereco->tamanho;
    int endV = tokenAnte.endereco->end;

    string tipoE;
    int tamanhoE;
    int endE;

    int buffer = novoTemp("char", 255);
    bool array = false;
    if (tokenLido.token == "[")
    {
        casaToken("[");
        if (tamanhoV == 0)
        {
            mensagemErro(TIPOS_INCOMPATIVEIS, "");
        }

        procedimentoE(&tipoE, &tamanhoE, &endE);
        if (tipoE != "int" || tamanhoE > 0)
        {
            mensagemErro(TIPOS_INCOMPATIVEIS, "lexemaV");
        }
        casaToken("]");
        array = true;
    }
    else{
        if(tipoV != "char" && tamanhoV > 0){
            mensagemErro(TIPOS_INCOMPATIVEIS, "");
        }
    }

    fout << "mov DX, " << buffer << endl <<
            "mov AL, 0FFh" << endl <<
            "mov DS:[" << buffer << "], AL" << endl <<
            "mov AH, 0Ah" << endl <<
            "int 21h" << endl <<
            "mov AH, 02h" << endl <<
            "mov DL, 0Dh" << endl <<
            "int 21h" << endl;

    if(tipoV == "char" && tamanhoV >0){
        string rotIncio = novoRot();
        string rotCmd = novoRot();
        string rotInc = novoRot();
        string rotFim = novoRot();

        fout << "mov DI, " << endV << endl <<
                "mov SI, " << buffer << endl <<
                rotIncio << ":" << endl <<
                "mov DX, DS:[DI]" << endl <<
                "cmp DX, 0Ah" << endl <<
                "je " << rotFim << endl <<
                "jmp " << rotCmd << endl <<
                rotInc << ":" << endl <<
                "add DI, 1" << endl <<
                "add SI, 1" << endl <<
                "jmp " << rotIncio << endl <<
                rotCmd << ":" << endl <<
                "mov DX, DS:[SI]" << endl <<
                "mov DS:[DI], DX" << endl <<
                "jmp " << rotInc << endl <<
                rotFim << ":" << endl << 
                "mov DS:[DI], 024h" << endl;
    }
    else{
        if(tipoV == "char"){
            fout << "mov DI, " << (buffer+2) << endl <<
                    "mov AX, DS:[DI]";
            if(array){
                fout << "mov SI, DS:[" << endE << "]" << endl <<
                        "add SI, " << endV << endl <<
                        "mov DS:[SI], AX" << endl;
            }else{
                fout << "mov DS:[" << endV << "], AX" << endl;
            }
        }
        else{
            string j1 = novoRot();
            string j2 = novoRot();
            string j3 = novoRot();

            fout << "mov DI, " << (buffer+2) << endl <<
                    "mov AX, 0" << endl <<
                    "mov CX, 10" << endl << 
                    "mov DX, 1" << endl << 
                    "mov BH, 0" << endl <<
                    "mov BL, DS:[DI]" << endl << 
                    "cmp BX, 2Dh" << endl <<
                    "jne " << j1 << endl << 
                    "mov DX, -1" << endl <<
                    "add DI, 1" << endl <<
                    "mov BL, DS:[DI]" << endl <<
                    j1 << ":" << endl <<
                    "push DX" << endl <<
                    "mov DX, 0" << endl <<
                    j2 << ":" << endl <<
                    "cmp BX, 0Dh" << endl <<
                    "je " << j3 << endl <<
                    "imul CX" << endl <<
                    "add BX, -48" << endl <<
                    "add AX, BX" << endl <<
                    "add DI, 1" << endl <<
                    "mov BH, 0" << endl <<
                    "mov BL, DS:[DI]" << endl <<
                    "jmp " << j2 << endl <<
                    j3 << ":" << endl <<
                    "pop CX" << endl <<
                    "imul CX" << endl;

            if(array){
                fout << "mov SI, DS:[" << endE << "]" << endl <<
                        "add SI, SI" << endl <<
                        "add SI, " << endV << endl <<
                        "mov DS:[SI], AX" << endl;
            }else{
                fout << "mov DS:[" << endV << "], AX" << endl;
            }
        }
    }

}

//Expressoes
//E-> F [ (=|<>|<|>|<=|>=) F]
void procedimentoE(string *tipoE, int *tamanhoE, int *endE)
{
    *tipoE = "";
    *tamanhoE = 0;

    string tipoF1;
    int tamanhoF1;
    int endF1;
    procedimentoF(&tipoF1, &tamanhoF1, &endF1);
    *tipoE = tipoF1;
    *tamanhoE = tamanhoF1;
    *endE = tamanhoF1;

    string tipoF2;
    int tamanhoF2;
    int endF2;
    if (tokenLido.token == "=" || tokenLido.token == "<>" || tokenLido.token == "<" || tokenLido.token == ">" || tokenLido.token == "<=" || tokenLido.token == ">=")
    {
        string rotVerdadeiro;
        rotVerdadeiro = novoRot();
        if (tokenLido.token == "=")
        {
            casaToken("=");
            procedimentoF(&tipoF2, &tamanhoF2, &endF2);
            fout << "mov AX, DS:[" << *endE << "]" << endl <<
                    "mov BX, DS:[" << endF2 << "]" << endl <<
                    "mov AH, 0" << endl << 
                    "mov BH, 0" << endl;
            if (*tipoE != tipoF2)
            {
                mensagemErro(TIPOS_INCOMPATIVEIS, "");
            }
            else
            {
                if (*tipoE == "char")
                { //checa comparacao de array de chars (string)
                    if ((*tamanhoE > 0 && tamanhoF2 == 0) || (*tamanhoE == 0 && tamanhoF2 > 0))
                    {
                        mensagemErro(TIPOS_INCOMPATIVEIS, "");
                    }
                }
                else
                {
                    if (*tamanhoE != 0 || tamanhoF2 != 0)
                    { //checa array de tipo != char
                        mensagemErro(TIPOS_INCOMPATIVEIS, "");
                    }
                }
            }
            if(*tipoE != "char" || *tamanhoE == 0){
                fout << "cmp AX, BX" << endl;
            }
            else{ //comparando strings

                string rotInicio = novoRot();
                string rotCmd = novoRot();
                string rotInc = novoRot();
                string rotTrue = novoRot();
                string rotFalse = novoRot();
                string rotFim = novoRot();
                string rotVerifica = novoRot();
                string rotVerifica2 = novoRot();
                string j1 = novoRot();
                string j2 = novoRot();
                string j3 = novoRot();
                string j4 = novoRot();

                fout << "mov DI, " << *endE << endl <<
                        "mov SI, " << endF2 << endl <<
                        "mov CX, " << (*endE+*tamanhoE) << endl <<
                        "mov DX, " << (endF2+tamanhoF2) << endl <<
                        rotInicio << ":" << endl <<
                        "cmp DI, CX" << endl <<
                        "jl " << j2 << endl <<
                        "jmp " << rotVerifica <<
                        j2 << ":" << endl <<
                        "cmp SI, DX" << endl <<
                        "jl " << j3 << endl <<
                        "jmp " << rotVerifica << endl <<
                        j3 << ":" << endl <<
                        "mov BX, DS:[DI]" << endl <<
                        "cmp BX, 024h" << endl <<
                        "jne " << j4 << endl <<
                        "jmp " << rotVerifica << endl <<
                        j4 << ":" << endl <<
                        "mov BX, DS:[SI]" << endl <<
                        "cmp BX, 024h" << endl <<
                        "jne " << rotCmd << endl <<
                        "jmp " << rotVerifica << endl <<
                        rotInc << ":" << endl <<
                        "add DI, 1" << endl <<
                        "add SI, 1" << endl <<
                        "jmp " << rotInicio << endl <<
                        rotCmd << ":" << endl <<
                        "mov AX, DS:[DI]" << endl <<
                        "mov BX, DS:[SI]" << endl <<
                        "cmp AX, BX" << endl <<
                        "jne " << rotFalse << endl <<
                        "jmp " << rotInc << endl <<
                        rotVerifica << ":" << endl <<
                        "cmp DI, CX" << endl <<
                        "je " << rotVerifica2 << endl <<
                        "mov BX, DS:[DI]" << endl <<
                        "cmp BX, 024h" << endl <<
                        "jne " << rotFalse << endl <<
                        rotVerifica2 << ":" << endl <<
                        "cmp SI, DX" << endl <<
                        "je " << rotTrue << endl <<
                        "mov BX:DS[SI]" << endl <<
                        "cmp BX, 024h" << endl <<
                        "jne " << rotFalse << endl <<
                        rotTrue << ":" << endl <<
                        "mov AX, 1" << endl <<
                        "jmp " << rotFim << endl <<
                        rotFalse << ":" << endl <<
                        "mov AX, 0" << endl <<
                        rotFim << ":" << endl;
            }
            fout << "je " << rotVerdadeiro << endl;

        }
        else
        {
            casaToken(tokenLido.token);
            procedimentoF(&tipoF2, &tamanhoF2, &endF2);
            fout << "mov AX, DS:[" << *endE << "]" << endl <<
                    "mov BX, DS:[" << endF2 << "]" << endl <<
                    "mov AH, 0" << endl << 
                    "mov BH, 0" << endl;
                    //TODO
            if (*tipoE != tipoF2)
            {
                mensagemErro(TIPOS_INCOMPATIVEIS, "");
            }
            if (*tamanhoE != 0 || tamanhoF2 != 0)
            {
                mensagemErro(TIPOS_INCOMPATIVEIS, "");
            }
            fout << "cmp AX, BX" << endl;
            if(tokenLido.token == "<>"){
                fout << "jne " << rotVerdadeiro << endl;
            }
            else{
                if(tokenLido.token == "<"){
                    fout << "jl " << rotVerdadeiro << endl;
                }
                else{
                    if(tokenLido.token == ">"){
                        fout << "jg " << rotVerdadeiro << endl;
                    }
                    else{
                        if(tokenLido.token == ">="){
                            fout << "jge " << rotVerdadeiro << endl;
                        }
                        else{ // <=
                            fout << "jle " << rotVerdadeiro << endl;
                        }
                    }
                }
            }
            
        }
        string rotFim = novoRot();
        fout << "mov AX, 0" << endl <<
                "jmp " << rotFim << endl <<
                rotVerdadeiro << ":" << endl <<
                "mov AX, 1" << endl <<
                rotFim << ":" << endl;

        *tipoE = "boolean";
        *tamanhoE = 0;
        *endE = novoTemp(*tipoE, *tamanhoE);

        fout << "mov DS:[" << *endE << "], AX" << endl; 

    }
}

//F-> [+|-] G {(+|-|or) G}
void procedimentoF(string *tipoF, int *tamanhoF, int *endF)
{

    *tipoF = "";
    *tamanhoF = 0;

    bool temSinal = false;
    bool neg = false;
    if (tokenLido.token == "+" || tokenLido.token == "-")
    {
        casaToken(tokenLido.token);
        temSinal = true;
        if(tokenLido.token == "-"){
            neg = true;
        }

    }

    string tipoG1;
    int tamanhoG1;
    int endG1;
    procedimentoG(&tipoG1, &tamanhoG1, &endG1);
    if (temSinal && (tipoG1 != "int" || tamanhoG1 != 0))
    {
        mensagemErro(TIPOS_INCOMPATIVEIS, "");
    }
    *tipoF = tipoG1;
    *tamanhoF = tamanhoG1;
    if(neg){
        *endF = novoTemp(*tipoF, *tamanhoF);
        fout << "mov AX, DS:[" << endG1 << "]" << endl <<
                "neg AX" << endl <<
                "add AX,1" << endl <<
                "mov DS:[" << *endF << "[, AX" << endl;
                
    }
    else{
        *endF = endG1;
    }


    while (tokenLido.token == "+" || tokenLido.token == "-" || tokenLido.token == "or")
    {
        string oper = tokenLido.token;
        casaToken(tokenLido.token);

        string tipoG2;
        int tamanhoG2;
        int endG2;
        procedimentoG(&tipoG2, &tamanhoG2, &endG2);
        
        fout << "mov AX, DS:[" << *endF << "]" << endl <<
                "mov BX, DS:[" << endG2 << "]" << endl;

        if (oper == "or")
        {
            if (*tipoF != "boolean" || tipoG2 != "boolean")
            {
                mensagemErro(TIPOS_INCOMPATIVEIS, "");
            }
            fout << "mov CX, AX" << endl <<
                    "add CX, BX" << endl <<
                    "imul BX" << endl <<
                    "sub CX, AX" << endl <<
                    "mov AX, CX" << endl;
        }
        else
        {
            if (*tipoF != "int" || tipoG2 != "int")
            {
                mensagemErro(TIPOS_INCOMPATIVEIS, "");
            }

            if(oper == "-"){
                fout << "sub AX, BX" << endl;
            }
            else{
                fout << "add AX, BX" << endl;
            }
        }
        *endF = novoTemp(*tipoF, *tamanhoF);
        fout << "mov DS:[" << *endF << "], AX" << endl;
    }
}

//G-> H {(*|and|/|%) H}
void procedimentoG(string *tipoG, int *tamanhoG, int *endG)
{
    *tipoG = "";
    *tamanhoG = 0;

    string tipoH1;
    int tamanhoH1;
    int endH1;
    procedimentoH(&tipoH1, &tamanhoH1, &endH1);
    *tipoG = tipoH1;
    *tamanhoG = tamanhoH1;
    *endG = endH1;

    while (tokenLido.token == "*" || tokenLido.token == "and" || tokenLido.token == "/" || tokenLido.token == "%")
    {
        string oper = tokenLido.token;
        casaToken(tokenLido.token);

        string tipoH2;
        int tamanhoH2;
        int endH2;
        procedimentoH(&tipoH2, &tamanhoH2, &endH2);
        fout << "mov AX, DS:[" << *endG << "]" << endl <<
                "mov BX, DS:[" << endH2 << "]" << endl;

        if (*tamanhoG != 0 || tamanhoH2 != 0)
        {
            mensagemErro(TIPOS_INCOMPATIVEIS, "");
        }
        if (oper == "and")
        {
            if (*tipoG != "boolean" || tipoH2 != "boolean")
            {
                mensagemErro(TIPOS_INCOMPATIVEIS, "");
            }
            fout << "imul BX" << endl;
        }
        else
        {
            if (*tipoG != "int" || tipoH2 != "int")
            {
                mensagemErro(TIPOS_INCOMPATIVEIS, "");
            }

            if(oper == "*"){
                fout << "imul BX" << endl;
            }
            else{
                if("/"){
                    fout << "cwd" << endl <<
                            "idiv BX" << endl;
                }
                else{
                    fout << "cwd" << endl <<
                            "idiv BX" << endl <<
                            "mov AX, DX" << endl;
                }
            }
        }
        *endG = novoTemp(*tipoG, *tamanhoG);
        fout << "mov DS:[" << *endG << "], AX" << endl;
    }
}

//H-> id ['['E']'] | const | not H | '('E')'
void procedimentoH(string *tipoH, int *tamanhoH, int *endH)
{

    *tipoH = "";
    *tamanhoH = 0;

    if (tokenLido.token == "id")
    {
        casaToken("id");
        if (tokenAnte.endereco->classe == "")
        {
            mensagemErro(ID_NAO_DECLARADO, tokenAnte.lexema);
        }
        *tipoH = tokenAnte.endereco->tipo;
        *tamanhoH = tokenAnte.endereco->tamanho;
        int enderecoID = tokenAnte.endereco->end;
        if (tokenLido.token == "[")
        {
            casaToken("[");

            if (*tamanhoH == 0)
            {
                mensagemErro(TIPOS_INCOMPATIVEIS, "");
            }
            string tipoE1;
            int tamanhoE1;
            int endE1;
            procedimentoE(&tipoE1, &tamanhoE1, &endE1);
            if (tipoE1 != "int" || tamanhoE1 > 0)
            {
                mensagemErro(TIPOS_INCOMPATIVEIS, "");
            }

            casaToken("]");
            *tamanhoH = 0;
            *endH = novoTemp(*tipoH, *tamanhoH);
            fout << "mov SI, DS:[" << endE1 << "]" << endl;
            if(*tipoH != "char"){
                fout << "add SI, SI";
            }
            fout << "add SI, " << enderecoID << endl <<
                "mov BX, DS:[SI]" << endl <<
                "mov DS:[" << *endH << "], BX" << endl;
        }
        else
        {
            if (*tipoH != "char" && *tamanhoH > 0)
            {
                mensagemErro(TIPOS_INCOMPATIVEIS, "");
            }
            *endH = enderecoID;
        }
    }
    else
    {
        if (tokenLido.token == "const")
        {
            casaToken("const");
            *tipoH = tokenAnte.tipo;
            *tamanhoH = tokenAnte.tamanho;
            if(*tipoH == "char" && *tamanhoH > 0){
                fout << "dseg SEGMENT PUBLIC" << endl <<
                    "byte " << tokenAnte.lexema.substr(0, tokenAnte.lexema.size()-1) << "$\"" << endl <<
                    "dseg ENDS" << endl;
                *endH = proxEndereco;
                proxEndereco += *tamanhoH + 1;
            }
            else{
                *endH = novoTemp(*tipoH, *tamanhoH);
                if(tokenAnte.lexema == "\'\\n\'"){
                    fout << "mov AX, " << 10 << endl <<
                            "mov DS:[" << *endH << "], AX " << endl;
                }else{ 
                    if(tokenAnte.lexema == "\'\\r\'"){
                        fout << "mov AX, " << 13 << endl <<
                                "mov DS:[" << *endH << "], AX " << endl;
                    }
                    else{
                        if(tokenAnte.lexema == "TRUE"){
                            fout << "mov AX, " << 1 << endl <<
                                "mov DS:[" << *endH << "], AX " << endl;
                        }
                        else{
                            if(tokenAnte.lexema == "FALSE"){
                                fout << "mov AX, " << 0 << endl <<
                                    "mov DS:[" << *endH << "], AX " << endl;
                            }
                            else{
                                fout << "mov AX, " << tokenAnte.lexema << endl <<
                                        "mov DS:[" << *endH << "], AX " << endl;
                            }
                        }
                    }
                }
            }
        }   
        else
        {
            if (tokenLido.token == "not")
            {
                casaToken("not");
                string tipoH1;
                int tamanhoH1;
                int endH1;
                procedimentoH(&tipoH1, &tamanhoH1, &endH1);
                if (tipoH1 != "boolean")
                {
                    mensagemErro(TIPOS_INCOMPATIVEIS, "");
                }
                *tipoH = tipoH1;
                *tamanhoH = tamanhoH1;
                *endH = novoTemp(*tipoH, *tamanhoH);
                fout << "mov AX, DS:[" << endH1 << "]" << endl <<
                        "neg AX" << endl <<
                        "add AX, 1" << endl <<
                        "mov DS:[" << *endH << "], AX" << endl;
            }
            else
            {
                if (tokenLido.token == "(")
                {
                    casaToken("(");
                    string tipoE2;
                    int tamanhoE2;
                    int endE2;
                    procedimentoE(&tipoE2, &tamanhoE2, &endE2);
                    *tipoH = tipoE2;
                    *tamanhoH = tamanhoE2;
                    *endH = endE2;
                    casaToken(")");
                }
            }
        }
    }
}

int main()
{
    stat = EXECUTANDO;
    analisadorSintatico();
    if (!(stat == ERRO))
    {
        cout << linha << " linhas compiladas.";
    }
    return 0;
}

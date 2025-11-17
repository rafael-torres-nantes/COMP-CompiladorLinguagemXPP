# Compilador X++ - Análise Léxica, Sintática e Semântica

## 👨‍💻 Projeto desenvolvido por: [Rafael Torres Nantes](https://github.com/rafael-torres-nantes) e [Sarah Merigue Baraldi](https://github.com/mbaraldi-sarah).

## Índice

* [📚 Contextualização do Projeto](#-contextualização-do-projeto)
* [🛠️ Tecnologias/Ferramentas utilizadas](#%EF%B8%8F-tecnologiasferramentas-utilizadas)
* [🖥️ Funcionamento do sistema](#%EF%B8%8F-funcionamento-do-sistema)
   * [🔍 Etapa 1 - Analisador Léxico](#-etapa-1---analisador-léxico)
   * [📋 Etapa 2 - Analisador Sintático](#-etapa-2---analisador-sintático)
   * [🧮 Etapa 3 - Tabela de Símbolos e Análise Semântica](#-etapa-3---tabela-de-símbolos-e-análise-semântica)
* [📌 Como executar o projeto](#-como-executar-o-projeto)

## 📚 Contextualização do Projeto

O projeto consiste em um compilador completo capaz de analisar o léxico, a sintaxe e a semântica de um código na linguagem fictícia X++, uma linguagem orientada a objetos simplificada baseada em Java. O programa foi desenvolvido utilizando a linguagem C++ e implementa todas as fases de um compilador front-end.

## 🛠️ Tecnologias/Ferramentas utilizadas

[<img src="https://img.shields.io/badge/Visual_Studio_Code-007ACC?">](https://code.visualstudio.com/)
[<img src="https://img.shields.io/badge/GitHub-181717?logo=github&logoColor=violet">](https://github.com/)
[<img src="https://img.shields.io/badge/-C++-blue?logo=cplusplus">](https://isocpp.org/)


## 🖥️ Funcionamento do sistema

### 🔍 Etapa 1 - Analisador Léxico

Nessa primeira etapa, foi implementada a Análise Léxica nos arquivos `scanner.h` e `scanner.cpp`. Onde no primeiro está declarada a classe _Scanner_ e no segundo o seu funcionamento. A implementação é como uma máquina de estados onde os tokens são verificados.

Além disso, o arquivo `token.h` contém a classe _Token_ onde estão definidos os tokens (_TokenTypes_) da linguagem. A seguir, estão apresentadas as **definições regulares dos tokens**, nas quais os símbolos terminais estão destacados em **negrito**:

1. _letter_ → **[a-zA-Z_]**
2. _digit_ → **[0-9]**
3. _ID_ → _letter_ (_letter_ | _digit_)**∗**
4. _INTEGER_LITERAL_ → _digit_**+**
5. _STRING_LITERAL_ → **"** (_ch_)**∗** **"**, onde _ch_ é qualquer caractere ASCII imprimível exceto aspas duplas e **\n**
6. _Operadores_ → **+** | **-** | **∗** | **/** | **%** | **=** | **==** | **!=** | **<=** | **<** | **>=** | **>**
7. _Separadores_ → **(** | **)** | **{** | **}** | **[** | **]** | **,** | **;** | **.**
8. _Comentários_ devem ser **ignorados**. Comentário de linha é iniciado por **//** e encerrado por nova linha. Comentário em bloco é delimitado por **/\*** e **\*/** sem aninhamento.
9. _Espaços em branco_ devem ser **ignorados**.
10. _Palavras Reservadas_: **class**, **extends**, **int**, **string**, **break**, **print**, **read**, **return**, **super**, **if**, **else**, **for**, **new**, **constructor**

#### Um pouco da implementação...

`scanner.h` - Declaração da classe _Scanner_.
```h
#include "superheader.h"

class Scanner 
{
    private: 
        string input;   
        int pos;        
        int line;       
        SymbolTable* symbolTable; 
    
    public:
        // Construtor
        Scanner(string, SymbolTable*);    // Arquivo de entrada e tabela de simbolos

        int getLine();    
    
        // Metodo que retorna o proximo token da entrada
        Token* nextToken();        
    
        // Metodo para manipular erros
        void lexicalError();
};
```

`scanner.cpp` - Funcionamento da classe _Scanner_.
```cpp
// Método que retorna o próximo token da entrada
// Método que retorna o próximo token da entrada
Token* Scanner::nextToken()
{
    Token* token;
    int state = 0;
    string lexeme;

    while (true)
    {
        switch (state)
        {
        case 0:
            if (input[pos] == '\0')
            {
                token = new Token(END_OF_FILE);
                return token;
            }
            else if (input[pos] == '<')
                state = 5;
            else if (input[pos] == '=')
                state = 13;
            else if (input[pos] == '>')
                state = 6;
            else if (input[pos] == '*')
                state = 7;
            else if (input[pos] == '-')
                state = 8;
            else if (input[pos] == '+')
[...]

case 3: // Dígito
    if (isdigit(input[pos])){
        lexeme += input[pos];
        pos++;
    }
    else
        state = 4;
     break;

case 4: // Integer Literal
    if (isalpha(input[pos]) || input[pos] == '_') {
        lexicalError();
    }
    token = new Token(INTEGER_LITERAL, lexeme);
    return token;

case 5: // <
    if (input[pos] == '=') {
        pos++;
        token = new Token(LESS_OR_EQUAL_THAN);
        return token;
    }
    else {
        token = new Token(LESS_THAN);
        return token;
    }
[...]
```

`token.h` - Classe _Token_ e TokenTypes.
```h
enum TokenType 
{
    UNDEFINED,               // 0  - Token indefinido
    ID,                      // 1  - Identificador (ex: nomes de variáveis)
    INTEGER_LITERAL,         // 2  - Literal inteiro (dígito+)
    STRING_LITERAL,          // 3  - Literal string delimitado por " e "

    OP,                      // 4  - Operador

[...]

class Token {
    public: 
        int type;       // Tipo do token
        int attribute;  
        string lexeme;  // Texto reconhecido (lexema)
    
        // Construtores para diferentes tipos de tokens

        // Apenas tipo
        Token(int type) // Exemplo: if, else.
        {
            this->type = type;
            attribute = UNDEFINED;
            lexeme = "";
        }

        // Tipo e lexema
        Token(int type, string lexeme)
        {
            this->type = type;
            attribute = UNDEFINED;
            this->lexeme = lexeme;
        }
```

### 📋 Etapa 2 - Analisador Sintático

O analisador sintático consiste em verificar se o código está seguindo as **regras de gramática** da linguagem X++. Ele foi implementado nos arquivos `parser.h` e `parser.cpp`, onde no primeiro está definida a classe _Parser_ e no segundo, suas funções.

Na tabela a seguir está descrita a gramática da linguagem X++. Os tokens da linguagem são representados em **negrito** e os não-terminais em _itálico_:

| Produção | Corpo |
|----------|-------|
| _Program_ → | _ClassList_ |
| _ClassList_ → | _ClassDecl_ _ClassList_ \| _ClassDecl_ |
| _ClassDecl_ → | **class ID** _ClassBody_ \| **class ID extends ID** _ClassBody_ |
| _ClassBody_ → | **{** _VarDeclListOpt_ _ConstructDeclListOpt_ _MethodDeclListOpt_ **}** |
| _VarDeclListOpt_ → | _VarDeclList_ \| _ε_ |
| _VarDeclList_ → | _VarDeclList_ _VarDecl_ \| _VarDecl_ |
| _VarDecl_ → | _Type_ **ID** _VarDeclOpt_ **;** \| _Type_ **[ ] ID** _VarDeclOpt_ **;** |
| _VarDeclOpt_ → | **, ID** _VarDeclOpt_ \| _ε_ |
| _Type_ → | **int** \| **string** \| **ID** |
| _ConstructDeclListOpt_ → | _ConstructDeclList_ \| _ε_ |
| _ConstructDeclList_ → | _ConstructDeclList_ _ConstructDecl_ \| _ConstructDecl_ |
| _ConstructDecl_ → | **constructor** _MethodBody_ |
| _MethodDeclListOpt_ → | _MethodDeclList_ \| _ε_ |
| _MethodDeclList_ → | _MethodDeclList_ _MethodDecl_ \| _MethodDecl_ |
| _MethodDecl_ → | _Type_ **ID** _MethodBody_ \| _Type_ **[ ] ID** _MethodBody_ |
| _MethodBody_ → | **( **_ParamListOpt_** ) {** _StatementsOpt_ **}** |
| _ParamListOpt_ → | _ParamList_ \| _ε_ |
| _ParamList_ → | _ParamList_ **, **_Param_ \| _Param_ |
| _Param_ → | _Type_ **ID** \| _Type_ **[ ] ID** |
| _StatementsOpt_ → | _Statements_ \| _ε_ |
| _Statements_ → | _Statements_ _Statement_ \| _Statement_ |
| _Statement_ → | _VarDeclList_ \| _AtribStat_ **;** \| _PrintStat_ **;** \| _ReadStat_ **;** \| _ReturnStat_ **;** \| _SuperStat_ **;** \| _IfStat_ \| _ForStat_ \| **break ;** \| **;** |
| _AtribStat_ → | _LValue_ **=** _Expression_ \| _LValue_ **=** _AllocExpression_ |
| _PrintStat_ → | **print** _Expression_ |
| _ReadStat_ → | **read** _LValue_ |
| _ReturnStat_ → | **return** _Expression_ |
| _SuperStat_ → | **super (** _ArgListOpt_ **)** |
| _IfStat_ → | **if (** _Expression_ **) {** _Statements_ **}** \| **if (** _Expression_ **) {** _Statements_ **} else {** _Statements_ **}** |
| _ForStat_ → | **for (** _AtribStatOpt_ **;** _ExpressionOpt_ **;** _AtribStatOpt_ **) {** _Statements_ **}** |
| _LValue_ → | **ID** _LValueComp_ |
| _LValueComp_ → | **. ID** _LValueComp_ \| **. ID [** _Expression_ **]** _LValueComp_ \| **. ID (** _ArgListOpt_ **)** _LValueComp_ \| **[** _Expression_ **]** _LValueComp_ \| _ε_ |
| _Expression_ → | _NumExpression_ \| _NumExpression_ _RelOp_ _NumExpression_ |
| _AllocExpression_ → | **new ID (** _ArgListOpt_ **)** \| _Type_ **[** _Expression_ **]** |
| _NumExpression_ → | _Term_ **+** _Term_ \| _Term_ **-** _Term_ \| _Term_ |
| _Term_ → | _UnaryExpression_ **\*** _UnaryExpression_ \| _UnaryExpression_ **/** _UnaryExpression_ \| _UnaryExpression_ **%** _UnaryExpression_ \| _UnaryExpression_ |
| _UnaryExpression_ → | **+** _Factor_ \| **-** _Factor_ \| _Factor_ |
| _Factor_ → | **INTEGER_LITERAL** \| **STRING_LITERAL** \| _LValue_ \| **(** _Expression_ **)** |
| _ArgListOpt_ → | _ArgList_ \| _ε_ |
| _ArgList_ → | _ArgList_ **,** _Expression_ \| _Expression_ |

A gramática foi implementada usando análise descendente recursiva (top-down parsing), onde cada não-terminal possui um método correspondente no parser.

#### Um pouco da implementação...

`parser.h` - Definição da Classe _Parser_
```h
class Parser {
public:
    // Construtor que inicializa o scanner com o arquivo de entrada e a tabela de símbolos
    Parser(string input, SymbolTable* st);

    // Método para iniciar o processo de parsing
    void run();

private:
    Scanner* scanner;         // Objeto Scanner para tokenizar a entrada
    Token* lToken;            // Token atual
    SymbolTable* symbolTable; // Tabela de símbolos para análise semântica
    SymbolTable* currentScope; // Escopo atual (para escopos aninhados)
    string currentClass;      // Nome da classe atual sendo processada
    string currentType;       // Tipo atual sendo processado
    bool currentIsArray;      // Se o tipo atual é array

    // Avança para o próximo token
    void advance();

    // Verifica se o token atual corresponde ao tipo esperado e avança
    void match(int t);

    // Métodos das produções gramaticais para a linguagem X++
    void Program();              // Program → ClassList
[...]
```

`parser.cpp` - Implementação das funções da Classe _Parser_.
```cpp
[...]
void Parser::run() {
    try {
        Program();
        cout << "\n[SUCESSO] Compilacao finalizada com sucesso." << endl;
    } catch (const runtime_error& e) {
        cout << e.what() << endl;
    }
}

void Parser::advance() {
    lToken = scanner->nextToken();
[...]
// Regra 1: Program → ClassList
void Parser::Program() {
    ClassList();
    match(END_OF_FILE);
}

// Regra 2: ClassList → ClassDecl ClassList | ClassDecl
void Parser::ClassList() {
    ClassDecl();
    if (lToken->type == CLASS) {
        ClassList();
    }
}
[...]
```

### 🧮 Etapa 3 - Tabela de Símbolos e Análise Semântica

Implementamos uma tabela de símbolos para **guardar** tanto as **palavras reservadas** quanto **variáveis** e **classes** criadas pelo usuário. Sua implementação se deu principalmente nos arquivos `stentry.h`, `stentry.cpp`, `symboltable.h`, `symboltable.cpp`, além de alterações nos arquivos da análise sintática.

**Além do requisitado**, foi implementada **análise semântica completa** como bonificação, incluindo:
- ✅ Verificação de declaração e uso de variáveis
- ✅ Verificação de declaração de classes
- ✅ Verificação de herança (classe pai existe)
- ✅ Controle de escopos hierárquicos (global → classe → método → bloco)
- ✅ Detecção de redeclarações
- ✅ Validação de tipos

#### Um pouco da implementação...

`stentry.h` - Declaração da classe _STEntry_.
```h
class STEntry {
public:
    Token* token;           // Ponteiro para o token associado.
    bool reserved;          // Indica se o símbolo é uma palavra reservada.
    SymbolKind kind;        // Tipo do símbolo (classe, variável, método, etc.)
    string type;            // Tipo do símbolo (int, string, nome de classe)
    bool isArray;           // Indica se é um array
    string parentClass;     // Para classes: classe pai (se houver herança)
    int line;               // Linha onde foi declarado

    // Construtores para criar uma entrada de símbolo com diferentes configurações.
    STEntry(); 
    STEntry(Token*);   
    STEntry(Token*, bool);
    STEntry(Token*, SymbolKind, string type = "", bool isArray = false, int line = 0);
};
```

`stentry.cpp` - Construtores da classe _STEntry_.
```cpp
// Construtor padrão que inicializa uma entrada de símbolo sem associar um token.
STEntry::STEntry() {
    token = nullptr;
    reserved = false;
    kind = KEYWORD;
    type = "";
    isArray = false;
    parentClass = "";
    line = 0;
}

// O símbolo não é marcado como reservado por padrão.
STEntry::STEntry(Token* tok) {
    token = tok;
    reserved = false;
    kind = VARIABLE;
    type = "";
    isArray = false;
    parentClass = "";
    line = 0;
}

// Construtor que inicializa uma entrada de símbolo com um token e uma flag de reserva.
STEntry::STEntry(Token* tok, bool res) {
    token = tok;
    reserved = res;
    kind = res ? KEYWORD : VARIABLE;
    type = "";
    isArray = false;
    parentClass = "";
    line = 0;
}
```

`symboltable.h` - Declaração da classe _SymbolTable_.
```h
class SymbolTable {
public:
    SymbolTable* parent; // Referência à tabela pai (escopo imediatamente anterior).
    std::map<std::string, STEntry*> symbols; // Armazena os símbolos do escopo atual.

    // Construtores para criar tabelas de símbolos, com ou sem um escopo pai.
    SymbolTable();
    SymbolTable(SymbolTable*);

    // Funções para manipulação da tabela de símbolos.
    bool add(STEntry*);          // Adiciona um novo símbolo.
    bool remove(std::string);    // Remove um símbolo.
    void clear();                // Limpa todos os símbolos.
    bool isEmpty();              // Verifica se a tabela está vazia.
    STEntry* get(std::string);   // Busca um símbolo pelo nome (lexema).
    SymbolTable* getParent();    // Retorna a tabela pai (escopo anterior).
    SymbolTable* initializeKeywords();        // Inicializa a tabela de símbolos com palavras-chave.
};
```

`symboltable.cpp` - Funcionalidades da classe _SymbolTable_.
```cpp
// Construtor padrão que inicializa uma nova tabela de símbolos sem um escopo pai.
SymbolTable::SymbolTable() {
    parent = nullptr;
}

// O escopo pai é usado para busca hierárquica de símbolos em escopos mais amplos.
SymbolTable::SymbolTable(SymbolTable* p) {
    parent = p;
}

// Tenta adicionar um novo símbolo à tabela atual.
// - Se já houver um símbolo com o mesmo lexema, a função retorna `false` sem adicionar.
// - Caso contrário, o símbolo é inserido e a função retorna `true`.
bool SymbolTable::add(STEntry* t) {
    if (symbols.find(t->token->lexeme) != symbols.end())
        return false; // Símbolo já existe.
    
    symbols.insert({t->token->lexeme, t});
    return true;
}
[...]
```

**Análise Semântica no Parser:**
```cpp
[...]
// Declara uma classe na tabela de símbolos.
void Parser::declareClass(string className, string parentClass) {
    STEntry* existing = symbolTable->get(className);
    
    // Verifica se já existe uma classe com esse nome.
    if (existing != nullptr && existing->kind == CLASS_NAME) {
        semanticError("Classe '" + className + "' ja foi declarada na linha " + to_string(existing->line));
    }
    
    // Se há classe pai, verifica se ela existe.
    if (!parentClass.empty()) {
        STEntry* parent = symbolTable->get(parentClass);
        if (parent == nullptr || parent->kind != CLASS_NAME) {
            semanticError("Classe pai '" + parentClass + "' nao foi declarada");
        }
    }
    
    // Cria entrada para a classe.
    Token* classToken = new Token(ID, className);
    STEntry* classEntry = new STEntry(classToken, CLASS_NAME, "class", false, scanner->getLine());
    classEntry->parentClass = parentClass;
    
    if (!symbolTable->add(classEntry)) {
        semanticError("Erro ao adicionar classe '" + className + "' na tabela de simbolos");
    }
}
[...]
void Parser::checkVariableDeclared(string varName) {
    STEntry* entry = currentScope->get(varName);
    
    if (entry == nullptr) {
        semanticError("Variavel '" + varName + "' nao foi declarada");
    }
    
    if (entry->reserved) {
        semanticError("'" + varName + "' e uma palavra reservada e nao pode ser usada como variavel");
    }
    
    // cout << "[SEMANTICO] Variavel '" << varName << "' usada na linha " << scanner->getLine() 
    //     << " (declarada na linha " << entry->line << ")" << endl;
}

void Parser::checkClassDeclared(string className) {
    STEntry* entry = symbolTable->get(className);
    
    if (entry == nullptr || entry->kind != CLASS_NAME) {
        semanticError("Classe '" + className + "' nao foi declarada");
    }
}

// Lança um erro semântico com mensagem detalhada.
void Parser::semanticError(string message) {
    cout << "\n[ERRO SEMANTICO] Linha " << scanner->getLine() << ": " << message << endl;
    exit(EXIT_FAILURE);
}
```

## 📌 Como executar o projeto

### Acesse a pasta do projeto no terminal:

```bash
cd part03_analise_semantica/
```

### Compile o código com o seguinte comando:

```bash
g++ -o xpp_compiler *.cpp
```

### Execute o programa com um arquivo de teste:

**Linux/Mac:**
```bash
./xpp_compiler tests/test_success.xpp
```

**Windows (PowerShell):**
```powershell
.\xpp_compiler.exe tests\test_success.xpp
```

### Exemplos de execução:

**Arquivo válido:**
```bash
.\xpp_compiler.exe tests\test_success.xpp
```
```

[SUCESSO] Compilacao finalizada com sucesso.
```

**Arquivo com erro léxico:**
```bash
.\xpp_compiler.exe tests\test_erro_lexico.xpp
```
```

[ERRO LEXICO] Linha 7: caractere invalido '@'
```

**Arquivo com erro sintático:**
```bash
.\xpp_compiler.exe tests\test_erro_sintaxe.xpp
```
```

[ERRO SINTATICO] Linha 8: esperava 'RIGHT_CURLY_BRACE' mas encontrou 'END_OF_FILE'
```

**Arquivo com erro semântico:**
```bash
.\xpp_compiler.exe tests\test_erro_semantico1.xpp
```
```

[ERRO SEMANTICO] Linha 7: Variavel 'resultado' nao foi declarada
```

---

# Analisador Léxico e Sintático (Compilador X++)

## 👨‍💻 Projeto desenvolvido por: [Rafael Torres Nantes](https://github.com/rafael-torres-nantes)

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
```cpp
class Scanner 
{
private: 
    string input;           // Armazena o texto de entrada
    int pos;                // Posição atual no buffer
    int line;               // Linha atual do arquivo
    SymbolTable* symbolTable; // Tabela de símbolos
    
public:
    // Construtor
    Scanner(string, SymbolTable*);
    
    int getLine(); // Retorna a linha atual
    
    // Método que retorna o próximo token da entrada
    Token* nextToken();        
    
    // Método para manipular erros léxicos
    void lexicalError(string);
};
```

`scanner.cpp` - Funcionamento da classe _Scanner_.
```cpp
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
        case 0: // Verifica os caracteres iniciais
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
            // ... outros operadores
            else if (isalpha(input[pos]) || input[pos] == '_')
            {
                state = 1;
                lexeme += input[pos];
            }
            else if (isdigit(input[pos]))
            {
                state = 3;
                lexeme += input[pos];
            }
            // ... tratamento de espaços e comentários
            
        case 1: // Identificador ou palavra reservada
            if (isalnum(input[pos]) || input[pos] == '_')
                lexeme += input[pos];
            else
                state = 2;
            pos++;
            break;

        case 2: // Verifica se é palavra reservada
            STEntry* entry = symbolTable->get(lexeme);
            if (entry != nullptr && entry->reserved) {
                token = new Token(entry->token->type, lexeme);
            } else {
                token = new Token(ID, lexeme);
            }
            pos--;
            return token;
            
        // ... outros estados
```

`token.h` - Classe _Token_ e TokenTypes.
```cpp
enum TokenType 
{
    UNDEFINED,             // 0  - Token não definido
    ID,                    // 1  - Identificador
    INTEGER_LITERAL,       // 2  - Literal inteiro
    STRING_LITERAL,        // 3  - Literal string
    
    // Palavras reservadas
    CLASS,                 // 4  - class
    EXTENDS,               // 5  - extends
    INT,                   // 6  - int
    STRING,                // 7  - string
    BREAK,                 // 8  - break
    PRINT,                 // 9  - print
    READ,                  // 10 - read
    RETURN,                // 11 - return
    SUPER,                 // 12 - super
    IF,                    // 13 - if
    ELSE,                  // 14 - else
    FOR,                   // 15 - for
    NEW,                   // 16 - new
    CONSTRUCTOR,           // 17 - constructor
    
    // Operadores
    PLUS_OPERATOR,         // 18 - +
    MINUS_OPERATOR,        // 19 - -
    MULTIPLY_OPERATOR,     // 20 - *
    DIVIDE_OPERATOR,       // 21 - /
    MODULO_OPERATOR,       // 22 - %
    ASSIGNMENT,            // 23 - =
    EQUAL,                 // 24 - ==
    NOT_EQUAL,             // 25 - !=
    LESS_THAN,             // 26 - <
    GREATER_THAN,          // 27 - >
    LESS_OR_EQUAL_THAN,    // 28 - <=
    GREATER_OR_EQUAL_THAN, // 29 - >=
    
    // Separadores
    LEFT_BRACKET,          // 30 - (
    RIGHT_BRACKET,         // 31 - )
    LEFT_CURLY_BRACE,      // 32 - {
    RIGHT_CURLY_BRACE,     // 33 - }
    LEFT_SQUARE_BRACKET,   // 34 - [
    RIGHT_SQUARE_BRACKET,  // 35 - ]
    COMMA,                 // 36 - ,
    SEMICOLON,             // 37 - ;
    DOT,                   // 38 - .
    
    END_OF_FILE            // 39 - EOF
};

class Token 
{
public: 
    int type;       // Tipo do token
    string lexeme;  // Texto reconhecido
    
    // Construtores
    Token(int type) 
    {
        this->type = type;
        lexeme = "";
    }

    Token(int type, string lexeme)
    {
        this->type = type;
        this->lexeme = lexeme;
    }
    
    // Método estático para obter nome do token
    static string getTokenTypeName(int type);
};
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
```cpp
class Parser {
public:
    // Construtor que inicializa o scanner
    Parser(string input, SymbolTable*);

    // Método para iniciar o processo de análise
    void run();

private:
    Scanner* scanner;          // Scanner para tokenização
    Token* lToken;             // Token atual
    SymbolTable* symbolTable;  // Tabela de símbolos
    SymbolTable* currentScope; // Escopo atual
    string currentClass;       // Classe atual
    string currentType;        // Tipo atual
    bool currentIsArray;       // Se é array

    // Métodos auxiliares
    void advance();
    void match(int t);
    
    // Métodos de análise semântica
    void enterScope();
    void exitScope();
    void declareClass(string className, string parentClass = "");
    void declareVariable(string varName, string varType, bool isArray);
    void checkVariableDeclared(string varName);
    void checkClassDeclared(string className);
    void semanticError(string message);

    // Métodos da gramática
    void Program();              // Program → ClassList
    void ClassList();            // ClassList → ClassDecl ClassList | ClassDecl
    void ClassDecl();            // ClassDecl → class ID ClassBody | class ID extends ID ClassBody
    void ClassBody();            // ClassBody → { VarDeclListOpt ConstructDeclListOpt MethodDeclListOpt }
    void VarDeclListOpt();       // VarDeclListOpt → VarDeclList | ε
    void VarDecl();              // VarDecl → Type ID VarDeclOpt ; | Type [] ID VarDeclOpt ;
    void Type();                 // Type → int | string | ID
    void MethodDecl();           // MethodDecl → Type ID MethodBody | Type [] ID MethodBody
    void MethodBody();           // MethodBody → ( ParamListOpt ) { StatementsOpt }
    void Statement();            // Statement → VarDeclList | AtribStat ; | ...
    void Expression();           // Expression → NumExpression | NumExpression RelOp NumExpression
    void LValue();               // LValue → ID LValueComp
    // ... outros métodos
    
    // Métodos auxiliares
    bool isType();
    bool isStatement();
    void error(string str);
};
```

`parser.cpp` - Implementação das funções da Classe _Parser_.
```cpp
// Produção principal: Program → ClassList
void Parser::Program() {
    ClassList(); // Analisa a lista de classes
    match(END_OF_FILE); // Espera que o arquivo termine
}

// Produção: ClassDecl → class ID ClassBody | class ID extends ID ClassBody
void Parser::ClassDecl() {
    match(CLASS); // Espera 'class'
    
    // Captura o nome da classe
    if (lToken->type != ID) {
        error("Nome da classe esperado");
    }
    string className = lToken->lexeme;
    currentClass = className;
    match(ID);
    
    string parentClass = "";
    if (lToken->type == EXTENDS) {
        advance();
        if (lToken->type != ID) {
            error("Nome da classe pai esperado");
        }
        parentClass = lToken->lexeme;
        match(ID);
    }
    
    // Análise semântica: declara a classe
    declareClass(className, parentClass);
    
    // Cria novo escopo para a classe
    enterScope();
    ClassBody();
    exitScope();
    
    currentClass = "";
}

// Produção: Type → int | string | ID
void Parser::Type() {
    if (lToken->type == INT || lToken->type == STRING || lToken->type == ID) {
        advance(); // Avança se o tipo for válido
    } else {
        error("Tipo esperado (int, string ou ID)");
    }
}
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
```cpp
// Enumeração para tipos de símbolos
enum SymbolKind {
    KEYWORD,         // Palavra reservada
    CLASS_NAME,      // Nome de classe
    VARIABLE,        // Variável
    METHOD,          // Método
    CONSTRUCTOR_SYM, // Construtor
    PARAMETER        // Parâmetro
};

class STEntry {
public:
    Token* token;         // Token associado
    bool reserved;        // Se é palavra reservada
    SymbolKind kind;      // Tipo do símbolo
    string type;          // Tipo (int, string, nome da classe)
    bool isArray;         // Se é array
    string parentClass;   // Classe pai (para classes)
    int line;             // Linha de declaração

    // Construtores
    STEntry(); 
    STEntry(Token*);   
    STEntry(Token*, bool);
    STEntry(Token*, SymbolKind, string type = "", bool isArray = false, int line = 0);
};
```

`stentry.cpp` - Construtores da classe _STEntry_.
```cpp
// Construtor padrão
STEntry::STEntry() {
    token = nullptr;
    reserved = false;
    kind = KEYWORD;
    type = "";
    isArray = false;
    parentClass = "";
    line = 0;
}

// Construtor completo para análise semântica
STEntry::STEntry(Token* tok, SymbolKind k, string t, bool arr, int ln) {
    token = tok;
    reserved = false;
    kind = k;
    type = t;
    isArray = arr;
    parentClass = "";
    line = ln;
}
```

`symboltable.h` - Declaração da classe _SymbolTable_.
```cpp
class SymbolTable {
public:
    SymbolTable* parent; // Tabela pai (escopo superior)
    std::map<std::string, STEntry*> symbols; // Símbolos do escopo atual

    // Construtores
    SymbolTable();
    SymbolTable(SymbolTable*);

    // Funções de manipulação
    bool add(STEntry*);          // Adiciona símbolo
    bool remove(std::string);    // Remove símbolo
    void clear();                // Limpa tabela
    bool isEmpty();              // Verifica se está vazia
    STEntry* get(std::string);   // Busca símbolo (busca hierárquica)
    SymbolTable* getParent();    // Retorna tabela pai
};
```

`symboltable.cpp` - Funcionalidades da classe _SymbolTable_.
```cpp
// Construtor com escopo pai
SymbolTable::SymbolTable(SymbolTable* p) {
    parent = p;
}

// Adiciona símbolo (verifica duplicação no escopo atual)
bool SymbolTable::add(STEntry* t) {
    if (symbols.find(t->token->lexeme) != symbols.end())
        return false; // Símbolo já existe
    
    symbols.insert({t->token->lexeme, t});
    return true;
}

// Busca símbolo com busca hierárquica (escopo atual → pais)
STEntry* SymbolTable::get(string name) {
    SymbolTable* table = this;
    auto s = table->symbols.find(name);

    while (s == table->symbols.end()) {
        table = table->parent;
        if (table == nullptr) 
            return nullptr; // Não encontrado em nenhum escopo
        s = table->symbols.find(name);
    }
    
    return s->second;
}
```

**Análise Semântica no Parser:**
```cpp
// Declara uma classe na tabela de símbolos
void Parser::declareClass(string className, string parentClass) {
    STEntry* existing = symbolTable->get(className);
    
    // Verifica redeclaração
    if (existing != nullptr && existing->kind == CLASS_NAME) {
        semanticError("Classe '" + className + "' ja foi declarada na linha " + to_string(existing->line));
    }
    
    // Verifica se classe pai existe
    if (!parentClass.empty()) {
        STEntry* parent = symbolTable->get(parentClass);
        if (parent == nullptr || parent->kind != CLASS_NAME) {
            semanticError("Classe pai '" + parentClass + "' nao foi declarada");
        }
    }
    
    // Cria entrada para a classe
    Token* classToken = new Token(ID, className);
    STEntry* classEntry = new STEntry(classToken, CLASS_NAME, "class", false, scanner->getLine());
    classEntry->parentClass = parentClass;
    
    symbolTable->add(classEntry);
    
    cout << "[SEMANTICO] Classe '" << className << "' declarada";
    if (!parentClass.empty()) {
        cout << " (herda de '" << parentClass << "')";
    }
    cout << " na linha " << scanner->getLine() << endl;
}

// Verifica se variável foi declarada antes do uso
void Parser::checkVariableDeclared(string varName) {
    STEntry* entry = currentScope->get(varName);
    
    if (entry == nullptr) {
        semanticError("Variavel '" + varName + "' nao foi declarada");
    }
    
    if (entry->reserved) {
        semanticError("'" + varName + "' e uma palavra reservada");
    }
    
    cout << "[SEMANTICO] Variavel '" << varName << "' usada na linha " << scanner->getLine() 
         << " (declarada na linha " << entry->line << ")" << endl;
}
```

## 📌 Como executar o projeto

### Acesse a pasta do projeto no terminal:

```bash
cd part03_analise_semantica/
```

### Compile o código com o seguinte comando:

```bash
g++ -o xpp_compiler principal.cpp parser.cpp scanner.cpp symboltable.cpp stentry.cpp
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

### Execute a suite de testes automatizados:

**Windows (PowerShell):**
```powershell
.\run_tests.ps1
```

**Saída esperada:**
```
========================================
  TESTES DO COMPILADOR X++
========================================

Testando: Programa valido
  Arquivo: test_success.xpp
  PASSOU

[...]

========================================
RESUMO
========================================
Total: 9
Passou: 9
Falhou: 0
```

### Exemplos de execução:

**Arquivo válido:**
```bash
.\xpp_compiler.exe tests\test_success.xpp
```
```
[SEMANTICO] Classe 'Pessoa' declarada na linha 4
[SEMANTICO] Variavel 'idade' do tipo 'int' declarada na linha 5
[SEMANTICO] Variavel 'nome' do tipo 'string' declarada na linha 6
...
Compilacao finalizada com sucesso.
```

**Arquivo com erro semântico:**
```bash
.\xpp_compiler.exe tests\test_erro_semantico1.xpp
```
```
[SEMANTICO] Classe 'Teste' declarada na linha 3
[SEMANTICO] Variavel 'valor' do tipo 'int' declarada na linha 4

[ERRO SEMANTICO] Linha 7: Variavel 'resultado' nao foi declarada
Token atual: ID ('resultado')
```

---

**📄 Documentação Adicional:**
- `RELATORIO_CONFORMIDADE.md` - Verificação completa dos requisitos do trabalho
- `README_ANALISE_SEMANTICA.md` - Detalhes da análise semântica implementada
- `MANUAL_USO.md` - Manual de uso rápido

**✅ Status:** Projeto completo com todas as etapas implementadas e testadas.

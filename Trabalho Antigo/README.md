# Analisador Léxico e Sintático C--

## 👨‍💻 Projeto desenvolvido por: [Rafael Torres Nantes](https://github.com/rafael-torres-nantes) e [Sarah Baraldi](https://github.com/mbaraldi-sarah).

## Índice

* [📚 Contextualização do Projeto](#-contextualização-do-projeto)
* [🛠️ Tecnologias/Ferramentas utilizadas](#%EF%B8%8F-tecnologiasferramentas-utilizadas)
* [🖥️ Funcionamento do sistema](#%EF%B8%8F-funcionamento-do-sistema)
   * [🔍 Etapa 1 - Analisador Léxico](#-etapa1---analisador-léxico)
   * [📋 Etapa 2 - Analisador Sintático](#-etapa-2---analisador-sintático)
   * [🧮 Etapa 3 - Tabela de Símbolos](#-etapa-3---tabela-de-símbolos)
* [📌 Como executar o projeto](#-como-executar-o-projeto)

## 📚 Contextualização do Projeto

O projeto consiste em um compilador capaz de analisar o léxico e a sintase de um código na linguagem fictícia C--, uma sublinguagem da C. O programa foi desenvolvido utilizando a linguagem C++.

## 🛠️ Tecnologias/Ferramentas utilizadas

[<img src="https://img.shields.io/badge/Visual_Studio_Code-007ACC?">](https://code.visualstudio.com/)
[<img src="https://img.shields.io/badge/GitHub-181717?logo=github&logoColor=violet">](https://github.com/)

[<img src="https://img.shields.io/badge/-C++-blue?logo=cplusplus">](https://isocpp.org/)


## 🖥️ Funcionamento do sistema

### 🔍 Etapa1 - Analisador Léxico

Nessa primeira etapa, foi implementada a Análise Léxica nos arquivos `scanner.h` e `scanner.cpp`. Onde no primeiro está declarada a classe _Scanner_ e no segundo o seu funcionamento. A implementaçãoé como uma a máquina de estados onde os tokens são verificados.

Além disso, o arquivo `token.h` contém a classe _Token_ onde estão definidos os tokens (_TokenTypes_) da linguagem. A seguir, estão apresentadas as **definições regulares dos tokens**, nas quais os símbolos terminais estão destacados em **negrito**:

1. _letter_ → **[a-zA-Z]**
2. _digit_ → **[0-9]**
3. _ID_ → letter (**_letter_** | **_digit_** | **_** **)** **∗**
4. _integerconstant_ → **_digit_+**
5. _charconstant_ → **'ch'** | ‘**\n**’ | ‘**\0**’, onde ch é _qualquer_ caractere ASCII _imprimível_ (como especificado pela função isprint(), **exceto \ e aspas simples**)
6. _stringconstant_ → **"(ch)*"**, onde ch é _qualquer_ caractere ASCII _imprimível_ (como especificado pela função isprint(), **exceto aspas duplas e \n**)
7. _Operadores_ → **+** | **-** | **∗** | **/** | **=** | **==** | **!=** | **<=** | **<** | **>=** | **>** | **&&** | **||** | **!**
8. _Separadores_ → **(** | **)** | **{** | **}** | **[** | **]** | **,** | **;**
9. _Comentários_ devem ser **ignorados**. Seguem o formato de C e podem ser em linha ou em bloco. O comentário em linha é iniciado pelos caracteres **//** e encerrado por nova linha e o comentário em bloco é delimitado por **/*** e ***/**.
10. _Espaços em branco_ devem ser **ignorados**.

#### Um pouco da implementação...

`scanner.h` - Declaração da classe _Scanner_.
```h
class Scanner 
{
    private: 
        string input;   //Armazena o texto de entrada, buffer de entrada
        int pos;        //Posição atual no buffer
        int line;       // Qual linha do arquivo estou
    
    public:
    //Construtor
        Scanner(string);    // arquivo de entrada

        int getLine();      // get para retornar pois arq privado
    
        //Método que retorna o próximo token da entrada
        Token* nextToken();        
    
        //Método para manipular erros
        void lexicalError();
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
        case 0: // Verifica os caracteres iniciais para determinar o tipo de token
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
[...]

case 3: // Dígito
            if (!isdigit(input[pos]))
                state = 4;
            else
                lexeme += input[pos];

            pos++;
            break;

        case 4: // Integer Constant
            if (isalpha(input[pos])) {
                lexicalError();
            }
            token = new Token(INTEGER_CONSTANT, lexeme);
            pos--;
            return token;

        case 5: // <
            if (input[pos] == '=')
                state = 39;
            else
                state = 40;
            pos++;
            break;
[...]
```

`token.h` - Classe _Token_ e TokenTypes.
```h
enum TokenType 
{
    UNDEFINED,             // 0  - Undefined token
    ID,                    // 1  - Identifier (e.g., variable names)
    DIGIT,                 // 2  - Integer literal
    LETTER,                // 3  - Letter [a-zA-Z]
    INTEGER_CONSTANT,      // 4  - digit+

[...]

class Token 
{
    public: 
        int type;       // Token type
        int attribute;  // Attribute which can be empty
        string lexeme;  // Recognized text
    
        // Constructors for different types of tokens

        // Only type
        Token(int type) // Example: if, else.
        {
            this->type = type;
            attribute = UNDEFINED;
            lexeme = "";
        }

        // Type and lexeme
        Token(int type, string lexeme)
        {
            this->type = type;
            attribute = UNDEFINED;
            this->lexeme = lexeme;
        }
[...]
```

### 📋 Etapa 2 - Analisador Sintático

O analisador sintático consiste em verificar se o código está seguindo as **regras de gramática** da linguagem. ELe foi implementado nos arquivos `parser.h` e `parser.cpp`, onde no primeiro está definida a classe _Parser_ e no segundo, sua funções.

Na tabela a seguir está descrita a gramática, que utiliza as notações ( N )∗ para representar **0 ou mais** repetições de  N e a notação ( N )? para representar **0 ou 1 repetição** de N. Os tokens da linguagem são  representados em **negrito** e os não-terminais em _itálico_. 

| Produção →                 | Corpo                    |
|------------------------|-------------------------| 
| _Program_ →                 | → (_Function_)  ∗ **EOF**                     |
| _Function_ →            | _Type_ **ID**( _ParamTypes_ ) { (_Type VarDeclaration_(, _VarDeclaration_) ∗ ; ) ∗ (_Statement_) ∗ }                | 
|      | **void ID(** _ParamTypes_ **){**(_Type VarDeclaration_(, _VarDeclaration_)  ∗ **;** ) ∗ (_Statement_) ∗ **}**
| _VarDeclaration_ →             | **ID** (**[integerconstant]** )?                 | 
| _Type_ → |  **char**  |
|          |  **int**  |
| _ParamTypes_ →         |  **void**  |
|          |  _Type_ **ID**( **[ ]** )? (**,** _Type_ **ID**( **[ ]** )?)∗  |
| _Statement_ →         |  **if** ( _Expression_ ) _Statement_ (**else** _Statement_)?  |
|          |  **while** ( _Expression_ ) _Statement_  |
|          |  **for (** (_Assign_)?**;** (_Expression_)?**;** (Assign)? **)** _Statement_  |
|          |  **return** (_Expression_)? **;**  |
|          |  _Assign_ **;**  |
|          |  **ID**( (_Expression_( **,** _Expression_)  ∗)? ) **;**   |
|          |  **{** (_Statement_) ∗ **}**  |
|          |  **;**  |
| _Assign_ →         |  **ID** (**[** _Expression_ **]**)? **=** _Expression_  |
| _Expression_ →         |  **-** _Expression_  |
|          |  **!** _Expression_  |
|          |  _Expression_ **BinOp** _Expression_  |
|          |  _Expression_ **Relop** _Expression_  |
|          |  _Expression_ **LogOp** _Expression_  |
|          |  **ID** ( **(** (_Expression_ ( , _Expression_)∗)? **)** | **[** _Expression_ **]**)?  |
|          |  **(** _Expression_ **)**  |
|          |  **integerconstant**  |
|          |  **charconstant**  |
|          |  **stringconstant**  |
| _BinOp_ →         |  **+**  |
|          |  **-**  |
|          |  **∗** |
|          |  **/**  |
| _RelOp_ →         |  **==**  |
|          |  **!=**  |
|          |  **<=**  |
|          |  **<**  |
|          |  **>=**  |
|          |  **>**  |
| _LogOp_ →         |  **&&**  |
|          |  \|\|  |

Para que a gramática pudesse ser compilada, foi necessário transformá-la em **LL(1)** removendo as **recursões** à esquerda e ambiguidades nos **FIRSTs** necessários.

#### Um pouco da implementação...

`parser.h` - Definição da Classe _Parser_
```h
class Parser {
public:
    // Constructor that initializes the scanner with the input string
    Parser(string input, SymbolTable*);

    // Method to start the parsing process
    void run();

private:
    Scanner* scanner; // Scanner object to tokenize the input
    Token* lToken;    // Current token
    SymbolTable* symbolTable; // Symbol table for storing identifiers

    // Method to advance to the next token
    void advance();

    // Method to match the current token with the expected token type and lexeme
    void match(int t);

    // Grammar production methods
    void Program();          // Program → (Function)∗ EOF
    void Function();         // Function → Type ID( ParamTypes ){ ... } | void ID( ParamTypes ){ ... }
    void VarDeclaration();   // VarDeclaration → ID ([integerconstant] )?
    void Type();             // Type → char | int
[...]
```

`parser.cpp` - Implementação das funções da Classe _Parser_.
```cpp
[...]
// Producao que reconhece tipos ("int" ou "char").
void Parser::Type() {
    if (lToken->lexeme == "char" || lToken->lexeme == "int") {
        advance(); // Avanca se o tipo for válido.
    } else {
        error("Tipo invalido. Esperado 'int' ou 'char'.");
    }
}
[...]
```

### 🧮 Etapa 3 - Tabela de Símbolos

Implementamos uma tabela de símbolos para **guardar** tanto as **palavras reservadas** quanto **variáveis** criadas pelo usuário. Sua implementaçao se deu principalmente nos arquivos `stentry.h`, `stentry.cpp`, `symboltable.h`, `symboltable.cpp`, além de alterações nos arquivos da análise sintática. No primeiro, foi definida a classe _STEntry_ e no segundo seus construtores. Já nos arquivos `symboltable.h`, `symboltable.cpp` temos  a definição da classe _SymbolTable_ e suas funcionalidades.

As alterações nos arquivos da análise sintática se deram por tratar erros de declaração e utilização de variáveis utilizando a tabela de símbolos.

#### Um pouco da implementação...

`stentry.h` - Declaração da classe _STEntry_.
```h
class STEntry {
public:
    Token* token;    // Ponteiro para o token associado.
    bool reserved;   // Indica se o símbolo é uma palavra reservada.

    // Construtores para criar uma entrada de símbolo com diferentes configurações.
    STEntry(); 
    STEntry(Token*);   
    STEntry(Token*, bool);
};
```

`stentry.cpp` - Construtores da classe _STEntry_.
```cpp
// Construtor padrão que inicializa uma entrada de símbolo sem associar um token.
STEntry::STEntry() {}

// Construtor que inicializa uma entrada de símbolo com um token fornecido.
// O símbolo não é marcado como reservado por padrão.
STEntry::STEntry(Token* tok) {
    token = tok;
    reserved = false;
}

// Construtor que inicializa uma entrada de símbolo com um token e uma flag de reserva.
STEntry::STEntry(Token* tok, bool res) {
    token = tok;
    reserved = res;
}
```

`symboltable.h` - Declaração da classe _SymbolTable_.
```h
class SymbolTable {
private:
    SymbolTable* parent; // Referência à tabela pai (escopo imediatamente anterior).
    std::map<std::string, STEntry*> symbols; // Armazena os símbolos do escopo atual.

public:
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
SymbolTable::SymbolTable() {
    parent = nullptr;
}

// Construtor que inicializa uma nova tabela de símbolos especificando um escopo pai.
// O escopo pai é usado para busca hierárquica de símbolos em escopos mais amplos.
SymbolTable::SymbolTable(SymbolTable* p) {
    parent = p;
}

// Tenta adicionar um novo símbolo à tabela atual.
// Verifica se já existe uma entrada para o lexema na tabela atual:
// - Se já houver um símbolo com o mesmo lexema, a função retorna `false` sem adicionar.
// - Caso contrário, o símbolo é inserido e a função retorna `true`.
bool SymbolTable::add(STEntry* t) {
    if (symbols.find(t->token->lexeme) != symbols.end())
        return false; // Símbolo já existe.
    
    symbols.insert({t->token->lexeme, t});
    return true;
}
```

## 📌 Como executar o projeto

### Acesse a pasta do projeto no cmd:

```bash
$ cd CompiladorCMM/
```

### Compile o código com o seguinte código:

```bash
g++ *.cpp -o cmm
```

### Execute o programa com o arquivo teste:
```bash
./cmm teste1.cmm
```
#include "superheader.h"

// Enumeração para tipos de símbolos na tabela.
enum SymbolKind {
    KEYWORD,        // Palavra reservada
    CLASS_NAME,     // Nome de classe
    VARIABLE,       // Variável local ou de classe
    METHOD,         // Método
    CONSTRUCTOR_SYM,// Construtor
    PARAMETER       // Parâmetro de método
};

// A classe `STEntry` representa uma entrada na tabela de símbolos.
// Cada entrada contém um ponteiro para um token associado e informações
// semânticas sobre o símbolo.
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

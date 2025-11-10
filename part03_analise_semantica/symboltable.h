#include "superheader.h"

// A classe `SymbolTable` representa uma tabela de símbolos que utiliza um mapa (`map`) para
// armazenar pares de chave-valor, onde a chave é uma string (o lexema) e o valor é um ponteiro
// para um objeto da classe `STEntry`.
// A tabela suporta escopos hierárquicos através da referência à tabela pai.
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
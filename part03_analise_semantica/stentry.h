#include "superheader.h"

// A classe `STEntry` representa uma entrada na tabela de símbolos.
// Cada entrada contém um ponteiro para um token associado e uma flag
// que indica se o símbolo é reservado (palavra-chave).
class STEntry {
public:
    Token* token;    // Ponteiro para o token associado.
    bool reserved;   // Indica se o símbolo é uma palavra reservada.

    // Construtores para criar uma entrada de símbolo com diferentes configurações.
    STEntry(); 
    STEntry(Token*);   
    STEntry(Token*, bool);
};
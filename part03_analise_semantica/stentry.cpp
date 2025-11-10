#include "superheader.h"

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

// Construtor que inicializa uma entrada de símbolo com um token fornecido.
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

// Construtor completo para análise semântica detalhada.
STEntry::STEntry(Token* tok, SymbolKind k, string t, bool arr, int ln) {
    token = tok;
    reserved = false;
    kind = k;
    type = t;
    isArray = arr;
    parentClass = "";
    line = ln;
}
#include "superheader.h"

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
#include "superheader.h"

// Construtor padrão que inicializa uma nova tabela de símbolos sem um escopo pai.
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

// Remove um símbolo da tabela baseado no lexema fornecido.
// Retorna `true` se a remoção foi bem-sucedida (símbolo encontrado e removido),
// ou `false` caso contrário.
bool SymbolTable::remove(string name) {
    return symbols.erase(name) != 0;
}

// Limpa todos os símbolos do escopo atual, esvaziando a tabela.
void SymbolTable::clear() {
    symbols.clear();
}

// Verifica se a tabela de símbolos está vazia.
// Retorna `true` se não houver nenhum símbolo registrado.
bool SymbolTable::isEmpty() {
    return symbols.empty();
}

// Busca um símbolo pelo nome (lexema).
// A busca é feita primeiro na tabela atual e, se não encontrado, sobe na hierarquia
// até o escopo global (tabela raiz):
// - Retorna um ponteiro para o `STEntry` se o símbolo for encontrado.
// - Retorna `nullptr` se o símbolo não for encontrado em nenhum escopo.
STEntry* SymbolTable::get(string name) {
    SymbolTable* table = this;
    auto s = table->symbols.find(name);

    while (s == table->symbols.end()) { // Continuar a busca nos escopos superiores.
        table = table->parent;
        if (table == nullptr) 
            return nullptr; // Chegou ao topo da hierarquia e não encontrou o símbolo.
        s = table->symbols.find(name);
    }
    
    return s->second;
}

// Retorna um ponteiro para a tabela pai (escopo anterior).
// Útil para navegação hierárquica entre diferentes escopos.
SymbolTable* SymbolTable::getParent() {
    return parent;
}
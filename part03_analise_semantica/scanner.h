#include "superheader.h"

class Scanner 
{
    private: 
        string input;   // Armazena o texto de entrada, buffer de entrada
        int pos;        // Posicao atual no buffer
        int line;       // Qual linha do arquivo estou
        SymbolTable* symbolTable; // Tabela de simbolos para diferenciar IDs de palavras reservadas
    
    public:
        // Construtor
        Scanner(string, SymbolTable*);    // Arquivo de entrada e tabela de simbolos

        int getLine();      // Get para retornar pois arq privado
    
        // Metodo que retorna o proximo token da entrada
        Token* nextToken();        
    
        // Metodo para manipular erros
        void lexicalError();
};

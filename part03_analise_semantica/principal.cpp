#include "superheader.h"

int main(int argc, char* argv[]) 
{
    //Verifica se foi executado corretamente
    //Essa main espera receber o nome do arquivo a ser
    //executado na linha de comando.
    if (argc != 2)
    {
        cout<< "Uso: ./compiler nome_arquivo.mj\n";
        return 1;
    }

    //Inicializando tabela de simbolos com palavras reservadas
    SymbolTable* symbolTable = new SymbolTable();

    symbolTable->add(new STEntry(new Token(INT, "int"), true));
    symbolTable->add(new STEntry(new Token(CHAR, "char"), true));
    symbolTable->add(new STEntry(new Token(IF, "if"), true));
    symbolTable->add(new STEntry(new Token(ELSE, "else"), true));
    symbolTable->add(new STEntry(new Token(WHILE, "while"), true));
    symbolTable->add(new STEntry(new Token(FOR, "for"), true));
    symbolTable->add(new STEntry(new Token(RETURN, "return"), true));
    symbolTable->add(new STEntry(new Token(VOID, "void"), true));

    Parser* parser = new Parser(argv[1], symbolTable);
    parser->run();

    return 0;
}
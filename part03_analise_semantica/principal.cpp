#include "superheader.h"

int main(int argc, char* argv[]) 
{
    // Verifica se foi executado corretamente.
    // Esta main espera receber o nome do arquivo a ser
    // executado na linha de comando.
    if (argc != 2)
    {
        cout << "Uso: ./xpp_compiler nome_arquivo.xpp\n";
        return 1;
    }

    // Inicializando tabela de simbolos com palavras reservadas do X++.
    SymbolTable* symbolTable = new SymbolTable();

    // Palavras reservadas da linguagem X++
    symbolTable->add(new STEntry(new Token(CLASS, "class"), true));
    symbolTable->add(new STEntry(new Token(EXTENDS, "extends"), true));
    symbolTable->add(new STEntry(new Token(INT, "int"), true));
    symbolTable->add(new STEntry(new Token(STRING, "string"), true));
    symbolTable->add(new STEntry(new Token(BREAK, "break"), true));
    symbolTable->add(new STEntry(new Token(PRINT, "print"), true));
    symbolTable->add(new STEntry(new Token(READ, "read"), true));
    symbolTable->add(new STEntry(new Token(RETURN, "return"), true));
    symbolTable->add(new STEntry(new Token(SUPER, "super"), true));
    symbolTable->add(new STEntry(new Token(IF, "if"), true));
    symbolTable->add(new STEntry(new Token(ELSE, "else"), true));
    symbolTable->add(new STEntry(new Token(FOR, "for"), true));
    symbolTable->add(new STEntry(new Token(NEW, "new"), true));
    symbolTable->add(new STEntry(new Token(CONSTRUCTOR, "constructor"), true));

    // Cria o parser passando o arquivo de entrada e a tabela de simbolos.
    Parser* parser = new Parser(argv[1], symbolTable);
    parser->run();

    return 0;
}
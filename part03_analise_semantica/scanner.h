#include "superheader.h"

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
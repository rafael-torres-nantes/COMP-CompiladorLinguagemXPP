#include "scanner.h"    

//Construtor
Scanner::Scanner(string input)
{
    this->input = input;

    cout << "Entrada: " << input << endl << "Tamanho: " 
         << input.length() << endl;

    pos = 0;
}

//Método que retorna o próximo token da entrada
Token* 
Scanner::nextToken()
{
    Token* tok;
    //string lexeme;

    //Consumir espaços em branco
    while (isspace(input[pos]))
        pos++;

    //Verificar os tokens possíveis
    //Fim de arquivo
    if (input[pos] == '\0')
    {
        tok = new Token(END_OF_FILE);
    }
    //Operadores aritméticos
    else if (input[pos] == '+')
    {
        pos++;
        tok = new Token(PLUS);
    }
    else if (input[pos] == '-')
    {
        pos++;
        tok = new Token(MINUS);
    }
    else if (input[pos] == '*')
    {
        pos++;
        tok = new Token(MULT);
    }
    else if (input[pos] == '/')
    {
        pos++;
        tok = new Token(DIV);
    }
    //Parênteses
    else if (input[pos] == '(')
    {
        pos++;
        tok = new Token(LPAREN);
    }
    else if (input[pos] == ')')
    {
        pos++;
        tok = new Token(RPAREN);
    }
    //Identificadores
    else if (isalpha(input[pos]) || input[pos] == '_')
    {
        //lexeme.push_back(input[pos]);
        pos++;
        while (isalnum(input[pos]) || input[pos] == '_')
        {
            //lexeme.push_back(input[pos]);
            pos++;
        }

        tok = new Token(ID);
    }
    //Números
    else if (isdigit(input[pos]))
    {
        //lexeme.push_back(input[pos]);
        pos++;
        while (isdigit(input[pos]))
        {
            //lexeme.push_back(input[pos]);
            pos++;
        }

        tok = new Token(NUMBER);

    }
    else
        lexicalError();

    return tok;
}

void 
Scanner::lexicalError()
{
    cout << "Token mal formado\n";
    
    exit(EXIT_FAILURE);
}
